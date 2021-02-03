# TinyL298 Driver

L298 H-Bridge motor driver using an ATtiny85.

##

![](https://raw.githubusercontent.com/GitMoDu/TinyL298/master/media/L298NESC.jpg)

Responds to 3 scalable states, plus neutral:

![](https://raw.githubusercontent.com/GitMoDu/TinyL298/master/media/InputRange.jpg)

Reverse scales negatively, around neutral, for tolerance on normal input. There's a small tolerance to prevent mixing the reverse max with brake min.

## Dependencies:
  * Fast IO https://github.com/GitMoDu/Fast
  * PWM https://github.com/micooke/PWM



Created for personal use, use it at your own risk and benefit.
