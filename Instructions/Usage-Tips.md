# Boilermaker Usage Tips

Although the Boilermaker adds temperature automation to your boiler, it still needs to be tuned to your boiler. This isn't anything complicated and is done by simply heating up water to find the magic numbers that work with your setup. Even though an Air Still and a Still Spirits T500 system both accomplish the same end result, their settings are entirely different. Compare the two below targeted at 80C/176F.

Air Still: 1 gallon, 700 watt 240 volt element running at 120 volts.
- Startup Power: 50%
- Fallback Power: 30%
- Adjustment Rate: 1%
- Deviation Rate: 0.2C
- Change Wait: 30 seconds
- Rest Period: 60 seconds
- SSR PWM Duty: 2.5 seconds

Still Spirits T500: 6 gallons, 1500 watt 120 volt element running at 120 volts.
- Startup Power: 90%
- Fallback Power: 40%
- Adjustment Rate: 1%
- Deviation Rate: 0.5C
- Change Wait: 30 seconds
- Rest Period: 60 seconds
- SSR PWM Duty: 1 second

Once they both do the fallback and stablize, the Air Still will hover between 10% and 13%, the Still Spirits T500 will hover between 18% and 20%. Yes, they will actually work downward when targeting a temperature below the liquid's boiling point. These numbers will slightly vary when you have ethanol in the boiler and depending on your altitude. The Air Still will do its fallback well before hitting 100% power, the Still Spirits T500 will work up to 100% and stay there for about an hour before doing its fallback.

If you have a well insulated boiler or a jacketed boiler, you shouldn't be surprised to see the Boilermaker hover at 10% and stay there. The power level will never drop below 10% because this constant amount of at least some heat rising up through the boiler helps prevent suspended solids from falling out of solution and scorching to the bottom or your heating element. This was the primary problem that I was intending to cure when I came up with the [Airhead](https://github.com/larry-athey/airhead) project.

When using the Boilermaker with the [RPi Smart Still](https://github.com/larry-athey/rpi-smart-still) you can duplicate the functionality of the Airhead's operating Mode 3 _(progressive temperature over time)_. Once your enable Boilermaker support, go to **Management->Configure Heating**, then set the **Fixed Temperature** drop-down to **No**, and set the **Time Spread** value to however many hours you expect your run to take. This is how long the run takes **after** your boiler has reached the minimum operating temperature.

The way this works is that the boiler temperature range of your program is divided up so there are 4 equal temperature jumps per hour. If you set the **Time Spread** to 3 hours, then this temperature range is divided by 12 and every 15 minutes that factor is added to the target temperature. You can see this change in the dashboard and the Boilermaker WebUI.

_**NOTE:** While the progressive temperature over time is a standard feature in the [Airhead](https://github.com/larry-athey/airhead) controller, it has not been added to the Boilermaker since most home distillers would never use it and it would just chew up valuable ESP32 code space. This can be easily accomplished with a looping script running on any computer on the same network as the Boilermaker if a person really wants it. I've found that most home distillers are more hands-on oriented, especially if they put a lot of time and money into building a big fancy still full of fine-tuning features._
