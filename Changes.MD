Changes since the version 1.02 release

---

- Lots of WiFi related performance improvements and lost connection recovery enhancements. Today's WiFi 6 and 7 routers have a bad habit of putting devices to sleep if the router feels that they are unattended. This has been causing problems with some Boilermakers in the middle of a run where the only option was to unplug it and plug it back in before the router would allow it to reconnect. This sleep feature has now been disabled and will no longer obey these commands from the router.

- PID auto tuning. This was added because no matter how you explain PID to somebody, it's still a rat-maze of if-then-else logic and most people aren't programmers. This requires your boiler to be full of water and the Boilermaker will first raise it to 66C/150F before the tuning begins, which takes 10 to 20 minutes after the boiler is up to temperature. Your web UI and API will become 100% unresponsive during this procedure and you only have 15 seconds to cancel it if you accidentally start it. The only output during this procedure is via serial connection.

- Over the air firmware updating. This was added so people can update the firmware without the need to install the full development environment. The Boilermaker becomes its own WiFi access point that you connect to and then simply upload the latest **boilermaker-firmware.bin** file with your web browser. Refer to the **/firmware/** directory of this GitHub repository for more information.
