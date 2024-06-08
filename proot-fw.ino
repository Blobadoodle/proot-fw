#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>
#include "icons.h"

// TODO: 

// [] ---------- Feature Toggles ---------- []

// #define IR_ENABLED // Boop sensor
// #define MIC_ENABLED // Speaking animation
#define LINK_ENABLED // Control over RX/TX
// #define MAGNET_ENABLED // Magnet sensor
// #define NEOPIXEL_ENABLED // Neopixels (duh)

// #define DEBUG_REDRAWS // print each time a redraw is done (along with redraw number)

// [] ---------- Pin Definitions ---------- []

// RGB LED
#define RED_PIN   5
#define GREEN_PIN 6
#define BLUE_PIN  4

// Controls
#define BTN_PIN 2

// MAX7219 Chip Select
#define CS 10

// [] ------------- Constants ------------- []

// Button debounce
#define DEBOUNCE_TIME 100

// Screen settings
#define HORIZONTAL_DISPLAYS 14
#define VERTICAL_DISPLAYS 1
#define DEFAULT_BRIGHTNESS 1 // 0-15

#define BAUD_RATE 115200

#define NUM_OF_EXPRESSIONS 4

// How long a num shows for (when changing brightness)
#define NUM_SHOWING_TIME 2000

#define BLINK_TIME 9000
#define BLINK_SPEED 20

#define GLITCH_TIME 7000
#define GLITCH_SPEED 100

#define LED_SHOWING_TIME 1000

// Don't touch these, these are the positions of each display (in pixels across)
#define EYE_RIGHT 0
#define MAW_RIGHT (EYE_RIGHT + 16)
#define NOSE_RIGHT (MAW_RIGHT + 32)
#define NOSE_LEFT (NOSE_RIGHT + 8)
#define MAW_LEFT (NOSE_LEFT + 8)
#define EYE_LEFT (MAW_LEFT + 32)

// Colours for each expression
const uint8_t colours[NUM_OF_EXPRESSIONS][3] = {
  {0, 8, 0},  // Green  = Happy    :)
  {0, 0, 8},  // Blue   = Spooked O_O
  {8, 0, 0},  // Red    = Angry   >:(
  {8, 0, 8}   // Purple = Booped  >w<
};

// [] ------------- Variables ------------- []

Max72xxPanel matrix = Max72xxPanel(CS, HORIZONTAL_DISPLAYS, VERTICAL_DISPLAYS);
uint8_t currentExpression = 0;
#ifdef DEBUG_REDRAWS
unsigned long numOfRedraws = 0;
#endif
volatile bool signalRedraw = true;

unsigned long currentMillis = 0;

unsigned long displayingNumTime = 0; // The millis() at the time the num started being shown
uint8_t displayingNum = 0;  
bool isNumBeingDisplayed = false;
uint8_t currentBrightness = DEFAULT_BRIGHTNESS;

unsigned long ledTime = 0;
bool showingLed = false;

unsigned long lastBlinkTime = 0;
unsigned long blinkSpeedTime = 0;

unsigned long lastGlitchTime = 0;
unsigned long glitchSpeedTime = 0;

bool forceNextGlitch = false;
bool forceNextBlink = false;

bool changeExpressionBlink = false;
uint8_t newExpression;

// [] ------------- Begin Code ------------ []

void setup() {
  Serial.begin(BAUD_RATE);
  Serial.println("ProotFW v0.1.0 initialising...");
  Serial.printf("Compiled at %s %s\n", __DATE__, __TIME__);

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  pinMode(BTN_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(BTN_PIN), onButton, FALLING);

  for(uint8_t i = 0; i < 14; i++) {
    matrix.setRotation(i, 1);
  }

  matrix.setIntensity(currentBrightness);

#ifdef LINK_ENABLED
  Serial1.begin(BAUD_RATE);
  Serial.println("Link enabled.");
#endif

  Serial.println("Initialisation complete.");
}

void loop() {
  currentMillis = millis();

  // Disable brightness number if it's been on for too long
  if(isNumBeingDisplayed && ((currentMillis - displayingNumTime) > NUM_SHOWING_TIME)) {
    signalRedraw = true;
    isNumBeingDisplayed = false;
  }

  // Disable LED if it's been on for too long
  if(showingLed && ((currentMillis - ledTime) > LED_SHOWING_TIME)) {
    showingLed = false;
    setLedColour(0, 0, 0);
  }

  bool shouldBlink = forceNextBlink || ((currentMillis - lastBlinkTime) > BLINK_TIME);
  signalRedraw |= (shouldBlink && (currentExpression != 3)); // he don't blink when booped

  bool shouldGlitch = forceNextGlitch || ((currentMillis - lastGlitchTime) > GLITCH_TIME);
  signalRedraw |= shouldGlitch;

  signalRedraw |= changeExpressionBlink;

  receiveLinkData();

  if(signalRedraw) {
    signalRedraw = false;

#ifdef DEBUG_REDRAWS
    Serial.printf("redraw(%d)\n", numOfRedraws);
    numOfRedraws++;
#endif

    drawFace(shouldBlink, shouldGlitch);
    matrix.write();
  }
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

// Guh.
void drawRightEye(const uint8_t *bitmap) {
  if(isNumBeingDisplayed) 
    writeNum(displayingNum);
  else 
    matrix.drawBitmap(EYE_RIGHT, 0, bitmap, 16, 8, HIGH);
}

// Blink, duh.
uint8_t blinkStep = 0;
uint8_t blinkPos = 0;
void blink() {
  if(!isNumBeingDisplayed)
    matrix.fillRect(EYE_RIGHT, blinkPos, 16, 8, LOW);

  matrix.fillRect(EYE_LEFT, blinkPos, 16, 8, LOW);


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

uint8_t glitchStep = 0;
void glitch() {
  if(glitchStep == 1) {
    matrix.drawBitmap(MAW_RIGHT, 0, Glitch1, 32, 8, HIGH);
    matrix.drawBitmap(MAW_LEFT, 0, Glitch1L, 32, 8, HIGH);
  } else {
    matrix.drawBitmap(MAW_RIGHT, 0, Glitch2, 32, 8, HIGH);
    matrix.drawBitmap(MAW_LEFT, 0, Glitch2L, 32, 8, HIGH);
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
    }
  }
}

// Load icon to display
void drawFace(bool shouldBlink, bool shouldGlitch) {
  matrix.fillScreen(LOW);

  if(!shouldGlitch) {
    matrix.drawBitmap(MAW_RIGHT, 0, maw, 32, 8, HIGH);
    matrix.drawBitmap(MAW_LEFT, 0, mawL, 32, 8, HIGH);
  } else {
    glitch();
  }

  matrix.drawBitmap(NOSE_RIGHT, 0, nose, 8, 8, HIGH);
  matrix.drawBitmap(NOSE_LEFT, 0, noseL, 8, 8, HIGH);

  switch(currentExpression) {
    case 0: // Happy :)
      drawRightEye(Eye);
      matrix.drawBitmap(EYE_LEFT, 0, EyeL, 16, 8, HIGH);
      break;
    
    case 1: // Spooked O_O
      drawRightEye(Spooked);
      matrix.drawBitmap(EYE_LEFT, 0, Spooked, 16, 8, HIGH);
      break;

    case 2: // Angry >:(
      drawRightEye(Angry);
      matrix.drawBitmap(EYE_LEFT, 0, AngryL, 16, 8, HIGH);
      break;

    case 3: // Booped >w<
      drawRightEye(vwv);
      matrix.drawBitmap(EYE_LEFT, 0, vwv, 16, 8, HIGH);
      break;
  }

  if(shouldBlink || changeExpressionBlink)
    blink();
}

void showNum(uint8_t num) {
  isNumBeingDisplayed = true;
  displayingNum = num;
  displayingNumTime = millis();
  signalRedraw = true;
}

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

void setBrightness(uint8_t brightness) {
  matrix.setIntensity(brightness);
  showNum(brightness);
  currentBrightness = brightness;
}

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

// Change colour of RGB LED
void setLedColour(uint8_t red, uint8_t green, uint8_t blue) {
  analogWrite(RED_PIN, red);
  analogWrite(GREEN_PIN, green);
  analogWrite(BLUE_PIN, blue);
  showingLed = true;
  ledTime = currentMillis;
}
