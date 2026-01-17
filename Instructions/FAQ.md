# Frequently Asked Questions

Q: Why didn't you make this a Bluetooth device with a companion mobile app?

A: Bluetooth has a limited range _(20 to 40 feet in open spaces)_ and I'm not interested in paying all of the Apple developer fees just to put an app in their app store and keep it there. Google's Play Store isn't anywhere near as bad, but it still requires the expense of a code signing signature. TCP/IP allows a person to monitor and control the Boilermaker anywhere on the network and the Web API wouldn't work without TCP/IP. Everybody already has a web browser on their phone or computer, so there's your app.

---

Q: What is the maximum wattage that the Boilermaker can handle?

A: That all depends on the solid state relay that you use. The pre-assembled units that I sell on my website are shipped with an SSR-100DA on a massive aluminum heat sink _(as shown on the home page of this GitHub repo)_ and use a 50mm fan to constantly pull air through the heat sink during an active run. Standard 12-3 wire fits through the strain reliefs with the sheath and is capable of handling up to 20 amps, 10 guage _(30 amp)_ wire could also be used with the sheath pulled back. Watts is calculated as volts * amps.

---

Q: Are there any plans to add volt/amp meter functionality to the Boilermaker's user interface?

A: No, and honestly this should be a meter on the outlet or junction box that you are tapped into because that's what you should be monitoring. Think about it, if you have a downstream cable heating up, that will eat current that your meter needs to run accurately. You want your meter upstream from that so that it can reliably warn you of any downstream problems. Those meters are dirt cheap, just connect it to your AC feed lines to the outlet or junction box and put the inductive donut around your hot wire.

---

Q: What's the reason for having the option of a digital temperature sensor or thermocouple?

A: Thermocouples aren't stable at low temperatures and the Boilermaker would be constantly chasing its tail at boiler temperatures with only a thermocouple. The digital temperature sensor is a much better choice for a boiler. If I limited the Boilermaker to only use the digital temperature sensor, then it would only be usable up to 125C/257F. Adding the option of also using a thermocouple allows the Boilermaker to run an electric grill or oven up to 260C/500F.

---

Q: Why would my Boilermaker randomly become unreachable over my WiFi until I restart it?

A: Router configuration changes needed. Your router is likely set to WPA3 and/or has band steering enabled so it's trying to shove the Boilermaker over to the 5 GHz WiFi which it does not support. Switch to WPA2 and disable band steering by using a separate SSIDs for the 2.4 GHz and 5 GHz WiFi networks. Lastly, if you still have issues, look in your router's QoS section and add a reservation for your Boilermaker there. That basically puts a "DO NOT TOUCH" label on it so the router won't screw with it anymore.

---

Q: Are ISP-provided WiFi routers acceptable for use with a Boilermaker?

A: Unless the ISP gives you full administrative access to the router, then I say absolutely not. ISP tech support is 90% useless in most cases because all they know is what their internal documentation tells them. They won't help you troubleshoot anything any further than what their employee knowledge base covers. This is because __anybody__ can get hired for their job with absolutely zero experience. Get a decent router with decent tech support and online resources.

---

Q: Can you add charting and voice alerts to the Boilermaker like you have in your [RPi Smart Still](https://github.com/larry-athey/rpi-smart-still) controller?

A: The ESP32 that runs the Boilermaker doesn't have enough code space for this. I would recommend using an open source solution such as [Grafana](https://github.com/grafana/grafana) for this and set your own alarm functions within it to notify you audibly when specific criteria are met. There are plenty of YouTube tutorials covering the setup and usage of Grafana.

---

Q: Why do you have to enter all temperature settings in celcius?

A: This leans into the previous question, but it also comes down to the fact that the temperature sensors also run in celcius. I have very little code space remaining in the ESP32 and wasting it on a bunch of temperature translation code is wasteful. It's not just the web UI that would end up with code bloat, but also the web API as well. Most people always use the same temperatures anyway, it's not that hard to simply remember that 180F is 82.2C, 90F is 32.2C, you get the idea. Also the fact is, the majority of the world uses celcius.

---

Q: Can this be used to control a pellet fed grill/smoker, such as a Traeger?

A: No, but I have [another project](https://github.com/larry-athey/climate-czar/tree/main/CZ_Pellet_Stove_Controller_v2) that would work for that. Although, the precision would leave much too be desired since you can't just "put out a fire" in one of those with an ESP32 to cool it down. Electric smokers are easier to control, just kill the power if it's too hot.

---
