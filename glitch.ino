uint8_t glitchStep = 0;
void glitch() {
  if(glitchStep == 1) {
    matrix.drawBitmap(MAW_RIGHT, 0, Glitch1, 32, 8, HIGH);
    matrix.drawBitmap(MAW_LEFT, 0, Glitch1L, 32, 8, HIGH);

    oled.drawBitmap(MAW_RIGHT + 8, 24, Glitch1, 32, 8, SH110X_WHITE);
    oled.drawBitmap(MAW_LEFT + 8, 24, Glitch1L, 32, 8, SH110X_WHITE);
  } else {
    matrix.drawBitmap(MAW_RIGHT, 0, Glitch2, 32, 8, HIGH);
    matrix.drawBitmap(MAW_LEFT, 0, Glitch2L, 32, 8, HIGH);

    oled.drawBitmap(MAW_RIGHT + 8, 24, Glitch2, 32, 8, SH110X_WHITE);
    oled.drawBitmap(MAW_LEFT + 8, 24, Glitch2L, 32, 8, SH110X_WHITE);
  }

  if(currentMillis - glitchSpeedTime > GLITCH_SPEED) {
    glitchStep++;
    glitchSpeedTime = currentMillis;

    if(glitchStep == 3) {
      glitchStep = 0;
      lastGlitchTime = currentMillis;
      forceNextGlitch = false;
      signalRedraw = true;
      matrix.drawBitmap(MAW_RIGHT, 0, maw, 32, 8, HIGH);
      matrix.drawBitmap(MAW_LEFT, 0, mawL, 32, 8, HIGH);

      oled.drawBitmap(MAW_RIGHT + 8, 24, maw, 32, 8, SH110X_WHITE);
      oled.drawBitmap(MAW_LEFT + 8, 24, mawL, 32, 8, SH110X_WHITE);
    }
  }
}