#pragma once

#include <cstdint>
#include <Settings.h>
#include <BLEControl.h>

class Fan {
	public:
		void Init(Settings *_settings, BLEControl *_ble);

		uint8_t fanSpeed = 0; // 0->10
		void SetFanSpeed(uint8_t newFanSpeed);
	protected:
		Settings *settings;
		BLEControl *ble;

		void OnWriteSpeed(uint8_t newSpeed);
};