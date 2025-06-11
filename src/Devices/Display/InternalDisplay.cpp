#include <Arduino.h>
#include "InternalDisplay.h"
#include <Configuration.h>
#include <Constants.h>
#include "../../Data/Bitmaps.h"
#include "../../Data/Expressions.h"
#include <tuple>

bool InternalDisplay::Init(StateManager *_state, Settings *_settings) {
	state = _state;
	settings = _settings;
	batteryUpdateTime.start(BATTERY_UPDATE_TIME);

	if(!display.begin(0, false)) {
		Serial.println("Failed to init SH1107 internal display.");
		return false;
	}

	delay(250); // wait for display to init

	display.setRotation(INTERNAL_DISPLAY_ROTATION);

	display.clearDisplay();
	display.setTextColor(SH110X_WHITE, SH110X_BLACK);
	display.setTextSize(1);

	display.drawBitmap(0, 0, Bitmaps::InternalDisplay::Bootup, 128, 128, SH110X_WHITE);

	display.setCursor(0, 104);
	display.printf("%s %s\n%s %s\nBy @blobadoodle", FIRMWARE_NAME, FIRMWARE_VERSION, __DATE__, __TIME__);

	display.display();

	return true;
}

std::tuple<uint8_t, uint8_t> getCoordsForExpressionButton(uint8_t expressionNum) {
	uint8_t x = 11;
	uint8_t y = 94;

	if(expressionNum >= 3) {
		x = 48;
		y += ((expressionNum - 3) * 12);
	} else {
		y += (expressionNum * 12);
	}

	return std::make_tuple(x, y);
}

// https://github.com/rlogiacco/BatterySense/blob/master/Battery.h line 101
static inline uint8_t sigmoidal(uint16_t voltage, uint16_t minVoltage, uint16_t maxVoltage) {
	// slow
	// uint8_t result = 110 - (110 / (1 + pow(1.468 * (voltage - minVoltage)/(maxVoltage - minVoltage), 6)));

	// steep
	// uint8_t result = 102 - (102 / (1 + pow(1.621 * (voltage - minVoltage)/(maxVoltage - minVoltage), 8.1)));

	// normal
	uint8_t result = 105 - (105 / (1 + pow(1.724 * (voltage - minVoltage)/(maxVoltage - minVoltage), 5.5)));
	return result >= 100 ? 100 : result;
}


uint8_t getBatteryPercentage(float _voltage) {
	uint16_t voltage = _voltage * 1000;
	if(voltage <= BATTERY_MIN_MV)
		return 0;
	else if(voltage >= BATTERY_MAX_MV)
		return 100;
	else
		return sigmoidal(voltage, BATTERY_MIN_MV, BATTERY_MAX_MV);
}

void InternalDisplay::Update(const uint8_t *bitmap, float batteryVoltage) {
	display.clearDisplay();

	display.drawBitmap(0, 0, Bitmaps::InternalDisplay::UI, 128, 128, SH110X_WHITE);

	// Draw top left version num
	display.setCursor(1, 1);
	display.setTextSize(1);
	display.setTextColor(SH110X_BLACK, SH110X_WHITE);
	display.print(FIRMWARE_VERSION);

	// Draw battery percentage
	display.setCursor(87, 1);

	// Only update battery if atleast 15 seconds have passed since last battery as ADC inaccuracies can cause the % to flicker
	if(batteryUpdateTime.hasPassed(BATTERY_UPDATE_TIME)) {
		lastBatteryPercentage = round(getBatteryPercentage(batteryVoltage) / 5.0) * 5; // Round to nearest 5% to reduce flicker
		batteryUpdateTime.restart();
	}

	display.printf("%d%%", lastBatteryPercentage);
	display.fillRect(113, 3, lastBatteryPercentage / 10, 3, SH110X_BLACK); // Draw battery icon level

	display.setTextColor(SH110X_WHITE, SH110X_BLACK);

	// Draw firmware name
	display.setCursor(3, 82);
	display.print(FIRMWARE_NAME);

	// Draw mic gain
	display.setCursor(104, 82);
	display.printf("%d", settings->data.defaultMicGain);

	// Draw fan percentage
	display.setCursor(104, 94);
	display.printf("%d%%", settings->data.defaultFanSpeed * 10);

	// Draw brightness level
	display.setCursor(104, 106);
	display.printf("%d", settings->data.defaultBrightness);

	// Draw the default startup expression
	display.setCursor(104, 118);
	display.printf("%d", settings->data.defaultExpression + 1);

	// TODO: Find a better way of splitting the individual face parts out, rather than just drawing it thrice and covering it with a mask
	for(uint8_t i = 0; i < 3; i++) {
		display.drawBitmap(8, 16 + (8 * i), bitmap, BITMAP_SIZE, 8, SH110X_WHITE);
	}
	display.drawBitmap(8, 16, Bitmaps::InternalDisplay::UIMask, BITMAP_SIZE, 24, SH110X_BLACK);

	// Draw the expression names
	display.setTextColor(SH110X_BLACK, SH110X_WHITE);
	for(uint8_t i = 0; i < NUM_OF_EXPRESSIONS; i++) {
		uint8_t x, y;
		std::tie(x, y) = getCoordsForExpressionButton(i);

		display.setCursor(x, y);
		display.print(Expressions[i].name);
	}

	// Highlight the currently selected expression
	uint8_t x, y;
	std::tie(x, y) = getCoordsForExpressionButton(state->lastExpression);

	x -= 10;
	y -= 2;

	display.fillRect(x, y, 35, 11, SH110X_INVERSE);

	// Higlight the focused UI portion
	if(state->focus == FOCUS_EXPRESSION) {
		display.drawRect(0, 79, 74, 49, SH110X_WHITE);
	} else {
		display.drawRect(73, 79, 55, 49, SH110X_WHITE);
		// Also highlight selected quicksetting
		uint8_t x = 74;
		uint8_t y = 81 + state->selectedQuickSetting * 12;
		display.fillRect(x, y, 53, 9, SH110X_INVERSE);
	}

	// Draw boop icon if booping
	if(state->isBooping)
		display.drawBitmap(18, 65, Bitmaps::InternalDisplay::BoopIcon, 16, 12, SH110X_WHITE);

	display.display();
}