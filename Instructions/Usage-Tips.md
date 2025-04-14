# Boilermaker Usage Tips

Although the Boilermaker adds temperature control automation to your boiler, it still needs to be tuned to your boiler. This isn't anything complicated and is done by simply heating up water to find the magic numbers that work with your setup. Even though an Air Still and a Still Spirits T500 system both accomplish the same end result, their settings are quite different. Compare the two below targeted at 80C/176F.

Air Still: 1 gallon, 700 watt 240 volt element running at 120 volts.
- Target Temperature: 80.0C/176.0F
- Startup Power: 50%
- Fallback Power: 33%
- Adjustment Rate: 1%
- Deviation Rate: 0.2C
- Change Wait: 30 seconds
- Rest Period: 60 seconds
- SSR PWM Duty: 2.5 seconds

Still Spirits T500: 6 gallons, 1500 watt 120 volt element running at 120 volts.
- Target Temperature: 80.0C/176.0F
- Startup Power: 90%
- Fallback Power: 40%
- Adjustment Rate: 1%
- Deviation Rate: 0.5C
- Change Wait: 30 seconds
- Rest Period: 30 seconds
- SSR PWM Duty: 1 second

Once they both do the fallback and stabilize, the Air Still will hover between 10% and 13%, the Still Spirits T500 will hover between 18% and 20%. Yes, they will actually work downward when targeting a temperature below the liquid's boiling point. These numbers will slightly vary when you have ethanol in the boiler and depending on your altitude. The Air Still will do its fallback well before hitting 100% power, the Still Spirits T500 will work up to 100% and stay there for about an hour before doing its fallback.

If you have a well insulated boiler or a jacketed boiler, you shouldn't be surprised to see the Boilermaker hover at 10% and stay there. The power level will never drop below 10% because this constant amount of at least some heat rising up through the boiler helps prevent suspended solids from falling out of solution and scorching to the bottom or your heating element. This was the primary problem that I was intending to cure when I came up with the [Airhead](https://github.com/larry-athey/airhead) project.

If you find that your boiler tends to run hot after the fallback and the Boilermaker always bottoms out to 10% power, reduce the SSR PWM duty width. If you find that the boiler just takes forever to stabilize the temperature after fallback, increase the SSR PWM duty width. You can set this with the USB serial connection or by issuing WebAPI calls with your web browser. Just remember to reboot your Boilermaker after making any changes to this setting since this is only initialized at startup.

When using the Boilermaker with the [RPi Smart Still](https://github.com/larry-athey/rpi-smart-still) you can duplicate the functionality of the Airhead's operating Mode 3 _(progressive temperature over time)_. Once you enable Boilermaker support, go back to **Management->Configure Heating**, then set the **Fixed Temperature** drop-down to **No**, and set the **Time Spread** value to however many hours you expect your run to take. This is how long the run is expected to take **after** your boiler has reached the program's minimum operating temperature.

The way this works is that the boiler temperature range of your program is divided up so there are 12 equal temperature jumps per hour. If you set the **Time Spread** to 3 hours, then this temperature range is divided by 36 jumps and every 5 minutes that factor is added to the target temperature. You can see this change in the dashboard and the Boilermaker WebUI.

### Using Your Boilermaker For Fermentation

Some people ferment on the grain and may also distill on the grain if they have a boiler with a motorized stirring paddle. Your Boilermaker can maintain fermentation temperatures by using the following settings. The reason this works is because if your wash/mash ever goes over temperature and your power level is at 10%, the PWM completely shuts off.

- Target Temperature: 32.2C/90.0F
- Startup Power: 10%
- Fallback Power: 10%
- Adjustment Rate: 1%
- Deviation Rate: 0.5C
- Change Wait: 30 seconds
- Rest Period: 30 seconds

### Controlling An Electric Smoker

Aside from controlling boilers, another great use for the Boilermaker is turning any absolutely cheap bottom of the line electric smoker into a WiFi enabled **reliable and accurate** electric smoker. My first digital smoker got taken out by a lightning strike and I replaced it with a cheap [Masterbuilt steam smoker](https://www.amazon.com/gp/product/B07VRJQXGL/) and a $25 PID controller. Let's just say that PID controllers have their place, but they're really not the best option for a smoker since the heating element spends more time off than on.

With a PID controller, the heating element barely stays on long enough to get your wood chips smoldering. Whereas with the Boilermaker, the power level never drops below 10%, so there is always some amount of heat coming from the heating element. Even with the default 2.5 second PWM duty, 50% power means the heating element runs for 1.25 seconds before resting for 1.25 seconds, which is actually plenty enough heat to cause the wood chips to start smoldering. 10% is still enough to keep them going

Since a smoker can reach much higher temperatures than a boiler, you will definitely need to use a Type-K thermocouple rather than a DS18B20 temperature sensor or you'll never know when it gets above 125/257F in the smoker. Generally, this wouldn't be a problem with a smoker since people usually slow cook things between 107C/225F and 121C/250F. However, even I have had the random fire flare up in the smoker because grease caught on fire when it dripped onto the red hot heating element.

I do have plans for making a WiFi enabled ESP32 powered temperature probe and timer unit for this Boilermaker application with multiple temperature probes. I just have a few other programming projects that I need to tie up first. But I think it would be handy to have the capability to connect up to 4 probes and a timer to control the Boilermaker and have it able to do an emergency shutdown if a fire is detected in the smoker. I'll get to this eventually, I'm just not exactly sure when that will happen.

Masterbuilt Steam Smoker Settings: 1800 watt 240 volt element running at 120 volts.
- Target Temperature: 107.2C/225.0F
- Startup Power: 90%
- Fallback Power: 80%
- Adjustment Rate: 1%
- Deviation Rate: 0.5C
- Change Wait: 45 seconds
- Rest Period: 60 seconds
- SSR PWM Duty: 2.5 seconds

### The SCR vs SSR Debate

There really is no sense in trying to prove one is better than the other because they both do the same thing, just at different speeds. SCRs run hotter because they are turning the power on and off 60 times per second in the USA _(50 times per second in most other countries)_. This heat is what eventually leads to the failure of an SCR, moreso than overloading one. An SSR will also fail from too much heat because it's really not any different than an SCR when you look at what's inside of one.

The main complaint I see about SSRs is that the cheap Chinese ones are misrepresented, claims that some SSR-40DA units are actually only 15 amps. Even if that's the case, the only way that it will fail is if its triac gets too hot because it'll burn itself open. If you attach it to a large enough heatsink and use a fan to remove heat from it, they'll handle 40 amps just fine. The problem comes from them being attached to one of those tiny heatsinks or none at all, and having no fan cooling. Don't abuse it by overheating it, and it won't burn up.

The only real advantage that I can see that an SCR has is if you need to control the speed of an AC motor or the dimming of incandescent lights. An SSR really isn't intended for that kind of switching speed. The low speed switching of an SSR is perfect for heating elements, on/off switching, and anything else that needs to blink/flash slower than an SCR controller is capable of.
