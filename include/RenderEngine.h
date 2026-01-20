#pragma once

#include <Data/Constants.h>
#include <Data/Configuration.h>
#include <cstdint>
#include <Adafruit_GFX.h>
#include <Chrono.h>
#include <StateManager.h>
#include <SDF.h>

class RenderEngine {
	public:
		GFXcanvas1 canvas = GFXcanvas1(BITMAP_SIZE, 8);
		void Update(StateManager state);
		void Init();
	protected:
		void DrawBitmapMirrored(const uint8_t *bitmap, uint8_t x, uint8_t width);

		void DrawMaw(int mawStage);
		void Glitch(uint8_t glitchStep);

		void Blink(int8_t blinkPos);
		
		SDF eyeSDF = SDF(EYE_SIZE, 8);
		void CheckCalcSDF(uint8_t currentExpression);
		void DrawEye(bool midTransition, uint8_t transitionFrame);

		uint8_t loadedExpression = 255;
};