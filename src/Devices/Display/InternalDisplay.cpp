#include <Arduino.h>
#include <Devices/Display/InternalDisplay.h>
#include <Data/Configuration.h>
#include <Data/Constants.h>
#include <Data/Bitmaps.h>
#include <Data/Expressions.h>
#include <tuple>

bool InternalDisplay::Init(StateManager *_state, Settings *_settings, Battery *_battery) {
	state = _state;
	settings = _settings;
	battery = _battery;
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

void InternalDisplay::Update(const uint8_t *bitmap, bool bleConnected) {
	display.clearDisplay();

	display.drawBitmap(0, 0, Bitmaps::InternalDisplay::UI, 128, 128, SH110X_WHITE);

	// Draw top left version num
	display.setCursor(1, 1);
	display.setTextSize(1);
	display.setTextColor(SH110X_BLACK, SH110X_WHITE);
	display.print(FIRMWARE_VERSION);

	// Draw top left BT symbol if connected
	if(bleConnected)
		display.drawBitmap(display.getCursorX(), 1, Bitmaps::InternalDisplay::Bluetooth, 5, 7, SH110X_WHITE, SH110X_BLACK);

	// Only update battery if atleast 15 seconds have passed since last battery as ADC inaccuracies can cause the % to flicker
	if(batteryUpdateTime.hasPassed(BATTERY_UPDATE_TIME)) {
		lastBatteryPercentage = round(battery->GetPercentage() / 5.0) * 5; // Round to nearest 5% to reduce flicker
		batteryUpdateTime.restart();
	}

	// Right align battery percentage
	char batteryStr[5];
	sprintf(batteryStr, "%d%%", lastBatteryPercentage);
	uint8_t pixelWidth = (6 * strlen(batteryStr)) - 1; // Each char is 6 pixels wide, minus 1 to account for no trailing space on last char
	display.setCursor((109 - pixelWidth), 1);
	
	// Draw battery percentage
	display.print(batteryStr);
	display.fillRect(113, 3, lastBatteryPercentage / 10, 3, SH110X_BLACK); // Draw battery icon level

	// Draw error icon if initialisation failed
	if(showError)
		display.drawBitmap((109 - pixelWidth) - 10, 0, Bitmaps::InternalDisplay::Error, 9, 9, SH110X_WHITE, SH110X_BLACK);

	display.setTextColor(SH110X_WHITE, SH110X_BLACK);

	// Draw firmware name
	display.setCursor(3, 82);
	display.print(FIRMWARE_NAME);

	// Draw rgb brightness
	display.setCursor(104, 82);
	display.printf("%d", settings->data.defaultRgbBrightness);

	// Draw fan percentage
	display.setCursor(104, 94);
	display.printf("%d%%", settings->data.defaultFanSpeed * 10);

	// Draw brightness level
	display.setCursor(104, 106);
	display.printf("%d", settings->data.defaultBrightness);

	// Draw the mic toggle
	display.setCursor(104, 118);
	display.print(settings->data.micToggle ? "ON" : "OFF");

	// Draw the mic level icons
	if(state->mawStage >= MAW_THRESHOLD_1)
		display.drawBitmap(10, 68, Bitmaps::InternalDisplay::Speaker1, 2, 6, SH110X_WHITE);
	if(state->mawStage >= MAW_THRESHOLD_1)
		display.drawBitmap(11, 66, Bitmaps::InternalDisplay::Speaker2, 3, 10, SH110X_WHITE);
	if(state->mawStage >= MAW_THRESHOLD_1)
		display.drawBitmap(12, 64, Bitmaps::InternalDisplay::Speaker3, 4, 14, SH110X_WHITE);

	// TODO: Find a better way of splitting the individual face parts out, rather than just drawing it thrice and covering it with a mask
	for(uint8_t i = 0; i < 3; i++)
		display.drawBitmap(8, 16 + (8 * i), bitmap, BITMAP_SIZE, 8, SH110X_WHITE);

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
	if(state->focus == Focus::Expressions) {
		display.drawRect(0, 79, 74, 49, SH110X_WHITE);
	} else {
		display.drawRect(73, 79, 55, 49, SH110X_WHITE);
		// Also highlight selected quicksetting
		uint8_t x = 74;
		uint8_t y = 81 + (uint8_t)state->selectedQuickSetting * 12;
		display.fillRect(x, y, 53, 9, SH110X_INVERSE);
	}

	// Draw boop icon if booping
	if(state->isBooping)
		display.drawBitmap(18, 65, Bitmaps::InternalDisplay::BoopIcon, 16, 12, SH110X_WHITE);

	display.display();
}

void InternalDisplay::HardwareTest() {
	display.clearDisplay();
	uint8_t contrastValues[] = {0, 127, 255};

	for (uint8_t i = 0; i < sizeof(contrastValues); i++) {
		display.setContrast(i);

		display.fillScreen(SH110X_WHITE);
		display.display();
		delay(500);

		display.fillScreen(SH110X_BLACK);
		display.display();
		delay(500);
	}

	display.setContrast(255);
	display.fillScreen(SH110X_WHITE);
	display.display();
}