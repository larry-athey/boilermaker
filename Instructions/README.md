# Instructions

Unlike other WiFi enabled ESP32 powered devices, the Boilermaker does not default as a WiFi access point in order to configure it. I'm already using nearly 75% of the available code space, I don't want to waste the remaining 25% on a one-time-use feature. I'm reserving that 25% for future features and enhancements, so the WiFi configuration and a few other things are configured over a USB cable and a plain old terminal program. Windows doesn't include one by default anymore, but [PuTTY](https://www.putty.org/) is still free and open source.

[Here's how to use PuTTY under Windows to connect to the Boilermaker.](https://x.com/i/grok/share/i61lGPHt1tv3cxOr8ogD5S6SR)

Mac OS users should install the free [CoolTerm](https://freeware.the-meiers.org/) program, which is also available for Linux desktop systems, including Raspberry Pi systems. Linux users can also install any of the many command line terminal programs with their package manager, such as minicom. Seriously, serial terminal programs are as old as mainframe servers, this is far from anything new.

Once your Boilermaker is connected to your computer with a USB data cable _(not just a charging cable)_, it will appear as a serial port and your terminal program will see it. The communications parameters are 9600 baud, 8N1. This is an old standard and shouldn't require any special configuration steps on your part.

Once you are connected, press your Enter key to refresh the screen and you will see the following in your terminal.

![Serial Configuration Menu](https://github.com/user-attachments/assets/d612be73-193b-4cfc-8d32-baa44cb02adb)

