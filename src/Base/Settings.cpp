#include <Arduino.h>
#include <WString.h> // Needed for EEPROM.h
#include <EEPROM.h>
#include <Settings.h>
#include <stddef.h>

bool Settings::CheckMagic() { // Returns true/fales if the magic is or isn't correct
	return memcmp(data.header.magic, SETTINGS_MAGIC, sizeof(SETTINGS_MAGIC)) == 0;
}

bool Settings::CheckRevision() {
	return data.header.revision == LATEST_SETTINGS_REVISION;	
}

void Settings::GetSettings() { // Loads the settings from the EEPROM into this.data
	EEPROM.get(0, data);
	ble->SetValue(BLE_DISPLAY_BRIGHTNESS, &data.defaultBrightness, sizeof(data.defaultBrightness));
	ble->SetValue(BLE_RGB_BRIGHTNESS, &data.defaultRgbBrightness, sizeof(data.defaultBrightness));
	ble->SetValue(BLE_FAN_SPEED, &data.defaultFanSpeed, sizeof(data.defaultBrightness));
	ble->SetValue(BLE_MIC_TOGGLE, (uint8_t*)&data.micToggle, sizeof(data.micToggle));
	ble->SetValue(BLE_BOOP_COUNTER, (uint8_t*)&data.boopCounter, sizeof(data.boopCounter));
}

bool Settings::WriteSettings() {
	if(!CheckMagic())
		return false;

	// TODO: CRC32

	EEPROM.put(0, data);
	EEPROM.commit();

	// Sanity check if data is being written correctly
	GetSettings();
	return CheckMagic();
}

bool Settings::ResetSettings() {
	data = SettingsData();
	return WriteSettings();
}

void Settings::IncrementBoopCounter() {
	data.boopCounter++;
	
	EEPROM.put(offsetof(SettingsData, boopCounter), data.boopCounter);
	EEPROM.commit();

	ble->SetValue(BLE_BOOP_COUNTER, (uint8_t*)&data.boopCounter, sizeof(data.boopCounter));
	ble->Notify(BLE_BOOP_COUNTER);
}

bool Settings::Init(BLEControl *_ble) { // Loads settings from EEPROM, or initialises EEPROM with empty settings if EEPROM is empty
	ble = _ble;

	EEPROM.begin(sizeof(SettingsData));

	GetSettings();

	if(!CheckMagic() || !CheckRevision()) {
		Serial.println("Invalid or out of date settings data found in EEPROM. Initialising...");

		if(!ResetSettings()) {
			Serial.println("Failed to initialise EEPROM. Using defaults.");
			return false;
		}
	}

	ble->SetWriteCallback(BLE_WRITE_SETTINGS, [this](NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo) {
		WriteSettings();
	});

	return true;
}