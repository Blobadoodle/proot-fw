#include <SDF.h>

SDF::SDF(uint8_t _width, uint8_t _height) : canvas(_width, _height) {
    width = _width;
    height = _height;

    currentSdf = new int8_t[width * height];
    prevSdf = new int8_t[width * height];
    newSdf = new int8_t[width * height];

    memset(currentSdf, 0, width * height);
}

void SDF::LoadSDF(const int8_t *sdf) {
    prevSdf = sdf;
    newSdf = sdf;
    memcpy(currentSdf, prevSdf, width * height);
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
    for(uint8_t y = 0; y < height; y++) {
        for(uint8_t x = 0; x < width; x++) {
            currentSdf[x + y * width] = (int8_t)((float)prevSdf[x + y * width] + time * ((float)newSdf[x + y * width] - (float)prevSdf[x + y * width]));
        }
    }
}