#pragma once

#include <Data/Expressions.h>

// CS/LOAD Pin for MAX7219's
#define MATRIX_CS 34

// How long inbetween blinks (ms)
#define BLINK_TIME 9000
// How long each blink frame should show for (ms)
#define BLINK_FRAME_TIME 20

// How long inbetween maw glitch (ms)
#define GLITCH_TIME 7000
// How long each maw glitch frame should show for (ms)
#define GLITCH_FRAME_TIME 100

// Default expression
#define DEFAULT_EXPRESSION EXPRESSION_HAPPY

// Interrupt pin for gesture sensor
#define APDS_INT 7
// How close hand has to be to register boop, 0->255
#define BOOP_THRESHOLD 36
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
#define MAW_THRESHOLD_1 350
#define MAW_THRESHOLD_2 550
#define MAW_THRESHOLD_3 700

// Pins for RGB LEDs
#define RGBLED_RIGHT_CHEEK_PIN 12
#define RGBLED_LEFT_CHEEK_PIN 13

// Self explanatory, 0->255
#define RGBLED_BRIGHTNESS 255

// How often to update the RGB LEDs (ms)
#define RGBLED_FRAME_TIME 100

// Perform a hardware test on startup
// #define HARDWARE_TEST
// Debug, not init until serial connection
// #define WAIT_FOR_SERIAL
// Reset EEPROM on startup
// #define RESET_EEPROM