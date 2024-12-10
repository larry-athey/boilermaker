# Boilermaker

WiFi enabled and ESP32 driven  power controller and temperature regulator for distillation boilers. Multiple units can be combined in a master/slave configuration.

_**This project began on December 9, 2024 and does not yet have a first official release.**_

This controller will run (1) a PWM controlled SCR board, or (2) a voltage controlled SCR board with a PWM to voltage convertor, or (3) a zero-crossing trigger solid state relay using a 0.20 Hz PWM _(my personal favorite)_.
