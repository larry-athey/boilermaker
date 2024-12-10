# Boilermaker

WiFi enabled and ESP32 driven  power controller and temperature regulator for distillation boilers. Multiple units can be combined in a master/slave configuration.

_**This project began on December 9, 2024 and does not yet have a first official release.**_

This controller will run (1) a PWM controlled SCR board, or (2) a voltage controlled SCR board with a [PWM to voltage convertor module](https://www.amazon.com/dp/B0BG2G5FMX) in the middle, or (3) a [zero-crossing trigger solid state relay](https://www.amazon.com/dp/B08FR13GYR) utilizing a 0.20 Hz PWM _(most reliable method for heating elements)_. The third option is sometimes referred to as a simplified PID controller, but it's actually just working like a PWM SCR controller slowed down to less than 1/1000 the speed.
