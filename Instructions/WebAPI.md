# Web API Usage

The WebUI communicates with the Boilermaker using HTTP API calls since it's a single page web user interface that dynamically updates using Ajax refresh. All settings except for the WiFi configuration can be configured using plain old HTTP GET requests, which means you can configure pretty much everything just by typing URLs into your web browser's address bar. WiFi configuration has to be done via serial connection because you can't use the WebAPI unless it's already connected to WiFi.

If you use **curl** in Linux or Mac OS to configure it, you have to tell it to use an old HTTP protocol. This is because there is limited code space in an ESP32 and modern HTTP protocols would have an absolutely huge code footprint. For example, if your Boilermaker has the IP address 192.168.1.150 and you wanted to read the current temperature in Celcius, you would use this command.

`curl -s --http0.9 http://192.168.1.150/get-tempc`

The `-s` option silences the network traffic progress that **curl** normally prints on the screen so all you get is the temperature value, and the `--http0.9` tells **curl** what HTTP protocol to use. If you don't specify the 0.9 protocol, you will only get an empty result.

_**NOTE:** This API only returns JSON objects for success or failure when setting values. Requested values are returned as-is, plain text. The ESP32 has limited code space and storing text strings eats up more space than plain old evaluation code. If I was dealing with unlimited code space like you have on any actual server running PHP, I'd be telling a different story here._

### Boilermaker API Call Listing

Not passing any API call or making an invalid API call will return the entire home page of the WebUI.

**/ajax-livedata** - Used by the WebUI to update the second data card on the screen.

**/ajax-settings** - Used by the WebUI to update the third data card on the screen.

**/ajax-progress** - Used by the WebUI to update the fourth data card on the screen.

**/?data_0=** - Primarily used by the WebUI to set the Operation Mode of the Boilermaker. Passing a value of 0 sets it to Constant Power, 1 sets it to Temperature Cruise, 2 sets it to Brewing/Fermentation. If you have slaves configured, all slaves will be switched to Constant Power mode when a run starts and will follow all power adjustments made by the master. Changes to this will not be honored if there is an active run.

**/?data_1=** - Primarily used by the WebUI to set the Target Temperature of the Boilermaker. The value passed here is the temperature in Celcius and must be a whole or floating point number between 0 and 260. If you are using this Boilermaker with the **RPi Smart Still** controller system, this value will be set lower boiler range temperature of your program when you start a run, and will be dynamically adjusted upward if you have the Progressive Temperature feature enabled.

**/?data_2=** - Primarily used by the WebUI to set the Startup Power of the Boilermaker. The value passed here must be a whole number between 10 and 100. Adjusting this value during an active Constant Power run will update the current power level.

**/?data_3=** - Primarily used by the WebUI to set the Fallback Power of the Boilermaker. The value passed here must be a whole number between 10 and 100. Adjusting this value during an active Constant Temp run will only work if the Boilermaker hasn't yet reached the target temperature.

**/?data_4=** - Primarily used by the WebUI to set the Adjustment Rate of the Boilermaker. The value passed here must be a whole number between 1 and 100. Adjusting this value during an active Constant Temp run will work at any point during the run.

**/?data_5=** - Primarily used by the WebUI to set the Deviation Rate of the Boilermaker. The value passed here is the temperature in Celcius and must be a whole or floating point number between 0.1 and 5. Adjusting this value during an active Constant Temp run will work at any point during the run.

**/?data_6=** - Primarily used by the WebUI to set the Change Wait of the Boilermaker. The value passed here must be a whole number between 1 and 1000 (seconds). Adjusting this value during an active Constant Temp run will work at any point during the run.

**/?data_7=** - Primarily used by the WebUI to set the Rest Period of the Boilermaker. The value passed here must be a whole number between 1 and 1000 (seconds). Adjusting this value during an active Constant Temp run will only work if the Boilermaker hasn't yet reached the target temperature.

**/?data_8=** - Primarily used by the web UI to set the PID proportional gain value. The value passed here is a floating point number between 0.1 and 10.0 and will not be honored if there is an active run.

**/?data_9=** - Primarily used by the web UI to set the PID integral gain value. The value passed here is a floating point number between 0.001 and 0.5 and will not be honored if there is an active run.

**/?data_10=** - Primarily used by the web UI to set the PID derivative gain value. The value passed here is a floating point number between 0.0 and 2.0 and will not be honored if there is an active run.

**/?data_11=** - Primarily used by the web UI to set the PID sample time value. The value passed here is a floating point number between 5.0 and 30.0 and will not be honored if there is an active run.

**/?data_12=** - Set the SSR low speed PWM duty time. The value here is a whole or floating point number between 1 and 5 _(seconds)_. The magic number for your system depends on the wattage and intended supply voltage of your heating element(s). If you are using a 1000 watt 240 volt element running on 120 volts, you would likely be better off with a 2.5 second duty time due to the slower reaction time of the element. If it's a 1000 watt 120 volt element at 120 volts, then you'll probably want a 1 second duty width. The slower the reaction time, the wider the duty time should be. If using this code with an SCR board, this is not a valid API call. You will need to restart your run to make this setting take effect.

**/?data_13=** - Primarily used by the web UI to set the progressive temperature feature on or off. The value passed here must be a whole number between 0 and 1 and will be rejected if there is an active run.

**/?data_14=** - Primarily used by the web UI to set the progressive temperature range. The value passed here must be a whole number between 1 and 50 for degrees celcius to add to the target temperature and will be rejected if there is an active run.

**/?data_15=** - Primarily used by the web UI to set the progressive temperature time. The value passed here must be a whole number between 1 and 24 hours and will be rejected if there is an active run.

**/form-0** through **/form-15** - Used by the WebUI to dynamically generate the various settings editor forms for the **/?data_*=** hooks.

**/get-correctionfactor** - Returns the current temperature sensor correction factor in Celcius.

**/get-power** - Returns the current power percentage level.

**/get-runtime** - Returns the total elapsed seconds of the currently active run, or zero if there is no active run.

**/get-slaveip1** through **/get-slaveip4** - Returns the IP address of the requested slave unit and will return 0.0.0.0 if the requested slave unit is not configured.

**/get-ssrpwm** - If using this code with an SSR and low speed PWM, this returns the duty width of the PWM in seconds. If using this code with an SCR controller, this is not a valid API call.

**/get-sensortype** - Returns the selected temperature sensor type. 0=MAX-6675/MAX-31855, 1=DS18B20

**/get-tempc** - Returns the current measured temperature in Celcius (after correction factor).

**/get-tempf** - Returns the current measured temperature in Fahrenheit (after correction factor).

**/get-uptime** - Returns the current system uptime in seconds.

**/get-wifistats** - Returns the current WiFi channel and signal level as two formatted lines of text. Keep in mind that a signal level of -40 is stronger than -50, the closer to zero the stronger the signal.

**/?power=** - Performs a power jump during an active Constant Power run without affecting the Startup Power setting stored in memory. The value passed here must be a whole number between 10 and 100. If you are manually controlling slaves, you must first put the slave in Constant Power mode and then start it before making this call or or the slave will start up at its pre-programmed Startup Power level.

**/reboot** - Reboots the Boilermaker. _(Any surprise there?)_ If you use **/get-uptime** and see that the Boilermaker has been running anywhere near 49 days, you should reboot it before starting a run because the internal code could reboot the unit in the middle of your run if you hit its maximum allowed uptime of 49.7 days.

**/?set-correctionfactor=** - Set a new temperature sensor correction factor. The value passed here is a temperature in Celcius and must be a whole or floating point number between -5 and 5. If you are using a sensor mounted outside of the boiler, such as when the sensor is mounted to the bottom like an Air Still. You will need to use a positive correction factor since it will be reading lower than the actual contents of the boiler. For example, the Airhead uses a +5 correction factor in most cases.

**/?set-sensortype=** - Set a new temperature sensor type. 0=MAX-6675/MAX-31855, 1=DS18B20

**/?set-slaveip1=** through **/?set-slaveip4=** - Set the IP address of the 4 possible slave units. This can be either IPV4 or IPV6 format. Setting a slave's IP to **0.0.0.0** will unassign that memory slot.

**/start-run** - Used to start a run.

**/stop-run** - Used to stop an active run.

**/toggle-run** - Primarily used by the WebUI to toggle the active run state of the Boilermaker.
