#pragma once

#include <cstdint>
#include <Adafruit_GFX.h>

class SDF {
    public:
        SDF(uint8_t _width, uint8_t _height);
        void CalcLerp(float time);
        void CalcBitmap();
        void LoadSDF(const float *sdf);
        void LoadNewSDF(const float *sdf);

        GFXcanvas1 canvas; // TODO: Bloat.

        uint8_t width;
        uint8_t height;

        const float *newSdf;
        float *prevSdf;
        float *currentSdf;
};