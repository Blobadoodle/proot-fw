#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>
#include <Adafruit_SH110X.h>

#include "icons.h"

// TODO: 

// [] ---------- Feature Toggles ---------- []

// #define IR_ENABLED // Boop sensor
// #define MIC_ENABLED // Speaking animation
#define LINK_ENABLED // Control over RX/TX
// #define MAGNET_ENABLED // Magnet sensor
// #define NEOPIXEL_ENABLED // Neopixels (duh)
// #define LED_ENABLED // RGB LED to show current expression

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

#define NUM_OF_EXPRESSIONS 6

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

// OLED stuff
#define OLED_WIDTH 128
#define OLED_HEIGHT 128
#define OLED_RESET -1
#define OLED_ADDRESS 0x3C
// 1 for 90*, 2 for 180*, 3 for 270*
#define OLED_ROTATION 2

// Colours for each expression (when using RGB LED)
const uint8_t colours[NUM_OF_EXPRESSIONS][3] = {
  {0, 8, 0}, // Green  = Happy    :)
  {0, 0, 8}, // Blue   = Spooked O_O
  {8, 0, 0}, // Red    = Angry   >:(
  {8, 0, 8}, // Purple = Booped  >w<
  {8, 8, 0}, // Yellow = Blush   o//o
  {0, 8, 8}, // Teal?  = Dead    X_X
};

// Expression names for the oled display

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

Adafruit_SH1107 oled = Adafruit_SH1107(OLED_WIDTH, OLED_HEIGHT, &Wire, OLED_RESET, 1000000, 100000);

// [] ------------- Begin Code ------------ []

void setup() {
  Serial.begin(BAUD_RATE);
  Serial.println(F("ProotFW v0.1.0 initialising..."));
  Serial.printf(F("Compiled at %s %s\n"), __DATE__, __TIME__);

  oledBootup();

#ifdef LED_ENABLED
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
#endif

  pinMode(BTN_PIN, INPUT);

  attachInterrupt(digitalPinToInterrupt(BTN_PIN), onButton, FALLING);

  for(uint8_t i = 0; i < 14; i++) {
    matrix.setRotation(i, 1);
  }
  matrix.setIntensity(currentBrightness);

#ifdef LINK_ENABLED
  Serial1.begin(BAUD_RATE);
  Serial.println(F("Link enabled."));
#endif

  Serial.println(F("Initialisation complete."));
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
    Serial.printf(F("redraw(%d)\n"), numOfRedraws);
    numOfRedraws++;
#endif

    drawFace(shouldBlink, shouldGlitch);
    matrix.write();
    refreshOledScreen();
  }
}