//------------------------------------------------------------------------------------------------
// Boilermaker SCR/SSR Brain | (CopyLeft) 2024-Present | Larry Athey (https://panhandleponics.com)
//
// Derived from the boiler management routines in my Airhead upgrade for Air Stills (and clones).
// WiFi enabled with a built-in web interface that you can access from your phone's web browser to
// control the unit. Initial configuration to set up the WiFi connection is performed using a USB
// cable and serial terminal program such as PuTTY for Windows or CoolTerm for Mac OS.
//
// Each unit on the same WiFi network can either be a master or the default of stand-alone/slave.
// During the serial configuration, you just provide the IP addresses of all slave units and your
// unit instantly sees itself as the master. It will then run the slave units in sync with itself.
// This would be used in large boilers where you have multiple high wattage heating elements that
// need to run on separate circuits/phases. You may add up to 4 slave unit IP addresses.
//
// All configuration of the temperature and power management is performed using HTTP GET requests
// to the unit's web API. This is how a master unit controls slaves on the network. This can also
// be used to implement Boilermakers into other systems without the need to wire them in and add
// code for any other system's proprietary protocols. Use a script as a translator if necessary.
//
// This code can be used with a LilyGo T-Display-S3, or can be headless without any local display.
// Comment out the LOCAL_DISPLAY constant below to use this code with any other ESP32 board. I've
// chosen a 38 pin unit as the alternative, but you can just change the GPIO pin assignments for
// the PWM output and temperature sensor inputs to work with whatever ESP32 board that you have.
//
// By default, this controller uses a DS18B20 temperature sensor which has the necessary range for
// use with a boiler (maximum of 125C/257F). If you intend to use this controller with an electric
// smoker, oven, or slow cooker where a hgher temperature needs to be monitored, you will need to
// use a Type-K thermocouple and a MAX-6675 amplifier. Comment out the DS18B20 constant for this.
//------------------------------------------------------------------------------------------------
#define LOCAL_DISPLAY            // Include libraries and code for the LilyGo T-Display-S3 board
#define DS18B20                  // Use DS18B20 temperature sensor instead of Type-K thermocouple
//------------------------------------------------------------------------------------------------
#ifdef LOCAL_DISPLAY
#include "Arduino_GFX_Library.h" // Standard GFX library for Arduino, built with version 1.4.9
#include "FreeSans9pt7b.h"       // https://github.com/moononournation/ArduinoFreeFontFile.git
#endif
//------------------------------------------------------------------------------------------------
#include "WiFi.h"                // ESP32-WROOM-DA will allow the blue on-board LED to react to WiFi traffic
#include "ESP32Ping.h"           // ICMP (ping) library used for keep-alive functions and slave testing
#include "Preferences.h"         // ESP32 Flash memory read/write library
#ifdef DS18B20
#include "OneWire.h"             // OneWire Network communications library
#include "DallasTemperature.h"   // Dallas Semiconductor DS18B20 temperature sensor library
#else
#include "max6675.h"             // Adafruit MAX-6675 amplifier library for Type-K thermocouples
#endif
//------------------------------------------------------------------------------------------------
#ifdef LOCAL_DISPLAY
#define SCREEN_BACKLIGHT 38      // Screen backlight LED pin
#define SCREEN_POWER_ON 15       // Screen power on/off pin
#define INC_BTN 0                // Value + button
#define DEC_BTN 14               // Value - button
//#define SCR_OUT 1              // PWM output to an SCR board (comment out if using an SSR as a simplified PID)
  #ifdef DS18B20
    #define ONE_WIRE 13          // 1-Wire network pin for the DS18B20 temperature sensor
  #else
    #define thermoDO 11          // MAX-6675 SPI data bus
    #define thermoCS 10          // "                   "
    #define thermoCLK 12         // "                   "
  #endif
#else
//#define SCR_OUT 17             // PWM output to an SCR board (comment out if using an SSR as a simplified PID)
  #ifdef DS18B20
    #define ONE_WIRE 15          // 1-Wire network pin for the DS18B20 temperature sensor
  #else
    #define thermoDO 19          // MAX-6675 SPI data bus
    #define thermoCS 23          // "                   "
    #define thermoCLK 5          // "                   "
  #endif
#endif
//------------------------------------------------------------------------------------------------
#ifndef SCR_OUT
#include "esp_timer.h"           // High resolution timer library for use with interrupt driven code
#include "driver/gpio.h"         // ESP-IDF GPIO library
  #ifdef LOCAL_DISPLAY
    #define SSR_OUT GPIO_NUM_1   // Same pin as used with an SCR board
  #else
    #define SSR_OUT GPIO_NUM_17  // Same pin as used with an SCR board
  #endif
int dutyCyclePercentage = 0;
hw_timer_t *timer = NULL;
#endif
//------------------------------------------------------------------------------------------------
#ifdef LOCAL_DISPLAY
Arduino_DataBus *bus = new Arduino_ESP32PAR8Q(7 /* DC */, 6 /* CS */, 8 /* WR */, 9 /* RD */,39 /* D0 */, 40 /* D1 */, 41 /* D2 */, 42 /* D3 */, 45 /* D4 */, 46 /* D5 */, 47 /* D6 */, 48 /* D7 */);
Arduino_GFX *gfx = new Arduino_ST7789(bus, 5 /* RST */, 0 /* rotation */, true /* IPS */, 170 /* width */, 320 /* height */, 35 /* col offset 1 */, 0 /* row offset 1 */, 35 /* col offset 2 */, 0 /* row offset 2 */);
Arduino_Canvas_Indexed *canvas = new Arduino_Canvas_Indexed(320 /* width */, 170 /* height */, gfx);
#endif
#ifdef DS18B20
OneWire oneWire(ONE_WIRE);
DallasTemperature DT(&oneWire);
#else
MAX6675 thermocouple(thermoCLK,thermoCS,thermoDO);
#endif
Preferences preferences;
//------------------------------------------------------------------------------------------------
bool ActiveRun = false;          // True if there's an active heating run
bool UpToTemp = false;           // True if the run startup has reached operating temperature
long LoopCounter = 0;            // Timekeeper for the loop to eliminate the need to delay it
long StartTime = 0;              // Start time of the current heating run
long LastAdjustment = 0;         // Time of the last power adjustment
long FallBackTime = 0;           // Time that TargetTemp was reached
float TempC = 0;                 // Current temperature reading C
float TempF = 0;                 // Current temperature reading F
float CorrectionFactor = 0;      // How much to correct temp sensor C readings (positive or negative)
float TargetTemp = 80;           // Target temperature (C) if OpMode = 1 is selected
float Deviation = 1;             // How many degrees the temperature is allowed to deviate
int ChangeWait = 120;            // How many seconds to wait between power adjustments
int RestPeriod = 60;             // Seconds to wait after fall back before temperature management
byte ChangePercent = 1;          // How much power % change to make when temperature is out of range
byte FallBackPercent = 50;       // Power % to fall back to when TargetTemp has been reached
byte StartupPercent = 50;        // Power % to start at when running in OpMode 1
byte PowerLevel = 0;             // Current power level 0-255, (100/255) * PowerLevel = % Power
byte OpMode = 0;                 // Operation mode, 0 = Power, 1 = Temperature
byte wifiMode = 0;               // DHCP (0) or manual configuration (1)
String wifiSSID;                 // WiFi network SSID
String wifiPassword;             // WiFi network password
String wifiIP;                   // WiFi network IPV4 address
String wifiMask;                 // WiFi network subnet mask
String wifiGateway;              // WiFi network default gateway address
String wifiDNS;                  // WiFi network DNS resolver address
String slaveIP1;                 // Slave unit 1 IPV4 address
String slaveIP2;                 // Slave unit 2 IPV4 address
String slaveIP3;                 // Slave unit 3 IPV4 address
String slaveIP4;                 // Slave unit 4 IPV4 address
String DeviceName;               // Device name to be displayed in the web UI
char Runtime[10];                // HH:MM:SS formatted time of the current controller run
//------------------------------------------------------------------------------------------------
#ifndef SCR_OUT
void IRAM_ATTR onTimer() { // Custom low frequency PWM similar to what you see in a PID controller
  static uint32_t cycleCounter = 0;
  cycleCounter ++;
    
  if (cycleCounter == 10) { // Reset counter every 10 interrupts (equivalent to 2.5 seconds if interrupt every 250ms)
    cycleCounter = 0;
  }

  if (cycleCounter < (dutyCyclePercentage / 10)) { // Turn on if within duty cycle, power level can never be < 10%
    gpio_set_level(SSR_OUT,1);
  } else {
    gpio_set_level(SSR_OUT,0);
  }
}
#endif
//------------------------------------------------------------------------------------------------
void setup() {
  // Enable serial communications for WiFi setup and slave IP address management
  Serial.begin(9600);
  delay(1000);
  Serial.println("");

  // Get the last user settings from flash memory
  GetMemory();
  if (wifiSSID == "none") {
    // New chip, flash memory not initialized
    //UserTemp1 = 80;
    //UserTemp2 = 86;
    //UserTime  = 2;
    //UserPower = 80;
    //UserMode  = 1;
    SetMemory();
  }

  #ifdef LOCAL_DISPLAY
  // Power up the screen and backlight
  pinMode(SCREEN_POWER_ON,OUTPUT);
  digitalWrite(SCREEN_POWER_ON,HIGH);
  ledcSetup(0,2000,8);
  ledcAttachPin(SCREEN_BACKLIGHT,0);
  ledcWrite(0,255); // Screen brightness (0-255)
  // Initialize the graphics library and blank the screen
  gfx->begin();
  gfx->setRotation(3);
  gfx->fillScreen(BLACK);
  // In order to eliminate screen flicker, everything is plotted to an off-screen buffer and popped onto the screen when done
  canvas->begin();
  //ScreenUpdate();
  #endif

  #ifndef SCR_OUT
  gpio_set_direction(SSR_OUT,GPIO_MODE_OUTPUT);
  gpio_set_level(SSR_OUT,0);
  // Timer setup for 2.5 second period (100% duty cycle would be on for 2.5 seconds, off for none)
  // All heating elements have a slow reaction time, so an SCR's switching frequency is wasteful
  timer = timerBegin(0,80,true); // Timer at 1 MHz, count up
  timerAttachInterrupt(timer,&onTimer,true); // Attach the PWM toggle function
  timerAlarmWrite(timer,250000,true); // Timer trigger set to 250ms (250,000 microseconds)
  timerAlarmEnable(timer); // Now enable the .20 Hz pulse width modulator
  #else
  // Assign the SCR controller output pin to a PWM channel
  // For heating elements, 1 KHz to 3 KHz is used, adjust as necessary
  ledcSetup(1,2000,8);
  ledcAttachPin(SCR_OUT,1);
  ledcWrite(1,0);
  #endif

  #ifdef DS18B20
  DT.begin();
  #else

  #endif
  LoopCounter = millis();
  LastAdjustment = LoopCounter;
}
//------------------------------------------------------------------------------------------------
void GetMemory() { // Get the configuration settings from flash memory on startup
  preferences.begin("prefs",true);
  //UserTemp1 = preferences.getUInt("usertemp1",0);
  //UserTemp2 = preferences.getUInt("usertemp2",0);
  //UserTime  = preferences.getUInt("usertime",0);
  //UserPower = preferences.getUInt("userpower",0);
  //UserMode  = preferences.getUInt("usermode",0);
  preferences.end();
}
//------------------------------------------------------------------------------------------------
void SetMemory() { // Update flash memory with the current configuration settings
  preferences.begin("prefs",false);
  //preferences.putUInt("usertemp1",UserTemp1);
  //preferences.putUInt("usertemp2",UserTemp2);
  //preferences.putUInt("usertime",UserTime);
  //preferences.putUInt("userpower",UserPower);
  //preferences.putUInt("usermode",UserMode);
  preferences.end();
}
//------------------------------------------------------------------------------------------------
void TempUpdate() { // Update the temperature sensor values
  #ifdef DS18B20
  DT.requestTemperatures();
  TempC = DT.getTempCByIndex(0);
  #else
  TempC = thermocouple.readCelsius();
  #endif
  TempC += CorrectionFactor; // CorrectionFactor can be a positive or negative value to calibrate
  TempF = TempC * 9 / 5 + 32;
}
//-----------------------------------------------------------------------------------------------
void PowerAdjust(byte Percent) { // Set the SCR board or SSR timing to a target power percentage
  LastAdjustment = millis();
  #ifndef SCR_OUT
  // This is a digital power controller, there are only on and off states
  dutyCyclePercentage = Percent;
  PowerLevel = round(Percent * 2.55);
  #else
  // This is an analog power controller, first set the power level to zero
  // and rest 1 second so all of the capacitors can fully discharge
  ledcWrite(1,0);
  delay(1000);
  // Then progressively adjust the power level up to the requested percentage
  if (Percent > 0) {
    PowerLevel = round(Percent * 2.55);
    float x = 2.55;
    while (x <= PowerLevel) {
      ledcWrite(1,x); // Function appears to round x on its own, no errors thrown
      delay(10);
      x += 2.55;
    }
  } else {
    PowerLevel = 0;
  }
  #endif
}
//-----------------------------------------------------------------------------------------------
void loop() {
  int CurrentPercent = round(0.392156863 * PowerLevel);
  long CurrentTime = millis();
  if (CurrentTime > 4200000000) {
    // Reboot the system if we're reaching the maximum long integer value of CurrentTime
    ESP.restart();
  }
  if (CurrentTime - LoopCounter >= 1000) {

    #ifdef LOCAL_DISPLAY
    //ScreenUpdate();
    #endif
    LoopCounter = CurrentTime;
  }
}
//------------------------------------------------------------------------------------------------
