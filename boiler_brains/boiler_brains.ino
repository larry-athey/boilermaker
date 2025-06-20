//------------------------------------------------------------------------------------------------
// Boilermaker SCR/SSR Brain | (CopyLeft) 2024-Present | Larry Athey (https://panhandleponics.com)
//
// You must be using a v2.x ESP32 library to compile this code. It appears that v3.x libraries do
// not contain compatible headers for certain legacy libraries that I rely on. You should also use
// the following URL in your preferences under Additional Boards Manager URLs.
//
// https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
//
// The project is based on the 38 pin ESP32 WROOM-DA (Dual Antenna) or similar. If yours doesn't
// have an onboard LED attached to GPIO 2, you can compile for the WROOM-DA and solder a 220 ohm
// resistor and LED in series between GPIO 2 and any ground pin. This provides a visual indicator
// of a functional WiFi connection and network activity.
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
// This controller uses either a DS18B20 temperature sensor or a thermocouple which depends on a
// MAX-6675/MAX-31855 amplifier module. The DS18B20 is my normal go-to due to their low price and
// they ore perfectly fine for use in distillation projects since they have an upper temperature
// limit of 125C/257F. Set the default value of SensorType to 1 to use a thermocouple, or use the
// necessary Web API call with your web browser to change the temperature sensor type.
//
// NOTE: When using these controllers in a master/slave setup, all 4 slaves are ran in constant
//       power mode which follows the power level of the master controller. They are still able
//       to run independently in constant power or constant temperature mode, and will maintain
//       their own settings. Also, each slave can have its own 4 slaves as well that will follow
//       what this controller tells it to do. Think of it like a pyramid scheme. LOL!!!
//------------------------------------------------------------------------------------------------
#include "WiFi.h"                // ESP32-WROOM-DA will allow the blue on-board LED to react to WiFi traffic
#include "HTTPClient.h"          // HTTP client library used for communicating with slave units
#include "ESP32Ping.h"           // ICMP (ping) library from https://github.com/marian-craciunescu/ESP32Ping
#include "Preferences.h"         // ESP32 Flash memory read/write library
#include "OneWire.h"             // OneWire Network communications library
#include "DallasTemperature.h"   // Dallas Semiconductor DS18B20 temperature sensor library
#include "max6675.h"             // Adafruit MAX-6675 amplifier library for Type-K thermocouples
//------------------------------------------------------------------------------------------------
#define FAN_OUT 16               // Cooling fan on/off pin (to 1K resistor, to base of 2N3904 transistor) [The PCB has this on GPIO 2, so onboard the WiFi activity LED no longer works]
//#define SCR_OUT 17             // PWM output to an SCR board (comment out if using a zero-crossing SSR)
#define ONE_WIRE 13              // 1-Wire network pin for the DS18B20 temperature sensor
#define thermoMISO 19            // MAX-6675 SPI data bus
#define thermoCS 5               // "                   "
#define thermoCLK 18             // "                   "
//------------------------------------------------------------------------------------------------
#ifndef SCR_OUT
  #include "esp_timer.h"         // High resolution timer library for use with interrupt driven code
  #include "driver/gpio.h"       // ESP-IDF GPIO library
  #define SSR_OUT GPIO_NUM_17    // Same pin as used with an SCR board

  float SSR_PWM = 2.5;           // If using an SSR, how many seconds wide is the PWM duty?
  int dutyCyclePercentage = 0;   // Low frequency PWM duty cycle percentage
  hw_timer_t *timer = NULL;      // High resolution timer library
#endif
//------------------------------------------------------------------------------------------------
OneWire oneWire(ONE_WIRE);
DallasTemperature DT(&oneWire);
MAX6675 thermocouple(thermoCLK,thermoCS,thermoMISO);
Preferences preferences;
WiFiServer Server(80);
//------------------------------------------------------------------------------------------------
bool ActiveRun = false;          // True if there's an active heating run
bool UpToTemp = false;           // True if the run startup has reached operating temperature
bool PWMenabled = false;         // True if the low speed PWM is enabled
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
byte SensorType = 1;             // Selected temperature sensor type: 0=MAX6675, 1=DS18B20
byte SlavesPinging = 0;          // Shows how many configured slaves are actually online
byte AdjustRate = 1;             // How much power % change to make when temperature is out of range
byte FallBackPercent = 50;       // Power % to fall back to when TargetTemp has been reached
byte StartupPercent = 50;        // Power % to start at or target power in OpMode 0
byte PowerLevel = 0;             // Current power level 0-255, (100/255) * PowerLevel = % Power
byte OpMode = 1;                 // Operation mode, 0 = Constant Power, 1 = Constant Temperature
byte wifiCheckCounter = 0;       // Used to check the WiFi connection once per minute
byte wifiMode = 0;               // DHCP (0) or manual configuration (1)
String wifiSSID = "none";        // WiFi network SSID
String wifiPassword;             // WiFi network password
String wifiIP;                   // WiFi network IPV4 address
String wifiMask;                 // WiFi network subnet mask
String wifiGateway;              // WiFi network default gateway address
String wifiDNS;                  // WiFi network DNS resolver address
String slaveIP1 = "";            // Slave unit 1 IPV4 address
String slaveIP2 = "";            // Slave unit 2 IPV4 address
String slaveIP3 = "";            // Slave unit 3 IPV4 address
String slaveIP4 = "";            // Slave unit 4 IPV4 address
String DeviceName;               // Network host name and device name to be displayed in the web UI
String Uptime = "00:00:00";      // Current system uptime
String Runtime = "00:00:00";     // Current heating runtime
String Version = "1.0.1";        // Current release version of the project
//------------------------------------------------------------------------------------------------
#include "slave_sync.h"          // Library for configuring and synchronizing slave units
#include "serial_config.h"       // Library for configuring WiFi connection and slave unit IP addresses
#include "web_ui.h"              // Library for the web user interface and HTTP API implementation
//-----------------------------------------------------------------------------------------------
#ifndef SCR_OUT
void IRAM_ATTR onTimer() { // Custom low frequency PWM designed specifically for SSR usage
  static uint32_t cycleCounter = 0;
  cycleCounter ++;
    
  if (cycleCounter == 10) { // Reset counter every 10 interrupts
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
  Serial.setDebugOutput(false);
  delay(1000);
  PurgeBuffer();
  if (Serial) Serial.println("");

  // Get the last user settings from flash memory
  GetMemory();
  if (wifiSSID == "none") {
    SetMemory();
  } else {
    if ((wifiSSID != "none") && (wifiPassword != "")) ConnectWiFi();
  }

  #ifndef SCR_OUT
  // Custom interrupt driven low speed (.1 Hz to 1 Hz) PWM for driving a solid state relay
  gpio_set_direction(SSR_OUT,GPIO_MODE_OUTPUT);
  gpio_set_level(SSR_OUT,0);
  timer = timerBegin(0,80,true); // Timer at 1 MHz, count up
  timerAttachInterrupt(timer,&onTimer,true); // Attach the PWM toggle function
  timerAlarmWrite(timer,SSR_PWM * 100000,true); // Timer trigger set to 2.5 seconds by default
  // timerAlarmEnable(timer); // Now enable the low speed pulse width modulator
  #else
  // Assign the SCR controller output pin to a PWM channel
  // For heating elements, 1 KHz to 3 KHz is used, adjust as necessary
  ledcSetup(1,2000,8);
  ledcAttachPin(SCR_OUT,1);
  ledcWrite(1,0);
  #endif
 
  DT.begin();
  pinMode(FAN_OUT,OUTPUT); 
  digitalWrite(FAN_OUT,LOW);
  LoopCounter = millis();
  LastAdjustment = LoopCounter;
  if (Serial) {
    ShowConfig();
    ConfigMenu();
  }
}
//------------------------------------------------------------------------------------------------
void ConnectWiFi() { // Connect to WiFi network, must be WPA2-PSK, not WPA3
  byte x = 0;
  if (Server) Server.end();
  WiFi.mode(WIFI_STA);
  if (wifiMode == 1) {
    bool Passed = true;
    int segCount = 0;
    int ipSegments[4];
    int maskSegments[4];
    int gwSegments[4];
    int dnsSegments[4];
    segCount = sscanf(wifiIP.c_str(),"%d.%d.%d.%d",&ipSegments[0],&ipSegments[1],&ipSegments[2],&ipSegments[3]);
    if (segCount != 4) {
      if (Serial) Serial.println("\nCannot parse static IP address!");
      Passed = false;
    }
    segCount = sscanf(wifiMask.c_str(),"%d.%d.%d.%d",&maskSegments[0],&maskSegments[1],&maskSegments[2],&maskSegments[3]);
    if (segCount != 4) {
      if (Serial) Serial.println("\nCannot parse subnet mask!");
      Passed = false;
    }
    segCount = sscanf(wifiGateway.c_str(),"%d.%d.%d.%d",&gwSegments[0],&gwSegments[1],&gwSegments[2],&gwSegments[3]);
    if (segCount != 4) {
      if (Serial) Serial.println("\nCannot parse gateway address!");
      Passed = false;
    }
    segCount = sscanf(wifiDNS.c_str(),"%d.%d.%d.%d",&dnsSegments[0],&dnsSegments[1],&dnsSegments[2],&dnsSegments[3]);
    if (segCount != 4) {
      if (Serial) Serial.println("\nCannot parse DNS resolver address!");
      Passed = false;
    }
    if (Passed) {
      IPAddress staticIP(ipSegments[0],ipSegments[1],ipSegments[2],ipSegments[3]);
      IPAddress subnet(maskSegments[0],maskSegments[1],maskSegments[2],maskSegments[3]);
      IPAddress gateway(gwSegments[0],gwSegments[1],gwSegments[2],gwSegments[3]);
      IPAddress dns(dnsSegments[0],dnsSegments[1],dnsSegments[2],dnsSegments[3]);
      if (! WiFi.config(staticIP,gateway,subnet,dns,dns)) {
        if (Serial) Serial.println("\nWiFi static IP configuration failed!");
        delay(2000);
      }
    } else {
      delay(2000);
    }
  }
  WiFi.setHostname(DeviceName.c_str());
  WiFi.begin(wifiSSID,wifiPassword);
  if (Serial) Serial.print("\nConnecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    if (Serial) Serial.print('.');
    delay(1000);
    x ++;
    if (x == 15) break;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Server.begin();
    wifiIP = WiFi.localIP().toString();
    wifiMask = WiFi.subnetMask().toString();
    wifiGateway = WiFi.gatewayIP().toString();
    wifiDNS = WiFi.dnsIP(0).toString();
    SlavesPinging = PingAllSlaves();
  } else {
    wifiIP = "";
    wifiMask = "";
    wifiGateway = "";
    wifiDNS = "";
    if (Serial) Serial.println("\nConnection Failed!");
    delay(2000);
  }
}
//------------------------------------------------------------------------------------------------
void GetMemory() { // Get the configuration settings from flash memory on startup
  preferences.begin("prefs",true);
  wifiMode         = preferences.getUInt("wifi_mode",0);
  wifiSSID         = preferences.getString("wifi_ssid","none");
  wifiPassword     = preferences.getString("wifi_password","");
  wifiIP           = preferences.getString("wifi_ip","");
  wifiMask         = preferences.getString("wifi_mask","");
  wifiGateway      = preferences.getString("wifi_gateway","");
  wifiDNS          = preferences.getString("wifi_dns","");
  DeviceName       = preferences.getString("device_name","Boilermaker-" + WiFi.macAddress());
  slaveIP1         = preferences.getString("slave1","");
  slaveIP2         = preferences.getString("slave2","");
  slaveIP3         = preferences.getString("slave3","");
  slaveIP4         = preferences.getString("slave4","");
  CorrectionFactor = preferences.getFloat("correction_factor",0.0);
  OpMode           = preferences.getUInt("op_mode",1);
  TargetTemp       = preferences.getFloat("target_temp",80.0);
  StartupPercent   = preferences.getUInt("startup_percent",50);
  FallBackPercent  = preferences.getUInt("fb_percent",50); // Cannot use the word "fallback" for some reason
  AdjustRate       = preferences.getUInt("adjust_rate",1);
  Deviation        = preferences.getFloat("deviation",1.0);
  ChangeWait       = preferences.getUInt("change_wait",120);
  RestPeriod       = preferences.getUInt("rest_period",60);
  SensorType       = preferences.getUInt("sensor_type",SensorType);
  #ifndef SCR_OUT
  SSR_PWM          = preferences.getFloat("ssr_pwm",2.5);
  #endif
  preferences.end();
}
//------------------------------------------------------------------------------------------------
void SetMemory() { // Update flash memory with the current configuration settings
  preferences.begin("prefs",false);
  preferences.putUInt("wifi_mode",wifiMode);
  preferences.putString("wifi_ssid",wifiSSID);
  preferences.putString("wifi_password",wifiPassword);
  preferences.putString("wifi_ip",wifiIP);
  preferences.putString("wifi_mask",wifiMask);
  preferences.putString("wifi_gateway",wifiGateway);
  preferences.putString("wifi_dns",wifiDNS);
  preferences.putString("device_name",DeviceName);
  preferences.putString("slave1",slaveIP1);
  preferences.putString("slave2",slaveIP2);
  preferences.putString("slave3",slaveIP3);
  preferences.putString("slave4",slaveIP4);
  preferences.putFloat("correction_factor",CorrectionFactor);
  preferences.putUInt("op_mode",OpMode);
  preferences.putFloat("target_temp",TargetTemp);
  preferences.putUInt("startup_percent",StartupPercent);
  preferences.putUInt("fb_percent",FallBackPercent); // Cannot use the word "fallback" for some reason
  preferences.putUInt("adjust_rate",AdjustRate);
  preferences.putFloat("deviation",Deviation);
  preferences.putUInt("change_wait",ChangeWait);
  preferences.putUInt("rest_period",RestPeriod);
  preferences.putUInt("sensor_type",SensorType);
  #ifndef SCR_OUT
  preferences.putFloat("ssr_pwm",SSR_PWM);
  #endif
  preferences.end();
}
//------------------------------------------------------------------------------------------------
void TempUpdate() { // Update the temperature sensor values
  if (SensorType == 1) {
    DT.requestTemperatures();
    float Test = DT.getTempCByIndex(0); // Returns -127.00 if the device reading fails
    if (Test > -127.00) TempC = Test;
  } else {
    TempC = thermocouple.readCelsius();
  }
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
  UpdateAllSlaves("/?power=" + String(Percent));
}
//-----------------------------------------------------------------------------------------------
void RunState(byte State) { // Toggle the active heating run state
  if (State == 1) {
    #ifndef SCR_OUT
    timerAlarmEnable(timer);
    PWMenabled = true;
    #endif
    StartTime = millis();
    ActiveRun = true;
    UpToTemp  = false;
    digitalWrite(FAN_OUT,HIGH);
    UpdateAllSlaves("/?data_0=0");
    UpdateAllSlaves("/start-run");
    PowerAdjust(StartupPercent);
  } else {
    Runtime = "00:00:00";
    ActiveRun = false;
    digitalWrite(FAN_OUT,LOW);
    PowerAdjust(0);
    UpdateAllSlaves("/stop-run");
    #ifndef SCR_OUT
    timerAlarmDisable(timer);
    gpio_set_level(SSR_OUT,0);
    PWMenabled = false;
    #endif
  }
}
//-----------------------------------------------------------------------------------------------
String HandleAPI(String Header) { // Handle HTTP API calls (this ain't gonna be pretty)
  //if (Serial) Serial.println("\n" + Header);
  Header.remove(0,4); // Delete the "GET " from the beginning
  Header.remove(Header.indexOf(" HTTP/1.1"),9); // Delete the " HTTP/1.1" from the end
  if (Header == "/ajax-livedata") { // Web UI update, live data card
    return LiveData();
  } else if (Header == "/ajax-settings") { // Web UI update, settings card
    return SettingsData();
  } else if (Header.indexOf("/?data_0=") == 0) { // Set Operation Mode
    if (ActiveRun) {
      return jsonFailure;
    } else {
      Header.remove(0,9);
      OpMode = Header.toInt();
      if (OpMode < 0) OpMode = 0;
      if (OpMode > 1) OpMode = 1;
      SetMemory();
      return jsonSuccess;
    }
  } else if (Header.indexOf("/?data_1=") == 0) { // Set Target Temperature
    Header.remove(0,9);
    TargetTemp = Header.toFloat();
    if (TargetTemp < 0) TargetTemp = 0;
    if (TargetTemp > 260) TargetTemp = 260;
    SetMemory();
    return jsonSuccess;
  } else if (Header.indexOf("/?data_2=") == 0) { // Set Startup Power
    Header.remove(0,9);
    StartupPercent = Header.toInt();
    if (StartupPercent < 10) StartupPercent = 10;
    if (StartupPercent > 100) StartupPercent = 100;
    SetMemory();
    if ((OpMode == 0) && (ActiveRun)) PowerAdjust(StartupPercent);
    return jsonSuccess;
  } else if (Header.indexOf("/?data_3=") == 0) { // Set Fallback Power
    Header.remove(0,9);
    FallBackPercent = Header.toInt();
    if (FallBackPercent < 10) FallBackPercent = 10;
    if (FallBackPercent > 100) FallBackPercent = 100;
    SetMemory();
    return jsonSuccess;
  } else if (Header.indexOf("/?data_4=") == 0) { // Set Adjustment Rate
    Header.remove(0,9);
    AdjustRate = Header.toInt();
    if (AdjustRate < 1) AdjustRate = 1;
    if (AdjustRate > 100) AdjustRate = 100;
    SetMemory();
    return jsonSuccess;
  } else if (Header.indexOf("/?data_5=") == 0) { // Set Deviation Rate
    Header.remove(0,9);
    Deviation = Header.toFloat();
    if (Deviation < 0.1) Deviation = 0.1;
    if (Deviation > 5) Deviation = 5;
    SetMemory();
    return jsonSuccess;
  } else if (Header.indexOf("/?data_6=") == 0) { // Set Change Wait Time
    Header.remove(0,9);
    ChangeWait = Header.toInt();
    if (ChangeWait < 1) ChangeWait = 1;
    if (ChangeWait > 1000) ChangeWait = 1000;
    SetMemory();
    return jsonSuccess;
  } else if (Header.indexOf("/?data_7=") == 0) { // Set Rest Period
    Header.remove(0,9);
    RestPeriod = Header.toInt();
    if (RestPeriod < 1) RestPeriod = 1;
    if (RestPeriod > 1000) RestPeriod = 1000;
    SetMemory();
    return jsonSuccess;
  } else if (Header == "/form-0") { // Get Form: Operation Mode
    return get_Form(0);
  } else if (Header == "/form-1") { // Get Form: Target Temperature
    return get_Form(1);
  } else if (Header == "/form-2") { // Get Form: Startup Power
    return get_Form(2);
  } else if (Header == "/form-3") { // Get Form: Fallback Power
    return get_Form(3);
  } else if (Header == "/form-4") { // Get Form: Adjustment Rate
    return get_Form(4);
  } else if (Header == "/form-5") { // Get Form: Deviation Rate
    return get_Form(5);
  } else if (Header == "/form-6") { // Get Form: Change Wait Time
    return get_Form(6);
  } else if (Header == "/form-7") { // Get Form: Rest Period
    return get_Form(7);
  } else if (Header == "/get-correctionfactor") { // Get the CorrectionFactor value
    return String(CorrectionFactor);
  } else if (Header == "/get-power") { // Get current power percentage
    return String(round(0.392156863 * PowerLevel),0);
  } else if (Header == "/get-slaveip1") { // Get slave 1 IP address
    if (slaveIP1 != "") {
      return slaveIP1;
    } else {
      return "0.0.0.0";
    }
  } else if (Header == "/get-slaveip2") { // Get slave 2 IP address
    if (slaveIP2 != "") {
      return slaveIP2;
    } else {
      return "0.0.0.0";
    }
  } else if (Header == "/get-slaveip3") { // Get slave 3 IP address
    if (slaveIP3 != "") {
      return slaveIP3;
    } else {
      return "0.0.0.0";
    }
  } else if (Header == "/get-slaveip4") { // Get slave 4 IP address
    if (slaveIP4 != "") {
      return slaveIP4;
    } else {
      return "0.0.0.0";
    }
  #ifndef SCR_OUT
  } else if (Header == "/get-ssrpwm") { // Get the SSR_PWM value
    return String(SSR_PWM);
  #endif
  } else if (Header == "/get-runtime") { // Get current heating runtime (seconds)
    if (ActiveRun) {
      return String((millis() - StartTime) / 1000);
    } else {
      return "0";
    }
  } else if (Header == "/get-sensortype") { // Get the current temerature sensor type
    return String(SensorType);
  } else if (Header == "/get-tempc") { // Get current Temp C reading
    return String(TempC,1);
  } else if (Header == "/get-tempf") { // Get current Temp F reading
    return String(TempF,1);
  } else if (Header == "/get-uptime") { // Get current system uptime (seconds)
    return String(millis() / 1000);
  } else if (Header == "/get-wifistats") { // Get current WiFi channel and signal level
    return "WiFi Channel: " + String(WiFi.channel()) + "\n" + "WiFi Signal: " + String(WiFi.RSSI());
  } else if (Header.indexOf("/?power=") == 0) { // Slave mode power jump
    if ((ActiveRun) && (OpMode == 0)) {
      Header.remove(0,8);
      if (Header.toInt() < 10) Header = "10";
      if (Header.toInt() > 100) Header = "100";
      PowerAdjust(Header.toInt());
      return jsonSuccess;
    } else {
      return jsonFailure;
    }
  } else if (Header == "/reboot") { // Reboot the controller
    return "Rebooting...";
  } else if (Header.indexOf("/?set-correctionfactor=") == 0) { // Set new CorrectionFactor value
    Header.remove(0,23);
    CorrectionFactor = Header.toFloat();
    if (CorrectionFactor < -5) CorrectionFactor = -5.0;
    if (CorrectionFactor > 5) CorrectionFactor = 5.0;
    SetMemory();
    return jsonSuccess;
  } else if (Header.indexOf("/?set-sensortype=") == 0) { // Set temperature sensor type
    Header.remove(0,17);
    if (Header.toInt() < 0) Header = "0";
    if (Header.toInt() > 1) Header = "1";
    SensorType = Header.toInt();
    SetMemory();
    return jsonSuccess;
  } else if (Header.indexOf("/?set-slaveip1=") == 0) { // Set new slaveIP1 address
    Header.remove(0,15);
    if (isValidIP(Header)) {
      if (Header == "0.0.0.0") Header = "";
      slaveIP1 = Header;
      SetMemory();
      return jsonSuccess;
    } else {
      return jsonFailure;
    }
  } else if (Header.indexOf("/?set-slaveip2=") == 0) { // Set new slaveIP2 address
    Header.remove(0,15);
    if (isValidIP(Header)) {
      if (Header == "0.0.0.0") Header = "";
      slaveIP2 = Header;
      SetMemory();
      return jsonSuccess;
    } else {
      return jsonFailure;
    }
  } else if (Header.indexOf("/?set-slaveip3=") == 0) { // Set new slaveIP3 address
    Header.remove(0,15);
    if (isValidIP(Header)) {
      if (Header == "0.0.0.0") Header = "";
      slaveIP3 = Header;
      SetMemory();
      return jsonSuccess;
    } else {
      return jsonFailure;
    }
  } else if (Header.indexOf("/?set-slaveip4=") == 0) { // Set new slaveIP4 address
    Header.remove(0,15);
    if (isValidIP(Header)) {
      if (Header == "0.0.0.0") Header = "";
      slaveIP4 = Header;
      SetMemory();
      return jsonSuccess;
    } else {
      return jsonFailure;
    }
  #ifndef SCR_OUT
  } else if (Header.indexOf("/?set-ssrpwm=") == 0) { // Set new SSR_PWM value (requires reboot)
    Header.remove(0,13);
    SSR_PWM = Header.toFloat();
    if (SSR_PWM < 1) SSR_PWM = 1.0;
    if (SSR_PWM > 5) SSR_PWM = 5.0;
    SetMemory();
    return jsonSuccess;
  #endif
  } else if (Header == "/start-run") { // Start a heating run
    if (! ActiveRun) RunState(1);
    return jsonSuccess;
  } else if (Header == "/stop-run") { // Stop the heating run
    RunState(0);
    return jsonSuccess;
  } else if (Header == "/toggle-run") { // Toggle the current run status
    if (ActiveRun) {
      RunState(0);
    } else {
      RunState(1);
    }
    return jsonSuccess;
  } else {
    return HomePage(); // Always send the home page if no recognized queries are received
  }
}
//-----------------------------------------------------------------------------------------------
void HandleSerialInput() { // Handle user configuration via the serial console
  String Option = ReadInput();
  PurgeBuffer();
  Serial.println("\n");
  if (Option == "0" ) {
    get_DeviceName();
  } else if (Option == "1" ) {
    get_wifiSSID();
    if (wifiSSID == "") wifiSSID == "none";
  } else if (Option == "2" ) {
    get_wifiPassword();
  } else if (Option == "3" ) {
    get_wifiMode();
  } else if (Option == "4" ) {
    WiFi.disconnect();
    delay(500);
    if ((wifiSSID != "none") && (wifiPassword != "")) ConnectWiFi();
  } else if (Option == "5" ) {
    get_SlaveIP1();
    if ((slaveIP1 == "0.0.0.0") || (! isValidIP(slaveIP1))) slaveIP1 = "";
  } else if (Option == "6" ) {
    get_SlaveIP2();
    if ((slaveIP2 == "0.0.0.0") || (! isValidIP(slaveIP2))) slaveIP2 = "";
  } else if (Option == "7" ) {
    get_SlaveIP3();
    if ((slaveIP3 == "0.0.0.0") || (! isValidIP(slaveIP3))) slaveIP3 = "";
  } else if (Option == "8" ) {
    get_SlaveIP4();
    if ((slaveIP4 == "0.0.0.0") || (! isValidIP(slaveIP4))) slaveIP4 = "";
  } else if (Option == "9" ) {
    get_CorrectionFactor();
    if (CorrectionFactor < -5) CorrectionFactor = -5.0;
    if (CorrectionFactor > 5) CorrectionFactor = 5.0;
  }
  #ifndef SCR_OUT
  else if (Option == "10" ) {
    get_SSR_PWM();
    if (SSR_PWM < 1) SSR_PWM = 1.0;
    if (SSR_PWM > 5) SSR_PWM = 5.0;
  }
  #endif
  SetMemory();
  ShowConfig();
  ConfigMenu();
}
//------------------------------------------------------------------------------------------------
void loop() {
  int CurrentPercent = round(0.392156863 * PowerLevel);
  long CurrentTime = millis();
  Uptime = formatMillis(CurrentTime);
  wifiCheckCounter ++;
  if (CurrentTime > 4200000000) {
    // Reboot the system if we're reaching the maximum long integer value of CurrentTime (49 days)
    ESP.restart();
  }
  // Check for HTTP API calls and handle as necessary
  WiFiClient Client = Server.available();
  if (Client) {
    CurrentTime = millis(); // Grab this again in case we just processed button presses
    long PreviousTime = CurrentTime;
    String Header = "";
    while (Client.connected() && CurrentTime - PreviousTime <= 2000) { // 2 second connection timeout
      CurrentTime = millis();
      if (Client.available()) {
        char c = Client.read();
        if ((c != '\r') && (c != '\n')) Header += c;
        if (c == '\n') {
          if (Header.indexOf("GET ") == 0) {
            String Content = HandleAPI(Header);
            Client.println(Content);
            if (Content == "Rebooting...") { // Need to wait to reboot until after the response is sent
              Client.stop();
              delay(1000);
              ESP.restart();
            }
            break;
          }
        }
      }
    }
    Client.stop();
  }
  // Check for serial console input and handle as necessary
  if ((Serial) && (Serial.available())) HandleSerialInput();
  if (CurrentTime - LoopCounter >= 1000) {
    TempUpdate();
    if (ActiveRun) {
      Runtime = formatMillis(CurrentTime - StartTime);
      if (OpMode == 1) { // OpMode 1 is constant temperature management
        if (! UpToTemp) {
          if (TempC >= TargetTemp) { // Target temperature has been reached
            UpToTemp = true;
            FallBackTime = millis();
            CurrentPercent = FallBackPercent;
            PowerAdjust(CurrentPercent);
          } else { // Progrssively increase power until target temperature has been reached
            if (CurrentTime - LastAdjustment >= (ChangeWait * 1000)) {
              CurrentPercent += AdjustRate;
              if (CurrentPercent > 100) CurrentPercent = 100;
              PowerAdjust(CurrentPercent);
            }
          }
        } else {
          if (CurrentTime - FallBackTime >= (RestPeriod * 1000)) {
            // You can equate this temperature management method to the operation of a car's cruise control
            // Unlike a PID controller, the heating element is always forcing some amount of heat upward
            if (CurrentTime - LastAdjustment >= (ChangeWait * 1000)) {
              if (TempC >= (TargetTemp + Deviation)) { // Over temperature
                CurrentPercent -= AdjustRate;
                if (CurrentPercent < 10) {
                  CurrentPercent = 10;
                  if (TargetTemp < 38) { // Fermentation temperature selected, stop the PWM
                    timerAlarmDisable(timer);
                    gpio_set_level(SSR_OUT,0);
                    PWMenabled = false;
                  }
                }
                PowerAdjust(CurrentPercent); // Decrease power
              } else if (TempC <= (TargetTemp - Deviation)) { // Under temperature
                CurrentPercent += AdjustRate;
                if (CurrentPercent > 100) CurrentPercent = 100;
                if (! PWMenabled) { // Restart the PWM if a fermentation temperature overage shut it down
                  timerAlarmEnable(timer);
                  PWMenabled = true;
                }
                PowerAdjust(CurrentPercent); // Increase power
              }
            }
          }
        }
      } else { // OpMode 0 is constant power, no temperature management

      }
    }
    if (wifiCheckCounter >= 60) {
      bool PingTest = Ping.ping(wifiGateway.c_str(),2);
      if ((WiFi.status() != WL_CONNECTED) || (! PingTest)) { // Reconnect WiFi if we got dropped
        WiFi.disconnect();
        delay(500);
        if ((wifiSSID != "none") && (wifiPassword != "")) ConnectWiFi();
      }
      SlavesPinging = PingAllSlaves();
      wifiCheckCounter = 0;
    }
    LoopCounter = CurrentTime;
  }
}
//------------------------------------------------------------------------------------------------
/*
// Create & run a new sketch with the following code to fully erase the flash memory of an ESP32

#include <nvs_flash.h>

void setup() {
  nvs_flash_erase(); // erase the NVS partition and...
  nvs_flash_init();  // initialize the NVS partition.
  while(true);
}

void loop() {

}
*/
//------------------------------------------------------------------------------------------------
