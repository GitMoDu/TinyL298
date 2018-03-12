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

#include <Fast.h>  //Fast and simple IO.

#define OUTPUT_ENABLE_PIN PB4 // Chip pin 3. Fixed pin, tied to PWM on Timer1.

#define UINT8_MAXVALUE 255
#define INT8_MAXVALUE 127

#define UINT16_MAXVALUE 65535
#define INT16_MAXVALUE 32767

#define UINT8_MIDDLE INT8_MAXVALUE
#define UINT16_MIDDLE INT16_MAXVALUE

#define UINT16_QUARTER 16383 //(UINT16_MIDDLE/2)

class TinyL298
{
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
		OCR1C = 255;						// Frequency.
	}

	// ||Backward|Brake|Neutral|Forward||
	void SetValue(uint16_t value)
	{
		if (value > UINT16_MIDDLE)
		{
			SetForward(value - UINT16_MIDDLE);
		}
		else if (value == UINT16_MIDDLE)
		{
			SetNeutral();
		}
		else if (value < UINT16_QUARTER)
		{
			SetBackward(UINT16_QUARTER - value);
		}
		else
		{
			SetBrake(UINT16_MIDDLE - value);
		}
	}

private:
	FastOut A1, A2;

private:
	inline void SetNeutral()
	{
		OCR1B = 0;
	}

	inline void SetBrake(uint16_t value)
	{
		OCR1B = map(value, 0, UINT16_QUARTER, 0, UINT8_MAXVALUE);
		A1 = LOW;
		A2 = LOW;
	}

	inline void SetForward(uint16_t value)
	{
		OCR1B = map(value, UINT16_MIDDLE, UINT16_MAXVALUE, 0, UINT8_MAXVALUE);
		A1 = HIGH;
		A2 = LOW;
	}

	inline void SetBackward(uint16_t value)
	{
		OCR1B = map(value, 0, UINT16_QUARTER, 0, UINT8_MAXVALUE);
		A1 = LOW;
		A2 = HIGH;
	}
};
#endif

