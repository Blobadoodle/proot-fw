#include <SDF.h>

#define SDF_SIZE sizeof(float) * width * height

SDF::SDF(uint8_t _width, uint8_t _height) : canvas(_width, _height) {
    width = _width;
    height = _height;

    currentSdf = new float[width * height];
    prevSdf = new float[width * height];

    memset(currentSdf, 0, SDF_SIZE);
}

void SDF::LoadSDF(const float *sdf) {
    newSdf = sdf;
    memcpy(currentSdf, sdf, SDF_SIZE);
    memcpy(prevSdf, sdf, SDF_SIZE);
}

void SDF::LoadNewSDF(const float *sdf) {
    newSdf = sdf;
    memcpy(prevSdf, currentSdf, sizeof(float) * width * height);
}

void SDF::CalcBitmap() {
    canvas.fillScreen(LOW);
    for(uint8_t y = 0; y < height; y++) {
        for(uint8_t x = 0; x < width; x++) {
            canvas.drawPixel(x, y, currentSdf[x + y * width] < 1);
        }
    }
}

void SDF::CalcLerp(float time) {
    if(time == 1) {
        memcpy(currentSdf, newSdf, SDF_SIZE);
        return;
    }

    for(uint8_t y = 0; y < height; y++) {
        for(uint8_t x = 0; x < width; x++) {
            currentSdf[x + y * width] = (prevSdf[x + y * width] + time * (newSdf[x + y * width] - prevSdf[x + y * width]));
        }
    }
}