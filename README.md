# Boilermaker

WiFi enabled and ESP32 driven  power controller and temperature regulator for distillation boilers. Multiple units can be combined in a master/slave configuration.

_**This project began on December 9, 2024 and does not yet have a first official release.**_

You may contact me directly at https://panhandleponics.com<br>
Subscribe to the official YouTube channel at https://www.youtube.com/@PanhandlePonics<br>

---

After dealing with one cheap Chinese SCR power controller after another I decided to combine one person's email suggestion about building a WiFi enabled PID controller and combine it with what I had just finished building for Air Still users. PID controllers have their place, but I think they're better suited for meat smokers, ovens, slow cookers, and incubators. The heating element spends more time off than on, bad for distilling.

This controller will run (1) a PWM controlled SCR board, or (2) a voltage controlled SCR board with a [PWM to voltage convertor module](https://www.amazon.com/dp/B0BG2G5FMX) in the middle, or (3) a [zero-crossing trigger solid state relay](https://www.amazon.com/dp/B08FR13GYR) utilizing a 0.20 Hz PWM _(most reliable method for heating elements)_. The third option is sometimes referred to as a simplified PID controller, but it's actually just working like a PWM SCR controller slowed down to less than 1/1000 the speed.

Heating elements have a slow reaction time, generally 1 second for every 1000 watts. An SCR controller divides your AC power sine wave into anywhere from 1000 to 20000 slices and then determines which back-to-back slices are on and which ones are off. Granted, this "works" for heating elements, but it's wasteful and never lets the heating element run at its full potential unless it's at 100%. Anything less just results in a lot of energy wasted by warming up the SCRs instead of the heating element. Exactly what benefit is a hot SCR providing?

In the low speed 0.20 Hz PWM that I came up with for the Airhead controller and implemented here, even at 50% power, the heating element runs for 1.25 seconds and rests for the same amount of time. The heating element actually runs for the full duration of its reaction time. Whereas with an SCR controller, the heating element only runs for 0.008 seconds 60 times per second. See why the low speed PWM is better?
