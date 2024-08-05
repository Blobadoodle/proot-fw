// Blink, duh.
uint8_t blinkStep = 0;
int8_t blinkPos = 0;
void blink() {
  if(!isNumBeingDisplayed) {
    matrix.fillRect(EYE_RIGHT, blinkPos, 16, 8, LOW);
    oled.fillRect(EYE_RIGHT + 8, blinkPos + 8, 16, 8, SH110X_BLACK);
  }

  matrix.fillRect(EYE_LEFT, blinkPos, 16, 8, LOW);
  oled.fillRect(EYE_LEFT + 8, blinkPos + 8, 16, 8, SH110X_BLACK);

  if(currentMillis - blinkSpeedTime > BLINK_SPEED) {
    blinkStep++;
    
    if(blinkStep < 8)
      blinkPos = blinkStep - 8;
    else
      blinkPos = -(blinkStep - 8);

    if(changeExpressionBlink && blinkStep == 8)
      currentExpression = newExpression;

    if(blinkStep == 17) {
      lastBlinkTime = currentMillis;
      forceNextBlink = false;
      changeExpressionBlink = false;
      blinkStep = 0;
    }

    blinkSpeedTime = currentMillis;
  }
}