# Boilermaker

WiFi enabled and ESP32 driven  power controller and temperature regulator for distillation boilers. Multiple units can be combined in a master/slave configuration.

_**This project began on December 9, 2024 and does not yet have a first official release.**_

This controller will run (1) a PWM controlled SCR board, or (2) a voltage controlled SCR board with a [PWM to voltage convertor module](https://www.amazon.com/dp/B0BG2G5FMX) in the middle, or (3) a [zero-crossing trigger solid state relay](https://www.amazon.com/dp/B08FR13GYR) utilizing a 0.20 Hz PWM _(most reliable method for heating elements)_. The third option is sometimes referred to as a simplified PID controller, but it's actually just working like a PWM SCR controller slowed down to less than 1/1000 the speed.

Heating elements have a slow reaction time, generally 1 second for every 1000 watts. An SCR controller divides your AC power sine wave into anywhere from 1000 to 20000 slices and then determines which back-to-back slices are on and which ones are off. Granted, this "works" for heating elements, but it's wasteful and never lets the heating element run at its full potential unless it's at 100%. Anything less just results in a lot of energy wasted by warming up the SCRs instead of the heating element. Exactly what benefit is a hot SCR providing for you?
