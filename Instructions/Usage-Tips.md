# Usage Tips and Tricks

Although the Boilermaker adds automation to your boiler, it still needs to be tuned to your boiler. This isn't anything complicated and is done by simply heating up water to find the magic numbers that work with your setup. Even though an Air Still and a Still Spirits T500 system both accomplish the same end result, their settings are entirely different. Compare the two below targeted at 80C/176F.

Air Still: 1 gallon, 700 watt 240 volt element running at 120 volts.
- Startup Power: 50%
- Fallback Power: 30%
- Adjustment Rate: 1%
- Deviation Rate: 0.2C
- Change Wait: 30 seconds
- Rest Period: 60 seconds

Still Spirits T500: 6 gallons, 1500 watt 120 volt element running at 120 volts.
- Startup Power: 90%
- Fallback Power: 40%
- Adjustment Rate: 1%
- Deviation Rate: 0.5C
- Change Wait: 30 seconds
- Rest Period: 60 seconds

Once they both do the fallback and stablize, the Air Still will hover between 10% and 13%, the Still Spirits T500 will hover between 18% and 20%. These numbers will slightly vary when you have ethanol in the boiler and depending on your altitude. The Air Still will do its fallback well before hitting 100% power, the Still Spirits T500 will work up to 100% and stay there for about an hour before doing its fallback.