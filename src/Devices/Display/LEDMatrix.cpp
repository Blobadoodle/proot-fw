#include <Arduino.h>
#include <Devices/Display/LEDMatrix.h>
#include <Data/Configuration.h>
#include <Data/Constants.h>

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

void LEDMatrix::HardwareTest() {
	for(uint8_t i = 0; i <= 15; i++) {
		matrix.setIntensity(i);
		matrix.fillScreen(HIGH);
		matrix.write();
		delay(250);
		matrix.fillScreen(LOW);
		matrix.write();
		delay(250);
	}

	matrix.setIntensity(15);
	matrix.fillScreen(HIGH);
	matrix.write();
}