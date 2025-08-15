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