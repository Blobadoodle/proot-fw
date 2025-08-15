#pragma once

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <StateManager.h>
#include <Settings.h>
#include <Chrono.h>
#include <Data/Configuration.h>

class InternalDisplay {
	protected:
		Adafruit_SH1107 display = Adafruit_SH1107(128, 128, &SPI, SH1107_DC, SH1107_RST, SH1107_CS, 8000000UL);
		StateManager *state;
		Settings *settings;

		Chrono batteryUpdateTime;
		uint8_t lastBatteryPercentage = 0;

	public:
		bool Init(StateManager *_state, Settings *_settings);

		void Update(const uint8_t *bitmap, float batteryVoltage);
};