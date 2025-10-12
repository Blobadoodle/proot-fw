#pragma once

#define FASTLED_RMT_MAX_CHANNELS 1
#include <FastLED.h>
#include <Data/Constants.h>
#include <Chrono.h>
#include <Settings.h>
class RGBLED {
	protected:
        CRGB leftCheek[RGBLED_CHEEK_NUM];
		CRGB rightCheek[RGBLED_CHEEK_NUM];

		Chrono frameTime = Chrono();

		void OnFrame();
		uint8_t outerSpinnerPos = 0;
		uint8_t innerSpinnerPos = 10;
		bool updatedInnerLastFrame = false;

	public:
		void Init(Settings *settings);
		void HardwareTest();
		void Tick();
		void SetBrightness(uint8_t newBrightness);
		uint8_t brightness;
};