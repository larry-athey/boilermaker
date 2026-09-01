# PID Usage and Tuning


**What is PID? (simple version)**
PID is a control method that tries to reach and hold your exact target temperature smoothly:

- **P (Proportional)** — How strongly it reacts to being off-target.
- **I (Integral)** — Corrects small long-term errors (like slowly drifting a bit low).
- **D (Derivative)** — Looks at how fast the temperature is changing and helps prevent overshooting.

In the Temperature Cruise mode, the **Change Wait** and **Adjustment Rate** are equal to the **P** parameter and the **Deviation Rate** is equal to the **I** parameter.

**Realistic expectations:**

- PID gives very steady temperature — great for mashing, fermenting, sous-vide style control, or electric smokers.
- It is **not** as thermally aggressive as the normal Airhead PI mode. Once at temperature it will pulse the heater more gently, so you will see significantly slower vapor production in distillation.
- The temperature control becomes more stable the longer it runs because the PID is training itself on the current load.
- Autotune results are a good starting point, but every boiler/setup is different. Feel free to tweak the numbers ±20–50% and test.
