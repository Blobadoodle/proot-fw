#include <Devices/Fan.h>
#include <Data/Configuration.h>
#include <Arduino.h>

void Fan::Init(Settings *settings) {
	pinMode(FAN_PIN, OUTPUT);
	SetFanSpeed(settings->data.defaultFanSpeed);
}

// newFanSpeed 0->10
void Fan::SetFanSpeed(uint8_t newFanSpeed) {
	fanSpeed = newFanSpeed;
	analogWrite(FAN_PIN, 255 * ((double)fanSpeed / 10));
}