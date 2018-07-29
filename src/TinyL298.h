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

#if defined (ARDUINO) || defined(ATTINY_CORE)
#include <Arduino.h>
#else
#pragma Only ATmega328 and ATTiny 85 supported.
#endif

#include <stdint.h>
#include <Fast.h>  //Fast and simple IO.

#define OUTPUT_ENABLE_PIN PB4 // Chip pin 3. Fixed pin, tied to PWM on Timer1.


#define UINT16_MIDDLE	((uint16_t)32767)
#define UINT16_QUARTER	((uint16_t)16383) 

#define NEUTRAL_RANGE	((uint16_t)500)

#define BRAKE_BOTTOM	(UINT16_QUARTER-NEUTRAL_RANGE)
#define NEUTRAL_TOP		(UINT16_MIDDLE+NEUTRAL_RANGE)
#define NEUTRAL_BOTTOM	(UINT16_MIDDLE-NEUTRAL_RANGE)

class TinyL298
{
private:
	FastOut A1, A2;

public:
	TinyL298(const uint8_t pinA1, const uint8_t pinA2)
		: A1(pinA1)
		, A2(pinA2)
	{
		pinMode(OUTPUT_ENABLE_PIN, OUTPUT);
		Begin();
	}

	void Begin()
	{
		//Thank you Gammon. http://www.gammon.com.au/timers
		TCCR1 = bit(CS10);					// No prescaler.
		GTCCR = bit(COM1B1) | bit(PWM1B);   // Clear OC1B on compare.
		OCR1B = 0;							// Duty cycle.
		OCR1C = UINT8_MAX;					// Frequency.
	}

	// ||Backward|Brake|Neutral|Forward||
	void SetValue(uint16_t value)
	{
		if (value > NEUTRAL_TOP)
		{
			//Set forward with duty-cycle.
			OCR1B = map(value, NEUTRAL_TOP, UINT16_MAX, 0, UINT8_MAX);
			A1 = HIGH;
			A2 = LOW;
		}
		else if (value <= NEUTRAL_TOP && value >= NEUTRAL_BOTTOM)
		{
			//Set neutral.
			OCR1B = 0;
		}
		else if(value < NEUTRAL_BOTTOM && value > BRAKE_BOTTOM)
		{
			//Set brake with duty-cycle.
			OCR1B = map(value, NEUTRAL_BOTTOM, BRAKE_BOTTOM, 0, UINT8_MAX);
			A1 = LOW;
			A2 = LOW;
		} 
		else
		{
			//Set reverse with duty-cycle.
			OCR1B = map(value, BRAKE_BOTTOM, 0, 0, UINT8_MAX);
			A1 = LOW;
			A2 = HIGH;
		}
	}

	inline void SetNeutral()
	{

	}

	inline void SetBrake(uint16_t value)
	{

	}

	inline void SetForward(uint16_t value)
	{

	}

	inline void SetBackward(uint16_t value)
	{
		
	}
};
#endif

