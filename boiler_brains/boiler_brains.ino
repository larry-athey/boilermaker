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
// need to run on separate circuits/phases.
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
    #define SSR_OUT 23           // Same pin as used with an SCR board
  #endif
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
float CorrectionFactor = 0;      // How much to reduce DS18B20 readings to reflect internal temperatue
//------------------------------------------------------------------------------------------------
void setup() {


}
//------------------------------------------------------------------------------------------------
void loop() {


}
//------------------------------------------------------------------------------------------------
