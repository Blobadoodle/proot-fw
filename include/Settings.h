#pragma once

#include <cstdint>
#include <cstring>
#include <FastCRC.h>
#include <Data/Constants.h>
#include <Data/Expressions.h>

struct SettingsHeader {
	char magic[7];
	uint8_t revision;
	uint32_t crc;

	SettingsHeader() {
		memcpy(magic, SETTINGS_MAGIC, 7 * sizeof(char));
		revision = LATEST_SETTINGS_REVISION;
		crc = 0;
	}
};

struct SettingsData {
	struct SettingsHeader header;
	uint8_t defaultRgbBrightness;
	uint8_t defaultBrightness;
	uint8_t defaultFanSpeed;
	bool micToggle;
	
	SettingsData() { // Default values
		header = SettingsHeader();
		defaultRgbBrightness = 1;
		defaultBrightness = 1;
		defaultFanSpeed = 0;
		micToggle = true;
	}
};

class Settings {
	private:
		bool CheckMagic();
		bool CheckRevision();
		FastCRC32 CRC32;

	public:
		bool Init();
		void GetSettings();
		bool WriteSettings();
		bool ResetSettings();
		SettingsData data;
};