///  TinyL298.h
/// 
///  Created for personal use, use it at your own risk and benefit.
///  https://github.com/GitMoDu/TinyL298
///  Depends on Fast for IO https://github.com/GitMoDu/Fast
///
///	 L298 H-Bridge driver using an ATtiny85.
///  Responds to 4 distinct states, each scalable.
///  ||Backward|Brake|Neutral|Forward||
/// 


#ifndef _TINYL298_h
#define _TINYL298_h

#if defined(ATTINY_CORE)
#else
#pragma Only ATTiny 85 supported.
#endif

#include <stdint.h>
#include <Fast.h>  //Fast and simple IO.

#include <PWM.h>

// Chip pin 6 (PB1). Fixed pin, tied to PWM on Timer1.
// PB4 is free for use this way.
#define OUTPUT_ENABLE_PIN PIN_B4 

#define UINT16_MIDDLE	((uint16_t)32767)
#define UINT16_QUARTER	((uint16_t)16383) 

#define NEUTRAL_RANGE	((uint16_t)500)

#define BRAKE_BOTTOM	(UINT16_QUARTER-NEUTRAL_RANGE)
#define NEUTRAL_TOP		(UINT16_MIDDLE+NEUTRAL_RANGE)
#define NEUTRAL_BOTTOM	(UINT16_MIDDLE-NEUTRAL_RANGE)

class TinyL298
{
private:
	FastOut A1;
	FastOut A2;

	const char PWMPin = 'a';
	const uint8_t TimerIndex = 1;
	const uint32_t Frequency = 31250;

public:
	TinyL298(const uint8_t pinA1, const uint8_t pinA2)
		: A1(pinA1)
		, A2(pinA2)
	{
		A1 = LOW;
		A2 = LOW;
	}

	void Begin()
	{
		A1 = LOW;
		A2 = LOW;

		pwm.set(TimerIndex, PWMPin, Frequency, 2);
		pwm.set_register(TimerIndex, PWMPin, 0);
		pwm.start(TimerIndex);
	}

	// ||Backward|Brake|Neutral|Forward||
	void SetValue(const uint16_t value)
	{
		if (value >= NEUTRAL_TOP)
		{
			// Set forward with duty-cycle.
			pwm.set_register(TimerIndex, PWMPin, map(value, NEUTRAL_TOP, UINT16_MAX, 0, UINT8_MAX));
			A1 = HIGH;
			A2 = LOW;
		}
		else if (value < NEUTRAL_TOP && value > NEUTRAL_BOTTOM)
		{
			// Set neutral (Off).
			pwm.set_register(TimerIndex, PWMPin, 0);
			A1 = LOW;
			A2 = LOW;
		}
		else if (value <= NEUTRAL_BOTTOM && value >= BRAKE_BOTTOM)
		{
			// Set brake with duty-cycle.
			pwm.set_register(TimerIndex, PWMPin, map(value, NEUTRAL_BOTTOM, BRAKE_BOTTOM, 0, UINT8_MAX));
			A1 = LOW;
			A2 = LOW;
		}
		else
		{
			// Set reverse with duty-cycle.
			pwm.set_register(TimerIndex, PWMPin, map(value, BRAKE_BOTTOM - 1, 0, 0, UINT8_MAX));
			A1 = LOW;
			A2 = HIGH;
		}
	}
};
#endif