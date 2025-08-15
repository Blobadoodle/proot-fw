#pragma once

#include <cstdint>
#include <Settings.h>

class Fan {
	public:
		void Init(Settings *settings);

		uint8_t fanSpeed = 0; // 0->10
		void SetFanSpeed(uint8_t newFanSpeed);
};