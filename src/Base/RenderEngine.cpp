#include "RenderEngine.h"
#include <Data/Bitmaps.h>
#include <Data/Expressions.h>
#include <StateManager.h>
#include <Data/Constants.h>
#include <Data/Configuration.h>
#include <cstdint>

// Reverse order of bits in byte
// https://stackoverflow.com/a/2602885
uint8_t reverseBits(uint8_t x) {
   x = (x & 0xF0) >> 4 | (x & 0x0F) << 4;
   x = (x & 0xCC) >> 2 | (x & 0x33) << 2;
   x = (x & 0xAA) >> 1 | (x & 0x55) << 1;
   return x;
}

// Horizontally flip a 1-bit bitmap
void flipBitmap(uint8_t *bitmap, int width, int height) {
    int bytesPerRow = (width + 7) / 8;
    
    for (int row = 0; row < height; row++) {
        uint8_t *rowPtr = bitmap + row * bytesPerRow;
        
        // Flip bits in each byte
        for (int i = 0; i < bytesPerRow; i++) {
            rowPtr[i] = reverseBits(rowPtr[i]);
        }
        
        // Reverse the bytes in the row
        for (int i = 0; i < bytesPerRow / 2; i++) {
            uint8_t temp = rowPtr[i];
            rowPtr[i] = rowPtr[bytesPerRow - i - 1];
            rowPtr[bytesPerRow - i - 1] = temp;
        }
    }
}

// Get position of facial feature for the other side of the display
#define GET_LEFT_POS(pos, width) (BITMAP_SIZE - (pos + width))

// Draw a bitmap on both sides of the display
void RenderEngine::DrawBitmapMirrored(const uint8_t *bitmap, uint8_t x, uint8_t width) {
	canvas.drawBitmap(x, 0, bitmap, width, 8, HIGH);

	// Flip bitmap for drawing on the other side
	uint8_t *flippedBitmap = new uint8_t[width * 8];
	memcpy(flippedBitmap, bitmap, width * 8);
	flipBitmap(flippedBitmap, width, 8);

	canvas.drawBitmap(GET_LEFT_POS(x, width), 0, flippedBitmap, width, 8, HIGH);

	delete[] flippedBitmap;
}

void RenderEngine::Blink(int8_t blinkPos) {
	canvas.fillRect(EYE_POS, blinkPos, 16, 4, LOW); // Top
	canvas.fillRect(EYE_POS, -blinkPos + 4, 16, 4, LOW); // Bottom

	canvas.fillRect(GET_LEFT_POS(EYE_POS, EYE_SIZE), blinkPos, 16, 4, LOW); // Top
	canvas.fillRect(GET_LEFT_POS(EYE_POS, EYE_SIZE), -blinkPos + 4, 16, 4, LOW); // Bottom
}

void RenderEngine::Glitch(uint8_t glitchPos) {
	uint8_t glitchThreshold = glitchPos / 2;

	for(uint8_t x = 0; x < NUM_OF_MATRICES * 8; x++) {
		for(uint8_t y = 0; y < 8; y++) {
			if(glitchThreshold > random(100))
				canvas.drawPixel(x, y, canvas.getPixel(x, y) ^ 1); // Invert/flip
		}
	}
}

void RenderEngine::DrawMaw(int mawStage) {
	switch(mawStage) {
		case 0:
			DrawBitmapMirrored(Bitmaps::LEDMatrix::Maw::Normal, MAW_POS, MAW_SIZE);
			break;
		case MAW_THRESHOLD_1:
			DrawBitmapMirrored(Bitmaps::LEDMatrix::Maw::Stage1, MAW_POS, MAW_SIZE);
			break;
		case MAW_THRESHOLD_2:
			DrawBitmapMirrored(Bitmaps::LEDMatrix::Maw::Stage2, MAW_POS, MAW_SIZE);
			break;
		case MAW_THRESHOLD_3:
			DrawBitmapMirrored(Bitmaps::LEDMatrix::Maw::Stage3, MAW_POS, MAW_SIZE);
			break;
	}
}

void RenderEngine::Update(StateManager state) {
	canvas.fillScreen(LOW);

	// Draw eye and blink
	DrawBitmapMirrored(Expressions[state.currentExpression].eyeBitmap, EYE_POS, EYE_SIZE);
	if(state.isBlinking)
		Blink(state.blinkPos);

	// Draw nose
	DrawBitmapMirrored(Bitmaps::LEDMatrix::Nose, NOSE_POS, NOSE_SIZE);

	// Draw maw

	DrawMaw(state.mawStage);

	if(state.isGlitching)
		Glitch(state.glitchPos);
}