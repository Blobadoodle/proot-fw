#pragma once

#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>
#include <Data/Configuration.h>
#include <Data/Constants.h>
#include <Settings.h>
#include <BLEControl.h>

class LEDMatrix {
	protected:
		Max72xxPanel matrix = Max72xxPanel(MATRIX_CS, NUM_OF_MATRICES, 1);
		Settings *settings;
		BLEControl *ble;

		void DrawBitmapFlipped(const uint8_t *srcBitmap, uint16_t xOffset, uint16_t width);
	public:
		void Init(Settings *_settings, BLEControl *_ble);

		void SetBrightness(uint8_t newBrightness);
		uint8_t brightness;

		void Update(const uint8_t *bitmap);

		void HardwareTest();

		void OnWriteIntensity(uint8_t newIntensity);
};