#include <Arduino.h>
#include "LEDMatrix.h"
#include <Configuration.h>
#include <Constants.h>

void LEDMatrix::Init(Settings *settings) {
	matrix.init();

	for(uint8_t i = 0; i < NUM_OF_MATRICES; i++) {
		matrix.setRotation(i, 1);
	}

	SetBrightness(settings->data.defaultBrightness);

	matrix.fillScreen(LOW);
	matrix.write();
}

void LEDMatrix::SetBrightness(uint8_t newBrightness) {
	brightness = newBrightness;
	matrix.setIntensity(newBrightness);
}

void LEDMatrix::Update(const uint8_t *bitmap) {
	matrix.fillScreen(LOW);

	matrix.drawBitmap(0, 0, bitmap, BITMAP_SIZE, 8, HIGH);

	matrix.write();
}