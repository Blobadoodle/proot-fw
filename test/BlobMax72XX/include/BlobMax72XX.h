#pragma once

#include <Adafruit_GFX.h>
#include <Arduino.h>

class BlobMax72XX : public Adafruit_GFX {
	public:
		BlobMax72XX(uint8_t _csPin, uint8_t _displays = 1);

		void init();

		// void setRotation(uint8_t rotation);

		void drawPixel(int16_t x, int16_t y, uint16_t color);

		// void fillScreen(uint16_t color);

		// void shutdown(bool status);

		// void setBrightness(uint8_t brightness);

		// void write();
	protected:
		uint8_t csPin;

		void spiTransfer(uint8_t opcode, uint8_t data);

		uint8_t *bitmap;
		uint8_t bitmapSize;

		uint8_t numDisplays;

		uint8_t *matrixRotation;
};