# Boilermaker

WiFi enabled and ESP32 driven  power controller and temperature regulator for distillation boilers. Multiple units can be combined in a master/slave configuration.

_**This project began on December 9, 2024 and does not yet have a first official release.**_

You may contact me directly at https://panhandleponics.com<br>
Subscribe to the official YouTube channel at https://www.youtube.com/@PanhandlePonics

**Facebook Idiot:** _Why would you bother?_<br>
**Me:** It's only a "bother" for somebody who couldn't do it on their best day. I'd rather have nicer things.

---

<img width="1024" src="https://github.com/user-attachments/assets/28a96f4a-670e-46cb-8502-e2bf2e2e060c"><br>
---

After dealing with one cheap Chinese SCR power controller after another I decided to combine one person's email suggestion about building a WiFi enabled PID controller and combine it with what I had just finished building for Air Still users. PID controllers have their place, but I think they're better suited for 3D printers, fermenters, ovens, slow cookers, and incubators. The heating element spends more time off than on, bad for distilling.

This controller will run (1) a PWM controlled SCR board, or (2) a voltage controlled SCR board with a [PWM to voltage convertor module](https://www.amazon.com/dp/B0BG2G5FMX) in the middle, or (3) a [zero-crossing trigger solid state relay](https://www.amazon.com/dp/B08FR13GYR) utilizing my custom low speed PWM _(most effective method for heating elements)_. My invention works like a PWM SCR controller slowed down to less than 1/1000 the speed and runs heating elements much more effectively.

Heating elements have a slow reaction time, generally 1 second for every 1000 watts. An SCR controller divides your AC power sine wave into anywhere from 1000 to 20000 slices and then determines which back-to-back slices are on and which ones are off. Granted, this "works" for heating elements, but it's wasteful and never lets the heating element run at its full potential unless it's at 100%. Anything less just results in a lot of energy wasted by warming up the SCRs instead of the heating element. Exactly what benefit are hot SCRs providing?

In the low speed 0.20 Hz PWM that I came up with for the [Airhead](https://github.com/larry-athey/airhead) controller and implemented here, even at 50% power, the heating element runs for 1.25 seconds and rests for 1.25 seconds. Most heating elements run for the full duration of their reaction time. Whereas when using an SCR controller, all heating elements only run for 0.008 seconds 60 times per second. See why the low speed PWM running an SSR is better?

This controller uses either a DS18B20 temperature sensor or a thermocouple and a MAX-6675/MAX-31855 amplifier module. If your intention is to only use this controller with a distillation boiler or a meat smoker, then the DS18B20 is perfectly fine since it has an upper temperature limit of 125C/257F. If you need a higher temperature range, you will want to comment out the DS18B20 constant and use a thermocouple and MAX-6675/MAX-31855 amplifier combo instead.

This controller also features a complete Web API for configuration and control, this is how one master unit controls up to 4 slaves. Each of those slaves can also have up to 4 slaves _(look at it like an Amway pyramid scheme)_. This means that these controllers can be added to all kinds of systems out there that offer API integration of third party devices and services.

The [RPi Smart Still](https://github.com/larry-athey/rpi-smart-still) system fully supports the Boilermaker and features progressive temperature output just like you see in Mode 3 of the [Airhead](https://github.com/larry-athey/airhead) controller. This is not a standard feature of Boilermaker since the vast majority of distillers would never use it. You could get close just by running it in constant power mode which will result in an imprecise progressive temperature rise like you'd have with a normal SCR controller.

_**NOTE:** The Boilermaker is also handy for turning any cheap electric smoker into one that's a whole lot more reliable and accurate, see the [usage tips](https://github.com/larry-athey/boilermaker/blob/main/Instructions/Usage-Tips.md) for more information._

<img width="1024" src="https://github.com/user-attachments/assets/6303cf1b-7fc9-481e-a0dc-133c85d81b82">
