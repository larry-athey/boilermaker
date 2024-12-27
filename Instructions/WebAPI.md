# Web API Usage

The WebUI communicates with the Boilermaker using HTTP API calls since it's a single page web user interface that dynamically updates using Ajax refresh. All settings except for the WiFi configuration can be configured using plain old HTTP GET requests, which means you can configure pretty much everything just by typing URLs into your web browser's address bar. WiFi configuration has to be done via serial connection because you can't use the WebAPI unless it's already connected to WiFi.

If you use **curl** in Linux or Mac OS to configure it, you have to tell it to use an old HTTP protocol. This is because there is limited code space in an ESP32 and modern HTTP protocols would have an absolutely huge code footprint. For example, if your Boilermaker has the IP address 192.168.1.150 and you wanted to read the current temperature in Celcius, you would use this command.

`curl -s --http0.9 http://192.168.1.150/get-tempc`

The `-s` option silences the network traffic progress that **curl** normally prints on the screen so all you get is the temperature value, and the `--http0.9` tells **curl** what HTTP protocol to use. If you don't specify the 0.9 protocol, you will only get an empty output.

### Boilermaker API Call Listing

Not passing any API call or making an invalid API call will return the entire home page of the WebUI.

**/ajax-livedata** - Used by the WebUI to update the second data card on the screen.

**/ajax-settings** - Used by the WebUI to update the third data card on the screen.

**/?data_0=** - Primarily used by the WebUI to set the operation mode of the Boilermaker. Passing a value of 0 sets it to Constant Power, 1 sets it to Constant Temp. If you have slaves configured, all slaves will be switched to Constant Power mode when a run starts and will follow all power adjustments made by the master.

**/?data_1=** - Primarily used by the WebUI to set the target temperature of the Boilermaker. The value passed here is the temperature in Celcius and must be a whole or floating point number between 0 and 260. If you are using this Boilermaker with the **RPi Smart Still** controller system, this value will be set lower boiler range temperature of your program when you start a run, and will be dynamically adjusted upward if you have the Progressive Temperature feature enabled.

**/?data_2=** - Primarily used by the WebUI to set the startup power of the Boilermaker. The value passed here must be a whole number between 10 and 100. Adjusting this during an active run does not change the current power level, you will need to restart the run.

**/?data_3=** - Primarily used by the WebUI to set the fallback power of the Boilermaker. The value passed here must be a whole number between 10 and 100. Editing this value during a live Constant Temp run will work if the Boilermaker hasn't yet reached the target temperature.
