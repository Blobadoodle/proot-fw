void oledBootup() {
  if(!oled.begin(OLED_ADDRESS, true)) {
    Serial.println(F("OLED allocation failed."));
    currentExpression = 5; // X_X
    drawFace(false, false);
    for(;;); // Loop forever
  }
  oled.clearDisplay();

  oled.setRotation(OLED_ROTATION); // 180 *

  oled.setCursor(0, 0);
  oled.setTextColor(SH110X_WHITE);

  // oled.println(F("ProotFW v0.1.0"));
  // oled.printf(F("Compiled at\n%s %s\n"), __DATE__, __TIME__);

  oled.drawBitmap(0, 0, bootupOLED, 128, 128, SH110X_WHITE);

  oled.display();

  delay(1500);
}

void refreshOledScreen() {
  // Face is already drawn in other parts of the program, takes up the upper 32 pixels, leaving 96

  oled.setCursor(0, 34);

  oled.setCursor(0, 120);
  oled.print(F("ProotFW v0.1.0"));

  oled.display();
}