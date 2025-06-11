#include <Arduino.h>
#include <WString.h> // Needed for EEPROM.h
#include <EEPROM.h>
#include "Settings.h"

bool Settings::CheckMagic() { // Returns true/fales if the magic is or isn't correct
	return memcmp(data.header.magic, SETTINGS_MAGIC, sizeof(SETTINGS_MAGIC)) == 0;
}

bool Settings::CheckRevision() {
	return data.header.revision == LATEST_SETTINGS_REVISION;	
}

void Settings::GetSettings() { // Loads the settings from the EEPROM into this.data
	EEPROM.get(0, data);
}

bool Settings::WriteSettings() {
	if(!CheckMagic()) {
		return false;
	}

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

bool Settings::Init() { // Loads settings from EEPROM, or initialises EEPROM with empty settings if EEPROM is empty
	EEPROM.begin(sizeof(SettingsData));

	GetSettings();

	if(!CheckMagic() || !CheckRevision()) {
		Serial.println("Invalid or out of date settings data found in EEPROM. Initialising...");

		if(!ResetSettings()) {
			Serial.println("Failed to initialise EEPROM. Using defaults.");
			return false;
		}
	}

	return true;
}