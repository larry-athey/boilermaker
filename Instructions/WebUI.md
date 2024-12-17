# Web User Interface

The WebUI is designed for cell phone screens in vertical/portrait mode. It will still work on desktop/laptop/tablet screens, but will appear stretched out. I would add more code to fine tune it to be more friendly to larger screen sizes, but I'm working with a limited amount of code space in the ESP32 and I already have it 75% full. If you must use it on a desktop PC, use Firefox and switch it into mobile app view by pressing **CTRL+Shift+M** or **Command+Option+M** if you are using a Mac.

It is important to remember that the Boilermaker code is derived from the [Airhead](https://github.com/larry-athey/airhead), which is a port of PHP to C++ from my [RPi Smart Still](https://github.com/larry-athey/rpi-smart-still) boiler management routines. What I've done with the Boilermaker is make various static variables in the Airhead code, user configurable options in the WebUI. This conveniently makes it possible to explain what these settings are doing with the 700 watt heating element in a 1 gallon boiler vessel. You can then adapt this information to your larger heating element and boiler vessel.

### First WebUI Data Card

In the first card at the top of the WebUI, you see the device name and the number of configured slave units. Remember, the device name is also its network host name. If you are using DHCP configuration for your WiFi and your router assigns your Boilermaker a new IP address, you will want to look for this name in your router's admin page where it shows the connected 2.4 GHz devices. To avoid this problem, you should assign your Boilermaker a static IP address outside of the router's DHCP pool, or have your router create an IP reservation for your Boilermaker.

The second line here that shows how many configured slave units you have, also shows how many of them are online and responding to pings. You should make sure that your slave units are online before you start a run. Otherwise, you will need to manually switch them constant power mode and then start them after your master unit has started. The HTTP API commands from the master to put slaves in the correct operating mode and start them is only sent once. Power adjustment commands are only sent to a slave if it is pinging.

### Second WebUI Data Card

The second WebUI card displays live data from the Boilermaker. This data in the following two WebUI cards is updated at a random time period anywhere from 4.5 to 5.5 seconds, there is not an actual continuous data stream between your cell phone and the Boilermaker, your phone only requests data updates from it at random times.
