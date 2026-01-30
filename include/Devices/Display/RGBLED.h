#pragma once

#include <FastLED.h>
#include <Data/Constants.h>
#include <Chrono.h>
#include <Settings.h>
#include <BLEControl.h>

class RGBLED {
	protected:
        CRGB leftCheek[RGBLED_CHEEK_NUM];
		CRGB rightCheek[RGBLED_CHEEK_NUM];

		Chrono frameTime = Chrono();

		void OnFrame();
		uint8_t outerSpinnerPos = 0;
		uint8_t innerSpinnerPos = 10;
		bool updatedInnerLastFrame = false;

		void OnWriteBrightness(uint8_t newBrightness);

		Settings *settings;
		BLEControl *ble;
	public:
		void Init(Settings *_settings, BLEControl *_ble);
		void HardwareTest();
		void Tick();
		void SetBrightness(uint8_t newBrightness);
		uint8_t brightness;
};