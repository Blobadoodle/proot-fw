#include <Devices/Battery.h>
#include <Data/Configuration.h>
#include <Data/Constants.h>
#include <Arduino.h>

// https://github.com/rlogiacco/BatterySense/blob/master/Battery.h line 101
static inline uint8_t sigmoidal(uint16_t voltage, uint16_t minVoltage, uint16_t maxVoltage) {
#if defined(BATTERY_SIGMOID_SLOW)
	uint8_t result = 110 - (110 / (1 + pow(1.468 * (voltage - minVoltage)/(maxVoltage - minVoltage), 6)));
#elif defined(BATTERY_SIGMOID_STEEP)
	uint8_t result = 102 - (102 / (1 + pow(1.621 * (voltage - minVoltage)/(maxVoltage - minVoltage), 8.1)));
#else // normal
	uint8_t result = 105 - (105 / (1 + pow(1.724 * (voltage - minVoltage)/(maxVoltage - minVoltage), 5.5)));
#endif
	return result >= 100 ? 100 : result;
}


void Battery::Init() {
	pinMode(VBAT, INPUT);
}

// https://github.com/UnexpectedMaker/esp32s3-arduino-helper/blob/main/src/UMS3.h line 210
float Battery::GetVoltage() {
	uint32_t millivolts = analogReadMilliVolts(VBAT);

	const uint32_t upper_divider = 442;
	const uint32_t lower_divider = 160;
	float voltage = (float)(upper_divider + lower_divider) / lower_divider / 1000 * millivolts;

	return voltage;
}

uint8_t Battery::GetPercentage() {
	float voltage = GetVoltage() * 1000;
	if(voltage <= BATTERY_MIN_MV)
		return 0;
	else if(voltage >= BATTERY_MAX_MV)
		return 100;
	else
		return sigmoidal(voltage, BATTERY_MIN_MV, BATTERY_MAX_MV);
}

void Battery::HardwareTest() {
	Serial.printf("Battery voltage: %.2fV\n", GetVoltage());
	Serial.printf("Battery percentage: %d%%\n", GetPercentage());
}