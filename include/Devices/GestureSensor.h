#pragma once

#include <Adafruit_APDS9960.h>

class GestureSensor {
	protected:
		Adafruit_APDS9960 apds;

	public:
		bool Init();
		bool CheckForBoop();
		bool HardCheckForBoop();
		void HardwareTest();
};