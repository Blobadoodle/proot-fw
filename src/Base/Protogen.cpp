#include <Arduino.h>
#include <Data/Configuration.h>
#include <Protogen.h>

bool Protogen::Init() {
	Serial.begin(115200);
#ifdef WAIT_FOR_SERIAL
	while(!Serial);
	delay(500);
#endif
	Serial.printf("Initialising %s %s...\n", FIRMWARE_NAME, FIRMWARE_VERSION);

	bool settingSuccess = settings.Init();
#ifdef RESET_EEPROM
	Serial.println("Resetting EEPROM due to firmware flag.");
	settings.ResetSettings();
#endif

	pinMode(LDO2_EN, OUTPUT);
	digitalWrite(LDO2_EN, HIGH);
	
	FastLED.addLeds<NEOPIXEL, PIXEL_DATA>(&pixel, 1);
	pixel = CRGB::Black;

	// TODO: This sucks
	battery.Init();
	stateManager.Init(&settings);
	fan.Init(&settings);
	mic.Init(&settings);
	matrix.Init(&settings);
	rgbled.Init(&settings);
	bool internalDisplaySuccess = internalDisplay.Init(&stateManager, &settings, &battery);
	userControls.Init(&stateManager, &matrix, &fan, &settings, &rgbled);
	bool gestureSensorSuccess = gestureSensor.Init();

	if(settingSuccess && internalDisplaySuccess && gestureSensorSuccess) {
		Serial.println("Successfully initialised!");
		fullyInitialised = true;
	} else {
		Serial.println("One or more errors prevented a successful initialisation.");
		Serial.printf("Settings: %s, Internal Display: %s, Gesture Sensor: %s\n",
			settingSuccess ? "OK" : "ERROR",
			internalDisplaySuccess ? "OK" : "ERROR",
			gestureSensorSuccess ? "OK" : "ERROR");
		fullyInitialised = false;
	}

	// Show red LED if initialisation failed + error icon on internal display
	if(!fullyInitialised) {
		pixel = CRGB::Red;
		FastLED.show();
		internalDisplay.showError = true;
	}

#ifdef HARDWARE_TEST
	Serial.println("Halting main loop and performing hardware test due to firmware flag.");
#endif

	return fullyInitialised;
};

void Protogen::Tick() {
	userControls.Tick(); // Check for button presses

	stateManager.Update(gestureSensor.CheckForBoop()); // Check if the screen should be updated (boop, expression change, blink, maw glitch)

	if(stateManager.redrawNeeded) { // Update the screen if it needs to be
		engine.Update(stateManager); // Turn the state into a bitmap
		matrix.Update(engine.canvas.getBuffer()); // Update the LED matrix
		internalDisplay.Update(engine.canvas.getBuffer()); // Update the internal HUD
	} else if(stateManager.internalOnlyRedrawNeeded) { // Redraw the internal HUD only if needed
		stateManager.internalOnlyRedrawNeeded = false;
		internalDisplay.Update(engine.canvas.getBuffer());
	}

	rgbled.Tick(); // Update the RGB LEDs
}

void Protogen::HardwareTest() {
	Serial.println("Testing gesture sensor...");
	gestureSensor.HardwareTest();
	delay(2500);

	Serial.println("Testing RGB LEDs...");
	rgbled.HardwareTest();
	delay(2500);

	Serial.println("Testing battery...");
	battery.HardwareTest();
	delay(2500);

	Serial.println("Testing fan..."); // and thus also testing the level shifter
	fan.SetFanSpeed(10);
	delay(2500);
	fan.SetFanSpeed(5);
	delay(2500);
	fan.SetFanSpeed(0);

	Serial.println("Testing internal display...");
	internalDisplay.HardwareTest();
	delay(2500);

	Serial.println("Testing LED matrix...");
	matrix.HardwareTest();
	delay(2500);

	Serial.println("Testing microphone...");
	mic.HardwareTest();
	delay(2500);

	Serial.println("Test complete! Restarting in 5 seconds...");
	delay(5000);
}