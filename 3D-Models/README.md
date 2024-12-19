<img width="1024" src="https://github.com/user-attachments/assets/18994608-b41e-46a7-9807-5cb6b2ad7947"><br>

This case should print just fine on any 3D printer since it's not unnecessarily large. However this will not be a quick print due to the thickness of the walls, which is necessary since PLA printed items will warp if exposed to escessive heat. I'm not even joking for a minute, on a Creality Ender 5 Pro running at standard resolution and using 30% infill, the top of the case takes 8.5 hours and the bottom takes 21 hours with no supports. Making the walls thinner and reducing the infill doesn't have that much effect on the time.

_**NOTE:** You are free to use any resolution and infill rate of your choosing. I just use 30% infill by default for everything in order to make top surfaces smoother. The difference in print time between 15% and 30% isn't enough to make me lower my standards._

Using my settings with no supports for the bottom or ESP32 cover and the required supports for the ESP32 opening in the lid, below are the weights of each item so you will know how much filament that you need to have left on your roll.

Bottom - 172 grams<br>
Lid - 82 grams<br>
ESP32 Cover - 10 grams<br>

I intend to sell Boilermakers as assembled and tested units _(and pre-configured for the customer's WiF network)_, and I really can't afford to send out units where the case will warp due to excessive internal heat. $30 for a weather-proof ABS outdoor utility enclosure also doesn't make sense to me for something like this. Especially since a 1 KG roll of PLA filament is only $23 and I could print 6 cases.

This case was designed to accommodate an [SSR-40DA](https://www.amazon.com/Inkbird-Solid-Thermostat-Temperature-Controller/dp/B00HV974KC/) mounted to a [large SSR heat sink](https://www.amazon.com/uxcell-Aluminum-Dissipation-Single-10A-100A/dp/B07C8R7MS2/), and supports both a standard [38 Pin ESP32](https://www.amazon.com/gp/product/B09J95SMG7/) or a [LilyGo T-Display-S3](https://www.amazon.com/gp/product/B0BRTT727Z/) in the factory shell _(for those who want a local LCD display)_. The 38 Pin ESP32 mounts in the same place as the LilyGo, but requires an additional cover _(**BM-ESP32-Cover.stl**)_ to be 3D printed to keep it in place.

This case also provides a footprint to mount a 6-column 90mm bus bar in the bottom for all AC power connections, and includes holes for 1/2" strain reliefs in case you decide to sacrifice a 12 gauge extension cord. Holes in the bottom of the case to the left and right of the bus bar are also provided in the event that you want to wall mount the unit.

There is also a 50mm fan mounting footprint between the AC power cable holes. You should install your fan so that it pulls cool air through the heat sink and evacuates the heat from the case. With most fans, this would mean the label side faces outward. I'd also suggest using a fan that comes with a guard/grill. This fan only runs when the Boilermaker is active so as not to fill it up with dust like a computer's cooling fan does.
