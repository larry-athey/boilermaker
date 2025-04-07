# About this PCB

This is designed by [FireflySE](https://github.com/FireflySE) of Sweden, the same guy who designed the RPi Hat for the [RPi Smart Still](https://github.com/larry-athey/rpi-smart-still) controller system.

This board supports both, type-k thermocouple or DS18B20 temperature sensors. There is a [new version](https://github.com/larry-athey/boilermaker/tree/main/3D-Models/PCB-Case) of the Boilermaker case for use with this PCB. The origiinal case doesn't provide enough room to fit the PCB between the fan and SSR heatsink.

_**NOTE:** You will need to make one minor modification to my source code to work correctly with this PCB. My original code has the FAN_OUT constant pointing to GPIO 16, this PCB has it on GPIO 2. I have no idea what the reason was for the switch._
