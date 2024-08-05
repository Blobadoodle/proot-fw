// Actually run the control comamnds
void handleLinkCommand(char command) {
  switch(command) {
    case '>': 
    case 'c': // Cycle through expression, like a button press
      incrementExpression();
      return;
    case '<': // Cycle through expressions but backwards
      if(currentExpression == 0)
        setCurrentExpression(NUM_OF_EXPRESSIONS - 1);
      else
        setCurrentExpression(currentExpression - 1);
      signalRedraw = true;
      return;
    case '+': // Increase brightness
      if(currentBrightness == 15)
        setBrightness(0);
      else
        setBrightness(currentBrightness + 1);
      return;
    case '-': // Decrease brightness
      if(currentBrightness == 0)
        setBrightness(15);
      else
        setBrightness(currentBrightness - 1);
      return;
    case 'b': // Force screen to blink
      forceNextBlink = true;
      return;
    case 'g': // Force screen to glitch
      forceNextGlitch = true;
      return;
  }

  // Set expression to specific one
  if(command >= '0' && command <= ('0' + NUM_OF_EXPRESSIONS) - 1) {
    setCurrentExpression(command - '0');
    signalRedraw = true;
  }
}

// receive data from Serial (and Serial1 if enabled)
void receiveLinkData() {
  if(Serial.available() > 0)
    handleLinkCommand(Serial.read());

#ifdef LINK_ENABLED
  if(Serial1.available() > 0)
    handleLinkCommand(Serial1.read());
#endif
}