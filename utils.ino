void showNum(uint8_t num) {
  isNumBeingDisplayed = true;
  displayingNum = num;
  displayingNumTime = millis();
  signalRedraw = true;
}

void setBrightness(uint8_t brightness) {
  matrix.setIntensity(brightness);
  showNum(brightness);
  currentBrightness = brightness;
}

// Change colour of RGB LED
void setLedColour(uint8_t red, uint8_t green, uint8_t blue) {
#ifdef LED_ENABLED
  analogWrite(RED_PIN, red);
  analogWrite(GREEN_PIN, green);
  analogWrite(BLUE_PIN, blue);
  showingLed = true;
  ledTime = currentMillis;
#endif
}

void setCurrentExpression(uint8_t expression) {
  newExpression = expression;
  changeExpressionBlink = true;
  signalRedraw = true;
  setLedColour(colours[expression][0], colours[expression][1], colours[expression][2]);
}

// Cycle through facial expressions
void incrementExpression() {
  if(currentExpression == NUM_OF_EXPRESSIONS - 1)
    setCurrentExpression(0);
  else
    setCurrentExpression(currentExpression + 1);
  signalRedraw = true;
}