# Web User Interface

The WebUI is designed for cell phone screens in vertical/portrait mode. It will still work on desktop/laptop/tablet screens, but will appear stretched out. I would add more code to fine tune it to be more friendly to larger screen sizes, but I'm working with a limited amount of code space in the ESP32 and I already have it 75% full. If you must use it on a desktop PC, use Firefox and switch it into mobile app view by pressing **CTRL+Shift+M** or **Command+Option+M** if you are using a Mac.

The Boilermaker code is derived from the [Airhead](https://github.com/larry-athey/airhead), which is a port of PHP to C++ from my [RPi Smart Still](https://github.com/larry-athey/rpi-smart-still) boiler management routines. What I've done with the Boilermaker is make various static variables in the Airhead code, user configurable options in the WebUI. The Air Still is a 700 watt heating element coupled with a 1 gallon boiler vessel. The default settings here are the Airhead parameters scaled up to work with a 7 gallon Still Spirits boiler. This should help you adjust the settings to match your boiler.

### First WebUI Data Card

In the first card at the top of the WebUI, you see the device name and the number of configured slave units. Remember, the device name is also its network host name. If you are using DHCP configuration for your WiFi and your router assigns your Boilermaker a new IP address, you will want to look for this name in your router's admin page where it shows the connected 2.4 GHz devices. To avoid this problem, you should assign your Boilermaker a static IP address outside of the router's DHCP pool, or have your router create an IP reservation for your Boilermaker.

The second line here that shows how many configured slave units you have, also shows how many of them are online and responding to pings. You should make sure that your slave units are online before you start a run. Otherwise, you will need to manually switch them constant power mode and then start them after your master unit has started. The HTTP API commands from the master to put slaves in the correct operating mode and start them is only sent once and only if they are pinging.

### Second WebUI Data Card

The second WebUI card displays live data from the Boilermaker. This data in the following two WebUI cards is updated at a random time period anywhere from 4.5 to 5.5 seconds. There is not an actual continuous data stream between your cell phone and the Boilermaker, your phone only requests data updates from it at random times.

**Run State:** This will say "Inactive" in yellow letters if the Boilermaker is idle, or it will say "Active" in red flashing letters if it is actively running your heating element.

**Uptime:** This shows how long your Boilermaker has been running without a restart in Hours:Minutes:Seconds. Keep in mind that ESP32 units only have a timer, not a real time clock. These will only run for 49.7 days before the timer rolls over to zero. The Boilermaker code will reboot the device at that point and that would be a bad thing to happen in the middle of a distillation run.

**Runtime:** This shows how long the current run has been active.

**Temperature:** Shows the current temperature whether or not there is an active run.

**Power Level:** Shows the current power level during an active run.

### Third WebUI Data Card

The third WebUI card displays all of the user configurable settings. These values are also updated on a random time basis since the Boilermaker has a complete HTTP API which allows it to be remotely configured by another device on the network. Tapping/clicking on any of the values shown in light blue will make a popup form appear so you can edit these as you choose. Keep in mind that the ESP32 is only a 240 MHz computer, its web server is a bit laggy. Sometimes it takes a couple seconds for the form to display.

**Current Mode:** This will either say "Constant Power" or "Constant Temp". Constant Power will run the unit like any other SCR power controller. Constant Temp will honor a target temperature and manage the power in order to maintain that temperature according to the settings that you choose below.

**Target Temp:** As the name implies, this is the target temperature that you want to shoot for. Everything is done in Celcius at the hardware level, so you need to enter your choice in Celcius here. The upper limit of 260C is 500F, which will require you to be using a thermocouple instead of a DS18B20. This will allow you to use a Boilermaker to control an electric smoker/grill. I use one to run the cheapest bottom of the barrel [Masterbuilt steam smoker](https://www.amazon.com/gp/product/B07VRJQXGL/) you can buy. Cheap way to have a smart smoker!

**Startup Power:** This parameter has two purposes. In Constant Power mode, this is the power level that the Boilermaker runs at. In Constant Temp mode, this is where your power level starts at as it slowly increases the power level until the **Target Temp** has been reached. 50% is a good starting point and also helps reduce the possibility of scorching suspended solids.

**Fallback Power:** As mentioned in the above, in Constant Temp mode, the Boilermaker starts out at one temperature and slowly increases power until the **Target Temp** has been reached. If you were to simply stop right there, you would quickly overshoot the target, especially if you're at 100%. So, after the **Target Temp** is reached, the Boilermaker has to fall back to a point and then begin temperature management by working up to the required power level. 50% is a good starting point, you will just have to tinker with settings to find the magic number for your setup.

**Adjustment Rate:** This parameter is how much your power is progressively adjusted over time to reach the **Target Temp**. This is also how much to adjust the power up or down when a temperature correction is needed. The temperature management here works exactly like the cruise control in a car. It progressively gets more and more gas to get up to speed, and then falls back. Then the gas is increased or decreased by a certain amount with a delay between adjustments to stay locked on the target speed.

**Deviation Rate:** This parameter determines how much temperature drift is allowed before a correction is required. The default values that I've provided here should be a good starting point for most people, but you will need to tinker with settings to find the magic number for your boiler. You don't want to keep this number too low or you will bog down the ESP32. You also don't want to go too high or you will have yo-yo temperatures. Remember, large volumes of water do not change temperature quickly.

**Change Wait:** As your Boilermaker starts up, this is the amount of time between each **Adjustment Rate** change as it works its way up to **Target Temp**. This is also the amount of time between temperature correction power adjustments. This provides time for the last power adjustment to affect the temperature of the boiler before deciding what the next adjustment should be.

**Rest Period:** After your boiler has reached **Target Temp**, the power decreases to **Fallback Power**, then the Boilermaker rests for a period of time to allow any turbulence in the boiler die down. This allows the temperature sensor to start gathering stable readings before the temperature management routines kick in. One minute works fine in most cases, but the larger the boiler, the longer the rest period will need to be. Yet another setting that you will need to tinker with to find your magic number.
