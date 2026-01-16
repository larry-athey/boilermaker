# Frequently Asked Questions

Q: Are there any plans to add volt/amp meter functionality to the Boilermaker's user interface?

A: No, and this really should be a meter on the outlet or junction box that you are connected to because that's what you should actually be monitoring. Those meters are dirt cheap, just connect it to your AC feed lines and put the inductive donut around your hot wire.

---

Q: Why would my Boilermaker randomly become unreachable over my WiFi?

A: Router configuration issues. Your router is likely set to WPA3 and/or has band steering enabled so it's trying to shove the Boilermaker over to the 5 GHz WiFi which it does not support. Switch to WPA2 and disable band steering by using a separate SSIDs for the 2.4 GHz and 5 GHz WiFi networks. Lastly, if you still have issues, look in your router's QoS section and add a reservation for your Boilermaker there. That basically puts a "DO NOT TOUCH" label on it so the router won't screw with it anymore.

---

Q: Can you add charting and voice alerts to the Boilermaker like you have in your [RPi Smart Still](https://github.com/larry-athey/rpi-smart-still) controller?

A: The ESP32 that runs the Boilermaker doesn't have enough code space for this. I would recommend using an open source solution such as [Grafana](https://github.com/grafana/grafana) for this and set your own alarm functions within it to notify you audibly when specific criteria are met. There are plenty of YouTube tutorials covering the setup and usage of Grafana.

---

Q: Can this be used to control a pellet fed grill/smoker, such as a Traeger?

A: No, but I have [another project](https://github.com/larry-athey/climate-czar/tree/main/CZ_Pellet_Stove_Controller_v2) that would work for that. Although, the precision would leave much too be desired since you can't just "put out a fire" in one of those with an ESP32 to cool it down.

---
