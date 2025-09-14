#pragma once

#define FASTLED_RMT_MAX_CHANNELS 1
#include <FastLED.h>
#include <Data/Constants.h>
#include <Chrono.h>

class RGBLED {
	protected:
        CRGB leftCheek[RGBLED_CHEEK_NUM];
		CRGB rightCheek[RGBLED_CHEEK_NUM];

		Chrono frameTime = Chrono();

		void OnFrame();
		uint8_t spinnerPos = 0;

	public:
		void Init();
		void HardwareTest();
		void Tick();
};