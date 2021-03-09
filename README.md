# TinyL298 Driver

L298 H-Bridge motor driver using an ATtiny85 @ 8 MHz using OptiBoot.


##

![](https://raw.githubusercontent.com/GitMoDu/TinyL298/master/media/L298NESC.jpg)

## Motor Driver:
  * PWM frequency 31250 Hz
  * Input Servo Range ~10 bit -> [1000; 2020] us 
  * Boot/Wake up time [1-2] ms
  * Update latency: ~250us after servo input falling edge
  * Responds to 3 scalable states, plus neutral:

![](https://raw.githubusercontent.com/GitMoDu/TinyL298/master/media/InputRange.jpg)

Reverse scales negatively, around neutral, for tolerance on normal input. There's a small tolerance to prevent mixing the reverse max with brake min.

## Dependencies:
  * Fast IO https://github.com/GitMoDu/Fast



Created for personal use, use it at your own risk and benefit.
