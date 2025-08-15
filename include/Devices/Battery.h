#pragma once

#include <cstdint>
#include <UMS3.h>

class Battery {
	public:
        void Init(UMS3 *ums3);
    
		float GetVoltage();
        uint8_t GetPercentage();
        void HardwareTest();
    protected:
        UMS3 *ums3;
};