# Frequently Asked Questions

Q: Are there any plans to add volt/amp meter functionality to the Boilermaker's user interface?

A: No, and this really should be a meter on the outlet or junction box that you are connected to because that's what you should actually be monitoring. Those meters are dirt cheap, just connect it to your AC feed lines and put the inductive donut around your hot wire.

---

Q: Why would my Boilermaker randomly become unreachable over my WiFi?

A: Router configuration issues. Your router is likely set to WPA3 and/or has band steering enabled so it's trying to shove the Boilermaker over to the 5 GHz WiFi which it does not support. Switch to WPA2 and disable band steering by using a separate SSIDs for the 2.4 GHz and 5 GHz WiFi networks. Lastly, if you still have issues, look in your router's QoS section and add a reservation for your Boilermaker there. That basically puts a "DO NOT TOUCH" label on it so the router won't screw with it anymore.

---

Q: Can you add charting and voice alerts to the Boilermaker like you have in your [RPi Smart Still](https://github.com/larry-athey/rpi-smart-still) controller?

A: The ESP32 that runs the Boilermaker doesn't have enough code space for this. I would recommend using an open source solution such as [Grafana](https://github.com/grafana/grafana) for this and set your own alarm functions within it to notify you audibly when specific criteria is met.

---
