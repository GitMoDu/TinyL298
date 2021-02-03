#include <TinyL298.h>

#ifdef ATTINY_CORE
#define OUTPUT_ENABLE_PIN PB1
#define OUTPUT_A1_PIN PB0
#define OUTPUT_A2_PIN PB4
#else
#pragma Only ATTiny 85 supported.
#endif

// Simulate typical RC PWM period.
#define OUTPUT_UPDATE_PERIOD_MILLIS 20

TinyL298 MotorDriver(OUTPUT_A1_PIN, OUTPUT_A2_PIN);

const uint32_t TestDurationMillis = 10000;
const uint32_t TestPauseMillis = 1000;
uint32_t LastUpdated = 0;
uint16_t TestValue = UINT16_MAX / 2;
uint32_t TestStart = 0;

void setup()
{
	MotorDriver.Begin();
	TestStart = millis();
}

void loop()
{
	if (millis() - LastUpdated > OUTPUT_UPDATE_PERIOD_MILLIS)
	{
		LastUpdated = millis();

		uint32_t Elapsed = millis() - TestStart;

		if (Elapsed > TestDurationMillis)
		{
			MotorDriver.SetValue(UINT16_MAX / 2);
			TestStart = millis();
		}
		if (Elapsed > (TestDurationMillis - TestPauseMillis))
		{
			MotorDriver.SetValue(UINT16_MAX / 2);
		}
		else if (Elapsed <= (TestDurationMillis - TestPauseMillis) / 2)
		{
			uint16_t value = map(Elapsed, 0, (TestDurationMillis - TestPauseMillis) / 2, UINT16_MAX / 2, UINT16_MAX);
			MotorDriver.SetValue(value);
		}
		else if (Elapsed > ((TestDurationMillis - TestPauseMillis) / 2) &&
			Elapsed <= (TestDurationMillis / 2))
		{
			MotorDriver.SetValue(UINT16_MAX / 2);
		}
		else
		{
			uint16_t value = map(Elapsed, (TestDurationMillis - TestPauseMillis) / 2, (TestDurationMillis - TestPauseMillis), UINT16_MAX / 4, 0);
			MotorDriver.SetValue(value);
		}
	}
}