Changes since the version 1.02 release

---

- Lots of WiFi related performance improvements and lost connection recovery enhancements.

- PID auto tuning. This was added because no matter how you explain PID to somebody, it's still a rat-maze of if-then-else logic and most people aren't programmers. This requires your boiler to be full of water and the Boilermaker will first raise it to 66C/150F before the tuning begins, which takes 10 to 20 minutes after the boiler is up to temperature. Your web UI and API will become 100% unresponsive during this procedure and you only have 15 seconds to cancel it if you accidentally start it. The only output during this process is via serial connection.

- Over the air firmware updating
