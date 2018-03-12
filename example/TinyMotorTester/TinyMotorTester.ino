#include <TinyL298.h>

#ifdef ATTINY_CORE
#define INPUT_PWM_PIN PB2
#define OUTPUT_A1_PIN PB0
#define OUTPUT_A2_PIN PB1
#else
#pragma Only ATTiny 85 supported.
#endif

#define OUTPUT_UPDATE_PERIOD_MILLIS 20
#define STEP_SIZE 10

TinyL298 MotorDriver(OUTPUT_A1_PIN, OUTPUT_A2_PIN);

uint32_t LastUpdated = 0;
uint16_t TestValue = UINT16_MIDDLE;

void setup()
{
	MotorDriver.Begin();
}

void loop()
{
	if (millis() - LastUpdated > OUTPUT_UPDATE_PERIOD_MILLIS)
	{
		LastUpdated = millis();
		MotorDriver.SetValue(TestValue);
		TestValue += STEP_SIZE;
	}
}