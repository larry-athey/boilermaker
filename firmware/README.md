# Firmware Updating

As of the Boilermaker v1.0.4 source code, end users can now update their firmware over-the-air using a web browser. The initial plan was to provide a USB firmware update utility, but the one that Espressif provides doesn't work like the Arduino IDE does and requires the user to perform circus tricks and learn to use a cryptic program that no non-techie will ever bother with.

To put the Airhead into firmware update mode, open the following URL with your web browser.

**http://boilermaker-ip/update-firmware** (where boilermaker-ip is the IP address of your Boilermaker)

The Boilermaker will start up a WiFi access point with the SSID **"Boilermaker-Updater"** and the password to connect is **"12345678"**.

Now open this URL: **http://192.168.4.1**

If you are unable to connect, check to make sure that your device pulled an IP address from the Airhead. If it does not pull one, you will need to manually configure a static IP address on your device. Here are settings that will work for you if you encounter this problem.

Address: 192.168.4.100<br>
Subnet Mask: 255.255.255.0<br>
Gateway: 192.168.4.1<br>
DNS: 192.168.4.1<br>

Click on the button to browse to your **boilermaker-firmware.bin** file and click the blue button to start the upload.

Once the upload progress bar has completed, the Boilermaker will reboot and will be running the new firmware.
