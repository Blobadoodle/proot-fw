#pragma once

#include <cstdint>
#include <Wire.h>

class Battery {
	public:
        void Init();
    
		float GetVoltage();
        uint8_t GetPercentage();
        void HardwareTest();
};