// Interrupt service routine, on button rising edge

volatile unsigned long lastDebounceTime = 0;

void onButton() {
  unsigned long debounceTime = millis();

  if(debounceTime - lastDebounceTime > DEBOUNCE_TIME) {
    incrementExpression();

    // if(currentBrightness == 15)
    //   setBrightness(0);
    // else
    //   setBrightness(currentBrightness + 1);
  }

  lastDebounceTime = debounceTime;
}