#include <Devices/Display/RGBLED.h>
#include <Data/Configuration.h>

void RGBLED::Init() {
    FastLED.addLeds<NEOPIXEL, RGBLED_RIGHT_CHEEK_PIN>(rightCheek, RGBLED_CHEEK_NUM);
    FastLED.addLeds<NEOPIXEL, RGBLED_LEFT_CHEEK_PIN>(leftCheek, RGBLED_CHEEK_NUM);
    FastLED.setBrightness(RGBLED_BRIGHTNESS);
}

void RGBLED::HardwareTest() {
    fill_solid(rightCheek, RGBLED_CHEEK_NUM, CRGB::Red);
    FastLED.show();
    delay(2500);

    fill_solid(rightCheek, RGBLED_CHEEK_NUM, CRGB::Blue);
    FastLED.show();
    delay(2500);

    fill_solid(rightCheek, RGBLED_CHEEK_NUM, CRGB::Green);
    FastLED.show();
}

void RGBLED::OnFrame() {
    // Spinner animation thing for cheeks
    if(spinnerPos >= RGBLED_CHEEK_NUM) {
        spinnerPos = 0;
    }

    fadeToBlackBy(rightCheek, RGBLED_CHEEK_NUM, 64);

    // rightCheek[spinnerPos] = CRGB::LightSkyBlue;
    rightCheek[spinnerPos] = blend(CRGB::LightSkyBlue, CRGB::Purple, (sin8(millis() / 32)));

    spinnerPos++;

    // Clone right cheek to left cheek
    memcpy(leftCheek, rightCheek, sizeof(rightCheek));
    FastLED.show();
}

void RGBLED::Tick() {
    if(frameTime.hasPassed(RGBLED_FRAME_TIME)) {
        OnFrame();
        frameTime.restart();
    }
}