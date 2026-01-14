# Instructions

<img width="1024" src="https://github.com/user-attachments/assets/9456aade-36ae-4404-bda9-26b92ea68955"><br>

_**IMPORTANT:** Please keep in mind that where you put your temperature sensor matters! No matter what you are using your Boilermaker for, you absolutely DO NOT want to be reading the temperature of your heating element! You need to be monitoring what you are boiling or the air around what you are smoking/roasting._

_**SECONDLY:** If your system appears to stop reading temperature changes when your temperature sensor comes in contact with your boiler, this means that your hot and neutral lines to the Boilermaker's internal power supply or the AC outlet itself are revesed. A quick (however unsafe) solution is to lift the ground connection, but you really should correct the polarity of the hot and neutral wires._

Unlike other WiFi enabled ESP32 powered devices, the Boilermaker does not default as a WiFi access point in order to configure it. I'm already using nearly 75% of the available code space, I don't want to waste the remaining 25% on a one-time-use feature. I'm reserving that 25% for future features and enhancements, so the WiFi configuration and a few other things are configured over a USB cable and a plain old terminal program. Windows doesn't include one by default anymore, but [PuTTY](https://www.putty.org/) is still free and open source.

[Here's how to use PuTTY under Windows to connect to the Boilermaker.](https://x.com/i/grok/share/i61lGPHt1tv3cxOr8ogD5S6SR)

Mac OS users should install the free [CoolTerm](https://freeware.the-meiers.org/) program, which is also available for Linux desktop systems, including Raspberry Pi systems. Linux users can also install any of the many command line terminal programs with their package manager, such as minicom. Seriously, serial terminal programs are as old as mainframe servers, this is far from anything new.

_**NOTE:** You can also connect to the Boilermaker with your mobile phone [according to Grok](https://x.com/i/grok/share/BJil4zNbj3p3dYd8i8HHYdEaw) but I haven't tried this yet. You can also simply use the serial terminal in the Arduino IDE by pressing CTRL+Shift+M._

Windows and Mac users will also need to download and install the [CP210x driver](https://www.silabs.com/developer-tools/usb-to-uart-bridge-vcp-drivers?tab=downloads) so the Boilermaker will appear as a serial port that your terminal program can connect to. Linux users need not bother with this since the driver is already part of the kernel source tree.

Once your Boilermaker is connected to your computer with a USB data cable _(not just a charging cable)_, it will appear as a serial port and your terminal program will see it. The communications parameters are 9600 baud, 8N1. This is an old standard and shouldn't require any special configuration steps on your part.

Once you are connected, press your Enter key to refresh the screen and you will see the following in your terminal. As you can see, the serial terminal is only used for configuring network related settings and a temperature sensor offset in the extremely rare event that you have an inaccurate sensor. No need for the added bloat of an access point and web configutator.<br>

![Serial Configuration Menu](https://github.com/user-attachments/assets/d612be73-193b-4cfc-8d32-baa44cb02adb)

_**IMPORTANT:** Since this project relies on ESP32 v2.14 to v2.17 board libraries, it only supports WPA2 WiFi networks. While it might connect to a WPA3 network at startup, it will repeatedly fall offline and attempt to reconnect every minute. If your router cannot be switched to WPA2 mode, you will need a separate WiFi access point (not an additional routeror WiFi extender) for the Boilermaker to connect to. TP-Link's budget EAP or WA series are best for this._

The menu options are as follows:

**0. Set Device Name** - This is where you give your Boilermaker a personalized name. This will be displayed in the Web UI and is also used as its hostname when it announces its presence on your network.

**1. Set SSID** - This is where you set the WiFi network name that you want to connect to.

**2. Set Password** - This is where you set the WiFi network password.

**3. Set DHCP or Static IP** - This is where you tell your Boilermaker to ask your router for an IP address, or you specifically set an IP address that never changes. It's far beyond the scope of this document to explain how IP addresses and TCP/IP standards work. I will just say that you will make life a lot easier for yourself if you set a static IP address. Otherwise, you will need to go into your router configuration and create a reservation for your Boilermaker so that it gets the same IP address every time.

**4. Connect to WiFi** - Use this option to test your WiFi configuration. Once you can see that your Boilermaker connects to your WiFi, you won't have to worry about it again unless you change your WiFi netwoork name or password. Your Boilermaker will automatically connect to your WiFi network at startup, and will check its connection once per minute and automatically reconnect if necessary.

**5. Set Slave 1 IP** through **8. Set Slave 4 IP** - This applies to all four slave IP address settings. Any Boilermaker can control up to 4 more Boilermakers, whether they are being used as a master or a slave. Which means that every slave can also have up to 4 slaves. You simply provide the IP address of other Boilermakers and they will automatically follow this one's power level and run status.

**9. Set Correction Factor** - In the extremely rare event that you got a temperature sensor that isn't reading correctly, you can enter a number from -5.00 to +5.00 (C) here to shift its readings up or down. Even at that rate, 5 degrees (C) in either direction would tell me to just replace the sensor. But, the option is here in case you ever need it.
