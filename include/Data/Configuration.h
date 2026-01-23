#pragma once

#include <Data/Expressions.h>
#include <FastLED.h>

// CS/LOAD Pin for MAX7219's
#define MATRIX_CS 34

// How long inbetween blinks (ms)
#define BLINK_TIME_MIN 6500
#define BLINK_TIME_MAX 12000
// How long each blink frame should show for (ms)
#define BLINK_FRAME_TIME 20
// How long the eye should stay fully shut when blinking (ms)
#define BLINK_SHUT_TIME 80

// How long inbetween glitch (ms)
#define GLITCH_TIME_MIN 25000
#define GLITCH_TIME_MAX 40000
// How long each glitch frame should show for (ms)
#define GLITCH_FRAME_TIME 10
// How many frames of glitch should be shown
#define GLITCH_FRAME_NUM 28

// Default expression
#define DEFAULT_EXPRESSION EXPRESSION_HAPPY

// Transition time/steps
#define TRANSITION_FRAME_TIME 5
#define TRANSITION_FRAMES 15

// Interrupt pin for gesture sensor
#define APDS_INT 7
// How close hand has to be to register boop, 0->255
#define BOOP_THRESHOLD 25
// How long a boop should last (ms)
#define BOOP_COOLDOWN 2000

// Pins of SH1107 InternalDisplay
#define SH1107_DC 41
#define SH1107_RST -1
#define SH1107_CS 42
// Rotation of internal OLED
#define INTERNAL_DISPLAY_ROTATION 1

// Pin for control button
#define BUTTON_PIN 21
// Whether 1 button click should mean cycle expression, or go to the 1st
// #define CYCLE_ON_ONE

// Pin for fan PWM
#define FAN_PIN 14

// Battery min/max voltage for percentage calculation
#define BATTERY_MIN_MV 3000
#define BATTERY_MAX_MV 4200

// How often to update battery percentage on the UI
#define BATTERY_UPDATE_TIME 15000

// Use a steep sigmoid for battery percentage calculation
// #define BATTERY_SIGMOID_STEEP
// Use a slow sigmoid for battery percentage calculation
// #define BATTERY_SIGMOID_SLOW

// Pins for MAX9814 microphone
#define MIC_INPUT 15
#define MIC_GAIN_PIN 16
// Valid mic gain values are 40 or 50
#define MIC_GAIN_VALUE 40
// Maw stages
#define MAW_THRESHOLD_1 1300
#define MAW_THRESHOLD_2 1500
#define MAW_THRESHOLD_3 1700

// Pins for RGB LEDs
#define RGBLED_RIGHT_CHEEK_PIN 12
#define RGBLED_LEFT_CHEEK_PIN 13

// Self explanatory, 0->255
#define RGBLED_BRIGHTNESS 255

// How often to update the RGB LEDs (ms)
#define RGBLED_FRAME_TIME 100

// RGB LED gradient values
#define RGBLED_COLOUR_1 0x3A61B4
#define RGBLED_COLOUR_2 0x80D6E0
#define RGBLED_POSITION_2 160 // 0-255
#define RGBLED_COLOUR_3 0xCEEBDF

// Perform a hardware test on startup
// #define HARDWARE_TEST
// Debug, not init until serial connection
// #define WAIT_FOR_SERIAL
// Reset EEPROM on startup
// #define RESET_EEPROM