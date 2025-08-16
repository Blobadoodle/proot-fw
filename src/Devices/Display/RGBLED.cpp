#include <Devices/Display/RGBLED.h>
#include <Data/Configuration.h>

void RGBLED::Init() {
    FastLED.addLeds<NEOPIXEL, RGBLED_RIGHT_CHEEK_PIN>(rightCheek, RGBLED_CHEEK_NUM);
    // FastLED.addLeds<NEOPIXEL, RGBLED_LEFT_CHEEK_PIN>(leftCheek, RGBLED_CHEEK_NUM);
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