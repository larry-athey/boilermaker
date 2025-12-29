# Boilermaker Usage Tips

_**NOTE:** Heating any boiler from the bottom-up often results in non-uniform temperature distribution—it's rarely linear across all levels due to how heat transfers in liquids. No controller can overcome fundamental physics or poor setup. To get the best results, a basic understanding of thermodynamics and fluid dynamics will help you set realistic expectations. Even advanced tools only work correctly when used with care._

Although the Boilermaker adds temperature control to your boiler using both the **Temperature Cruise** and **Brewing/Fermentation** operating modes, it still needs to be tuned to your boiler. This isn't anything complicated and is done by simply heating up water to find the magic numbers that work with your setup. Even though an Air Still and a Still Spirits T500 system both accomplish the same end result, their settings are quite different. Compare the two below targeted at 80C/176F.

By default, the low speed PWM is set to a 2.5 second duty time. You will need to adjust this to match your heating element(s). You can find this setting near the bottom of the page when the **Brewing/Fermentation** mode is selected. The more efficient your heating element(s), the lower the duty time should be. The range is adjustable from 1 to 5 seconds. As you can see in the examples below, the Air Still's 700 watt element is very inefficient and requires a longer PWM duty time.

The examples below use the **Temperature Cruise** mode. To understand this mode, look at it like an old analog cruise control in a car while traveling down a road with a lot of hills. As you go down a hill, the cruise control will ease up on the gas pedal and may actually ease up on it completely. Even in that case, your engine is still running and still providing some forward power. The cruise control only relies on inertia loss to slow you down, no imposed resistance beyond the weight of the vehicle going up the next hill.

Air Still: 1 gallon, 700 watt 120 volt element running at 120 volts.
- Operation Mode: Temperature Cruise
- Target Temperature: 80.0C/176.0F
- Startup Power: 50%
- Fallback Power: 33%
- Adjustment Rate: 1%
- Deviation Rate: 0.2C
- Change Wait: 30 seconds
- Rest Period: 60 seconds
- SSR PWM Duty: 2.5 seconds

Still Spirits T500: 6 gallons, 1500 watt 120 volt element running at 120 volts.
- Operation Mode: Temperature Cruise
- Target Temperature: 80.0C/176.0F
- Startup Power: 75%
- Fallback Power: 25%
- Adjustment Rate: 1%
- Deviation Rate: 0.5C
- Change Wait: 30 seconds
- Rest Period: 60 seconds
- SSR PWM Duty: 1 second

Once they both do the fallback and stabilize, the Air Still will hover between 10% and 13%, the Still Spirits T500 will hover between 18% and 20%. Yes, they will actually work downward when targeting a temperature below the liquid's boiling point. These numbers will slightly vary when you have ethanol in the boiler and depending on your altitude. The Air Still will do its fallback well before hitting 100% power, the Still Spirits T500 will work up to 100% and stay there for about an hour before doing its fallback.

If you have a well insulated boiler or a jacketed boiler, you shouldn't be surprised to see the Boilermaker hover at less than 10%. The power level will never drop below 1% because this constant amount of at least some heat rising up through the boiler helps prevent suspended solids from falling out of solution and scorching to the bottom or your heating element. This was the primary problem that I was intending to cure when I came up with the [Airhead](https://github.com/larry-athey/airhead) project.

If you find that your boiler tends to run hot after the fallback and the Boilermaker always bottoms out to 1% power, reduce the PWM duty time. If you find that the boiler just takes forever to stabilize the temperature after fallback, increase the PWM duty time. 

## Using Your Boilermaker For Brewing and Fermentation

Some people ferment on the grain and some also distill on the grain if they have a boiler with a motorized paddle. Your Boilermaker can actually maintain fermentation temperatures by using the **Brewing/Fermentation** operating mode. This turns the Boilermaker into an actual PID controller with very strict tuning, which means that this is much slower than the **Temperature Cruise** mode.

_**NOTE:** This document isn't meant to be a tutorial on PID controllers, I recommend that you follow YouTube tutorial videos that explain them._

Keep in mind that you have a lot of suspended solids in brewing and fermentation. A PID controller may quickly work up to 100% power depending on the current and target temperatures. This will almost always result in scorching. It's recommended that you first warm up your mash/wort in **Temperature Cruise** mode at a 25% Startup Power, 1-2 minute Change Wait, and a 1% Fallback Power. You would then switch to **Brewing/Fermentation** mode after the fallback. This guarantees no scorching and no over-shoot.

_**Remember,** the liquid at the bottom of your boiler is hotter than the top any time that the heating element is running. You may want to under-shoot your target temperature and allow stratification to happen before you increase your target temperature. For example, if your goal is 82.2C/180F, set your target temperature to 80C/176F and wait for stratificacion to rise to your goal, then increase your target temperature._

The placement of your temperature sensor also makes a major difference, mid-height of the boiler vessel is the best location. Too close to the bottom and the system will think that it's up to temperature before anything above it is. Too close to the top and you will likely end up scorching stuff at the bottom. Circulation pumps or motorized mixing paddles are an absolute necessity when you have a lot of solids.

Unlike some units on the market, this PID controller does not train itself and store any profile data in flash RAM. Every new run causes it to train itself from scratch. If you were to train the PID controller on plain water and then try to use that saved profile with a beer wort, you would end up scorching the suspended solids to your heating element.

_**PRO-TIP:** The **Cruise Then Brew** operation mode performs the above mode shift procedure automatically when the target temperature is reached. You can also use the progressive temperature function to automatically raise the target temperature up to your goal over 1 hour._

## Controlling An Electric Smoker

Aside from controlling boilers, another great use for the Boilermaker is turning any absolutely cheap bottom of the line electric smoker into a WiFi enabled **reliable and accurate** electric smoker. My first digital smoker got taken out by a lightning strike and I replaced it with a cheap [Masterbuilt steam smoker](https://www.amazon.com/gp/product/B07VRJQXGL/) and a $25 PID controller. Let's just say that PID controllers have their place, but they're really not the best option for a smoker since the heating element spends more time off than on.

With a PID controller, the heating element barely stays on long enough to get your wood chips smoldering. Whereas with the Boilermaker in **Temperature Cruise** mode, the power level usually never drops below 1%, so there is always some amount of heat coming from the heating element. Even with the default 2.5 second PWM duty time, 50% power means the heating element runs for 1.25 seconds before resting for 1.25 seconds, which is actually plenty enough heat to cause the wood chips to start smoldering.

Since a smoker can reach much higher temperatures than a boiler, you will might want to use a Type-K thermocouple rather than a DS18B20 temperature sensor or you'll never know when it gets above 125/257F in the smoker. Generally, this wouldn't be a problem with a smoker since people usually slow cook things between 107C/225F and 121C/250F. However, even I have had the random fire flare up in the smoker because grease dripped onto the red hot heating element.

I do have plans for making a WiFi enabled ESP32 powered temperature probe and timer unit for the Boilermaker and my [CZ Pellet Stove Controller](https://github.com/larry-athey/climate-czar/tree/main/CZ_Pellet_Stove_Controller_v2) with multiple temperature probes. I just have a few other programming projects that I need to tie up first. But I think it would be handy to have the capability to connect up to 4 probes and a timer to control the things and have it able to do an emergency shutdown if a fire is detected in the smoker. I'll get to this eventually, I'm just not exactly sure when that will happen.

Masterbuilt Steam Smoker Settings: 1800 watt 120 volt element running at 120 volts.
- Operation Mode: Temperature Cruise
- Target Temperature: 107.2C/225.0F
- Startup Power: 90%
- Fallback Power: 80%
- Adjustment Rate: 1%
- Deviation Rate: 0.5C
- Change Wait: 45 seconds
- Rest Period: 60 seconds
- SSR PWM Duty: 2.5 seconds

## The SCR vs SSR Debate

There really is no sense in trying to prove one is better than the other because they both do the same thing, just at different speeds. SCRs run hotter because they are turning the power on and off 120 times per second in the USA _(100 times per second in most other countries)_. This heat is what eventually leads to the failure of an SCR, moreso than overloading one. An SSR will also fail from too much heat because it's really not any different than an SCR when you look at what's inside of one.

The main complaint I see about SSRs is that the cheap Chinese ones are misrepresented, claims that some SSR-40DA units are actually only 15 amps. Even if that's the case, the only way that it will fail is if its triac gets too hot because it'll burn itself open. If you attach it to a large enough heatsink and use a fan to remove heat from it, they'll handle 40 amps just fine. The problem comes from them being attached to one of those tiny heatsinks or none at all, and having no fan cooling. Don't abuse it by overheating it, and it won't burn up.

The only real advantage that I can see that an SCR has is if you need to control the speed of an AC motor or the dimming of incandescent lights. An SSR really isn't intended for that kind of switching speed. The low speed switching of an SSR is perfect for heating elements, on/off switching, and anything else that needs to blink/flash slower than an SCR controller is capable of.
