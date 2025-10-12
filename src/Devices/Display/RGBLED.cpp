#include <Devices/Display/RGBLED.h>
#include <Data/Configuration.h>

void RGBLED::Init(Settings *settings) {
    FastLED.addLeds<NEOPIXEL, RGBLED_RIGHT_CHEEK_PIN>(rightCheek, RGBLED_CHEEK_NUM);
    FastLED.addLeds<NEOPIXEL, RGBLED_LEFT_CHEEK_PIN>(leftCheek, RGBLED_CHEEK_NUM);
    FastLED.setBrightness(settings->data.defaultRgbBrightness * (255 / 15));
}

void RGBLED::HardwareTest() {
    fill_solid(rightCheek, RGBLED_CHEEK_NUM, CRGB::Red);
    memcpy(leftCheek, rightCheek, sizeof(rightCheek));
    FastLED.show();
    delay(2500);

    fill_solid(rightCheek, RGBLED_CHEEK_NUM, CRGB::Blue);
    memcpy(leftCheek, rightCheek, sizeof(rightCheek));
    FastLED.show();
    delay(2500);

    fill_solid(rightCheek, RGBLED_CHEEK_NUM, CRGB::Green);
    memcpy(leftCheek, rightCheek, sizeof(rightCheek));
    FastLED.show();
}

// Spinner animation thing for cheeks
void RGBLED::OnFrame() {
    CRGB pixelColour = blend(CRGB::LightSkyBlue, CRGB::Purple, sin8(millis() / 32));

    fadeToBlackBy(rightCheek, RGBLED_CHEEK_NUM, 64);

    if(outerSpinnerPos >= CHEEK_OUTER_SIZE)
        outerSpinnerPos = 0;
    
    if(innerSpinnerPos >= CHEEK_INNER_SIZE)
        innerSpinnerPos = 11;

    rightCheek[outerSpinnerPos] = rightCheek[CHEEK_OUTER_SIZE + innerSpinnerPos] = rightCheek[RGBLED_CHEEK_NUM - 1] = pixelColour;

    outerSpinnerPos++;
    if(!updatedInnerLastFrame) {
        innerSpinnerPos--;
        updatedInnerLastFrame = true;
    } else {
        updatedInnerLastFrame = false;
    }

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

void RGBLED::SetBrightness(uint8_t newBrightness) {
    brightness = newBrightness;
    FastLED.setBrightness(newBrightness * (255 / 15));
}