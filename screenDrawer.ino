// Write number to right eye (used for showing current brightness) (only used in above function, use showNum instead)
void writeNum(uint8_t num) {
  // matrix.fillRect(EYE_RIGHT, 0, 16, 8, LOW);

  if(num > 9) {
    uint8_t tens = num / 10;
    uint8_t ones = num - (tens * 10);

    matrix.drawChar(EYE_RIGHT, 0, '0' + tens, HIGH, HIGH, 1);
    matrix.drawChar(EYE_RIGHT + 8, 0, '0' + ones, HIGH, HIGH, 1);
  } else {
    matrix.drawChar(EYE_RIGHT, 0, '0' + num, HIGH, HIGH, 1);
  }
}

// Guh.
void drawRightEye(const uint8_t *bitmap) {
  if(isNumBeingDisplayed) {
    writeNum(displayingNum);
  } else {
    matrix.drawBitmap(EYE_RIGHT, 0, bitmap, 16, 8, HIGH);
    oled.drawBitmap(EYE_RIGHT + 8, 8, bitmap, 16, 8, SH110X_WHITE);
  }
}

// Guh 2x
void drawLeftEye(const uint8_t *bitmap) {
  matrix.drawBitmap(EYE_LEFT, 0, bitmap, 16, 8, HIGH);
  oled.drawBitmap(EYE_LEFT + 8, 8, bitmap, 16, 8, SH110X_WHITE);
}

// Load icon to display
void drawFace(bool shouldBlink, bool shouldGlitch) {
  matrix.fillScreen(LOW);
  oled.clearDisplay();

  if(!shouldGlitch) {
    matrix.drawBitmap(MAW_RIGHT, 0, maw, 32, 8, HIGH);
    matrix.drawBitmap(MAW_LEFT, 0, mawL, 32, 8, HIGH);

    oled.drawBitmap(MAW_RIGHT + 8, 24, maw, 32, 8, SH110X_WHITE);
    oled.drawBitmap(MAW_LEFT + 8, 24, mawL, 32, 8, SH110X_WHITE);
  } else {
    glitch();
  }

  matrix.drawBitmap(NOSE_RIGHT, 0, nose, 8, 8, HIGH);
  matrix.drawBitmap(NOSE_LEFT, 0, noseL, 8, 8, HIGH);

  oled.drawBitmap(NOSE_RIGHT + 8, 16, nose, 8, 8, SH110X_WHITE);
  oled.drawBitmap(NOSE_LEFT + 8, 16, noseL, 8, 8, SH110X_WHITE);

  switch(currentExpression) {
    case 0: // Happy :)
      drawRightEye(Eye);
      drawLeftEye(EyeL);
      break;
    
    case 1: // Spooked O_O
      drawRightEye(Spooked);
      drawLeftEye(Spooked);
      break;

    case 2: // Angry >:(
      drawRightEye(Angry);
      drawLeftEye(AngryL);
      break;

    case 3: // Booped >w<
      drawRightEye(vwv);
      drawLeftEye(vwvL);
      break;
    
    case 4: // Blush o//o
      drawRightEye(Blush);
      drawLeftEye(BlushL);
      break;
    
    case 5: // Dead X_X
      drawRightEye(Dead);
      drawLeftEye(Dead);
  }

  if(shouldBlink || changeExpressionBlink)
    blink();
}