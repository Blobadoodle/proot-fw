#pragma once

#include <Constants.h>
#include <cstdint>
#include <Adafruit_GFX.h>
#include <Chrono.h>
#include "../StateManager/StateManager.h"

class RenderEngine {
	public:
		GFXcanvas1 canvas = GFXcanvas1(BITMAP_SIZE, 8);
		void Update(StateManager state);
		// void Init();
	protected:
		void DrawBitmapMirrored(const uint8_t *bitmap, uint8_t x, uint8_t width);

		// uint8_t mawGlitchStep = 0;
		// bool isMawGlitching = false;
		void GlitchMaw(uint8_t mawGlitchStep);

		// int8_t blinkPos = 0;
		// uint8_t blinkStep = 0;
		// bool isBlinking = false;
		void Blink(int8_t blinkPos);

		// Chrono blinkTimer = Chrono();
		// Chrono blinkFrameTimer = Chrono();
		// Chrono glitchTimer = Chrono();
		// Chrono glitchFrameTimer = Chrono();
};