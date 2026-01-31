#pragma once

// This is like Configuration.h, but it's not advised for the user to edit

#define FIRMWARE_NAME "ProotFW"
#define FIRMWARE_VERSION "v0.1.0"
#define NUM_OF_MATRICES 14

// Size of RenderEngine bitmap
#define BITMAP_SIZE NUM_OF_MATRICES * 8 

// Size in pixels of each feature
#define EYE_SIZE 16
#define MAW_SIZE 32
#define NOSE_SIZE 8

// X Offset of each feature
#define EYE_POS 0
#define MAW_POS (EYE_POS + EYE_SIZE)
#define NOSE_POS (MAW_POS + MAW_SIZE)
#define CENTER_POS (NOSE_POS + NOSE_SIZE)

// Used for the settings stored in the EEPROM
#define LATEST_SETTINGS_REVISION 0
#define SETTINGS_MAGIC "ProotFW"

// Used for RGB LEDs
#define RGBLED_CHEEK_NUM 37
#define CHEEK_OUTER_SIZE 24
#define CHEEK_INNER_SIZE 12

// Pins for UMS3 stuff
#define LDO2_EN 17
#define PIXEL_DATA 18
#define VBAT 10
#define VBUS_PRESENT 33

// FFT stuff
#define FFT_SAMPLES 256
#define FFT_SAMPLE_RATE 8000

// BLE UUIDs
#define BLE_PROTOGEN_SERVICE "7777" // Should be 128-bit but then it's too long for an advertisement packet
#define BLE_FIRMWARE_CHAR "4db97151-1825-49b7-862c-999e50f7a1de"

#define BLE_CURRENT_EXPR_CHAR "be0bf400-b046-4c76-b778-0e3a61edd7b6"
#define BLE_AVAILABLE_EXPR_CHAR "5d902e7e-619f-471e-bbc8-898e5da171de"

#define BLE_DISPLAY_BRIGHTNESS_CHAR "f108fc78-e06d-4c65-9855-185dd852ebd2"
#define BLE_RGB_BRIGHTNESS_CHAR "ce2d7a5a-21c0-4723-8e6e-9d0fd955b951"
#define BLE_FAN_SPEED_CHAR "8e6b36a5-172f-4e75-bb41-87ff8e219b36"
#define BLE_MIC_TOGGLE_CHAR "bb1c39bd-4650-44dc-a655-5ba21d19f03c"

#define BLE_FORCE_GLITCH "6c183031-caa8-4ef9-89da-d6a827de3a4a"
#define BLE_FORCE_BLINK "195acf8f-d195-496b-b773-afa2a24be13c"