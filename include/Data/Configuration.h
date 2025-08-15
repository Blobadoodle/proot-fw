#pragma once

// Rotation of internal OLED
#define INTERNAL_DISPLAY_ROTATION 2

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

// Interrupt pin for gesture sensor
#define APDS_INT 7
// How close hand has to be to register boop, 0->255
#define BOOP_THRESHOLD 40
// How long a boop should last (ms)
#define BOOP_COOLDOWN 2000

// Pins of SH1107 InternalDisplay
#define SH1107_DC 41
#define SH1107_RST -1
#define SH1107_CS 42

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

// Pins for MAX9814 microphone
#define MIC_INPUT 35
#define MIC_GAIN 36

// Perform a hardware test on startup
#define HARDWARE_TEST
// Debug, not init until serial connection
#define WAIT_FOR_SERIAL
// Reset EEPROM on startup
// #define RESET_EEPROM