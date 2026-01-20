#pragma once

#include <cstdint>
#include <Adafruit_GFX.h>

class SDF {
    public:
        SDF(uint8_t _width, uint8_t _height);
        void CalcLerp(float time);
        void CalcBitmap();
        void LoadSDF(const int8_t *sdf);

        GFXcanvas1 canvas;

        uint8_t width;
        uint8_t height;
        const int8_t *newSdf;
        const int8_t *prevSdf;
        int8_t *currentSdf;
};