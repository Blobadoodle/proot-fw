#pragma once

#include <Data/Constants.h>
#include <cstdint>
#include <Adafruit_GFX.h>
#include <Chrono.h>
#include <StateManager.h>

class RenderEngine {
	public:
		GFXcanvas1 canvas = GFXcanvas1(BITMAP_SIZE, 8);
		void Update(StateManager state);
	protected:
		void DrawBitmapMirrored(const uint8_t *bitmap, uint8_t x, uint8_t width);

		void DrawMaw(int mawStage);
		void Glitch(uint8_t glitchStep);

		void Blink(int8_t blinkPos);
};