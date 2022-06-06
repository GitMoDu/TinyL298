///  TinyL298.h
/// 
///  Created for personal use, use it at your own risk and benefit.

///  https://github.com/GitMoDu/TinyL298
///  Depends on Fast for IO https://github.com/GitMoDu/Fast
///
///	 L298 H-Bridge driver using an ATtiny85.
///  Responds to 4 distinct states.
///		Brake [UINT16_QUARTER ; 0]
///		Backward [UINT16_QUARTER ; 0]
///		Neutral [UINT16_MIDDLE]
///		Forward [UINT16_MIDDLE ; UINT16_MAX]
///  ~31 KHz carrier for PWM


#ifndef _TINYL298_h
#define _TINYL298_h

#if defined(ATTINY_CORE)
#else
#pragma Only ATTiny 85 supported.
#endif

#include <stdint.h>

// https://github.com/GitMoDu/Fast
#include <Fast.h>


// Chip pin 6 (PB1). Fixed pin, tied to PWM on Timer1.
// PB4 is free for use this way.
#define OUTPUT_ENABLE_PIN PIN_PB1

class TinyL298
{
private:
	FastOut A1;
	FastOut A2;

	const char PWMPin = 'a';
	const uint8_t TimerIndex = 1;
	const uint32_t Frequency = 31500;
	static const uint8_t OutputMax = UINT8_MAX;
	static const uint8_t OutputMin = 0;


	static const uint16_t ReverseBottomTolerance = 100;
	static const uint16_t InputMin = 0;
	static const uint16_t InputMax = UINT16_MAX;
	static const uint16_t InputNeutral = UINT16_MAX / 2;
	static const uint16_t InputBrakeTop = UINT16_MAX / 4;
	static const uint16_t InputReverseBottom = InputBrakeTop + ReverseBottomTolerance;

public:
	TinyL298(const uint8_t pinA1, const uint8_t pinA2)
		: A1(pinA1, LOW)
		, A2(pinA2, LOW)
	{
	}

	void Start()
	{
		A1 = LOW;
		A2 = LOW;

		pinMode(PIN_B1, OUTPUT);

		TCCR1 = 0;
		TIMSK &= ~_BV(TOIE1);
		TCNT1 = 0;
		//TCCR1 = 0b01100010;//16MHZ
		TCCR1 = 0b01100001;//8MHZ
		OCR1C = UINT8_MAX;
		OCR1A = UINT8_MAX / 2;

		TIFR = (1 << TOV1); // Clear timer overflow interrupt flag;
	}

	void Stop()
	{
		A1 = LOW;
		A2 = LOW;

		TCCR1 = 0;

		pinMode(OUTPUT_ENABLE_PIN, OUTPUT);
		digitalWrite(OUTPUT_ENABLE_PIN, LOW);
	}

	void SetValue(const uint16_t value)
	{
		// Auto start;
		if (TCCR1 == 0)
		{
			Start();
		}

		if (value >= InputNeutral)
		{
			// Set forward with duty-cycle. From low to high.
			OCR1A = map(value, InputNeutral, InputMax, OutputMin, OutputMax);
			A1 = HIGH;
			A2 = LOW;
		}
		else if (value >= InputReverseBottom)
		{
			// Set reverse with duty-cycle. From high to low.
			OCR1A = map(value, InputNeutral, InputReverseBottom, OutputMin, OutputMax);
			A1 = LOW;
			A2 = HIGH;
		}
		else if (value >= InputBrakeTop)
		{
			// Set neutral (Off).
			OCR1A = 0;
			A1 = LOW;
			A2 = LOW;
		}
		else
		{
			// Set brake with duty-cycle. From high to low.
			OCR1A = map(value, InputBrakeTop, InputMin, OutputMin, OutputMax);
			A1 = LOW;
			A2 = LOW;
		}
	}
};
#endif