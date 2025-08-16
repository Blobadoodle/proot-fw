#pragma once

#define FASTLED_RMT_MAX_CHANNELS 1
#include <FastLED.h>
#include <Data/Constants.h>

class RGBLED {
	protected:
        CRGB leftCheek[RGBLED_CHEEK_NUM];
		CRGB rightCheek[RGBLED_CHEEK_NUM];

	public:
		void Init();
		void HardwareTest();
};