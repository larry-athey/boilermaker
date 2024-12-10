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
// the PWM output and temperature sensor input to work with whatever ESP32 board that you have.
//------------------------------------------------------------------------------------------------
#define LOCAL_DISPLAY 1          // Include libraries and code for the LilyGo T-Display-S3 board
//------------------------------------------------------------------------------------------------
#ifdef LOCAL_DISPLAY
#include "Arduino_GFX_Library.h" // Standard GFX library for Arduino, built with version 1.4.9
#include "FreeSans9pt7b.h"       // https://github.com/moononournation/ArduinoFreeFontFile.git
#endif
//------------------------------------------------------------------------------------------------
#include "WiFi.h"                // ESP32-WROOM-DA will allow the blue on-board LED to react to WiFi traffic
#include "ESP32Ping.h"           // ICMP (ping) library used for keep-alive functions and slave testing
#include "OneWire.h"             // OneWire Network communications library
#include "DallasTemperature.h"   // Dallas Temperature DS18B20 temperature sensor library
#include "Preferences.h"         // ESP32 Flash memory read/write library
//------------------------------------------------------------------------------------------------
#ifdef LOCAL_DISPLAY
#define SCREEN_BACKLIGHT 38      // Screen backlight LED pin
#define SCREEN_POWER_ON 15       // Screen power on/off pin
#define INC_BTN 0                // Value + button
#define DEC_BTN 14               // Value - button
//#define SCR_OUT 1              // PWM output to an SCR board (comment out if using an SSR as a simplified PID)
#define ONE_WIRE 13              // 1-Wire network pin for the DS18B20 temperature sensor
#else
//#define SCR_OUT 23             // PWM output to an SCR board (comment out if using an SSR as a simplified PID)
#define ONE_WIRE 15              // 1-Wire network pin for the DS18B20 temperature sensor
#endif
//------------------------------------------------------------------------------------------------
#ifndef SCR_OUT
#include "esp_timer.h"           // High resolution timer library for use with interrupt driven code
#include "driver/gpio.h"         // ESP-IDF GPIO library
  #ifdef LOCAL_DISPLAY
    #define SSR_OUT GPIO_NUM_1   // Same pin as used with an SCR board
  #else
    #define SSR_OUT GPIO_NUM_23  // Same pin as used with an SCR board
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
OneWire oneWire(ONE_WIRE);
DallasTemperature DT(&oneWire);
Preferences preferences;
//------------------------------------------------------------------------------------------------
long LoopCounter = 0;            // Timekeeper for the loop to eliminate the need to delay it
long LastAdjustment = 0;         // Time of the last power adjustment
float TempC = 0;                 // Current temperature reading C
float TempF = 0;                 // Current temperature reading F
float CorrectionFactor = 0;      // How much to reduce DS18B20 readings to reflect internal temperatue
byte PowerLevel = 0;             // Current power level 0-255, (100/255) * PowerLevel = % Power
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

  DT.begin();
  LoopCounter = millis();
  LastAdjustment = LoopCounter;
}
//------------------------------------------------------------------------------------------------
void TempUpdate() { // Update the temperature sensor values
  DT.requestTemperatures();
  TempC = DT.getTempCByIndex(0);
  TempC -= CorrectionFactor; // Adjust if DS18B20 is reflecting too much heating element temperature
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

}
//------------------------------------------------------------------------------------------------
