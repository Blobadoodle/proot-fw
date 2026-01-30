#include <Devices/Fan.h>
#include <Data/Configuration.h>
#include <Arduino.h>

void Fan::Init(Settings *_settings, BLEControl *_ble) {
	settings = _settings;
	ble = _ble;

	pinMode(FAN_PIN, OUTPUT);
	SetFanSpeed(settings->data.defaultFanSpeed);

	ble->SetWriteCallback(BLE_FAN_SPEED_CHAR, [this](NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo) {
		OnWriteSpeed(pCharacteristic->getValue().data()[0]);
	});
}

void Fan::OnWriteSpeed(uint8_t newSpeed) {
	uint8_t actualNew = constrain(newSpeed, 0, 10);
	SetFanSpeed(actualNew);

	settings->data.defaultFanSpeed = actualNew;
	settings->WriteSettings();
}

// newFanSpeed 0->10
void Fan::SetFanSpeed(uint8_t newFanSpeed) {
	fanSpeed = newFanSpeed;
	analogWrite(FAN_PIN, 255 * ((double)fanSpeed / 10));
}