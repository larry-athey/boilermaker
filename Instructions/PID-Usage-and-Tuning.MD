# PID Usage and Tuning

Many people think that a PID controller is the be-all and end-all to temperature and power controllers. This is actually anything but the truth. However, PID controllers have their place and shine in some areas, but pale in others. The only time that a PID controller is of any use in distillation is if you are running at a full boil, but you damn well better have a dephlegmator in your column or you're gonna get a lot of heads and tails, and some really watered down distillate.

If you own an Air Still (or clone) with the Airhead upgrade installed, do a test run in standard Airhead mode (PI mode). Then re-run the distillate again (dump it back in the still) with the unit running in μBoilermaker mode (PID mode) using the same time and temperature settings. You'll be lucky if you get a full shot glass of distillate.

### What is PID? (simple version)

PID is a control method that tries to reach and hold your exact target temperature smoothly:

- **P (Proportional)** — How strongly it reacts to being off-target.
- **I (Integral)** — Corrects small long-term errors (like slowly drifting a bit low).
- **D (Derivative)** — Looks at how fast the temperature is changing and helps prevent overshooting.

In the Boilermaker's **Temperature Cruise** mode (PI mode), the **Change Wait** and **Adjustment Rate** are equal to the **P** parameter and the **Deviation Rate** is equal to the **I** parameter.

### Realistic Expectations

- PID gives very steady temperature — great for mashing, fermenting, sous-vide style control, or electric smokers.
- It is **not** as thermally aggressive as the Boilermaker's PI mode. Once at temperature it will pulse the heater more gently, so you will see significantly slower vapor production in distillation.
- The temperature control becomes more stable the longer it runs because the PID is training itself on the current load.
- Autotune results are a good starting point, but every boiler/setup is different. Feel free to tweak the numbers ±20–50% and test.

### PID Auto Tuning

The first thing you need to do here is be **absolutely** aware of the fact that auto tuning is only meant to be used as an initial point to begin fine tuning from, this is not a magic bullet that saves you from having to learn how PID controllers work.

This procedure is not a fast process and requires your boiler to be completely filled to its full line. This procedure also preheats the boiler to 66C/150F before the tuning begins, so don't run this if you're in a hurry. Lastly, once you start it, you only have 15 seconds to stop it because the web UI will become completely unresponsive until the procedure completes.

You may want to preheat your boiler yourself so you aren't sitting there for an hour or more wondering how far along things are. It also doesn't hurt to preheat the boiler to the temperature that you actually intend to run at. All that matters is that there's ample space between that temperature and the boiling point of water at your altitude.

Once the auto tuning procedure completes, you will see the **Tuning Data** line display a string of text like this.

**Ku=0.102 Dt=160.2 Tau=163.0 → Kp=0.647 Ki=0.003 Kd=0.012 (valid=1)**

The main element that you need to be concerned with is the last one, **valid=1** means that it completed without error. If you see **valid=0** then the tuning failed and your P/I/D settings will not be updated. If any of the first three elements are zero or if the Kp/Ki/Kd elements are outside of the valid ranges allowed by the web UI, it will be flagged as a failure.

If you have an inefficient boiler setup, then you will likely see a lot of failures. This would include under-sized heating elements for the amount of water volume, or a boiler that loses heat too easily. If the Boilermaker is sending power to your heating element and the temperature sensor sees too little change in a given time frame, this will cause the values outlined in the above.
