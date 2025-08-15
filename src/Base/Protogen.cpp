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

	ums3.begin();
	ums3.setPixelPower(false);
	ums3.setLDO2Power(true);

	bool settingSuccess = settings.Init();
#ifdef RESET_EEPROM
	Serial.println("Resetting EEPROM due to firmware flag.");
	settings.ResetSettings();
#endif

	stateManager.Init(&settings);
	fan.Init(&settings);
	matrix.Init(&settings);
	bool internalDisplaySuccess = internalDisplay.Init(&stateManager, &settings);
	userControls.Init(&stateManager, &matrix, &fan, &settings);;
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

	// Show red LED if initialisation failed
	if(!fullyInitialised) {
		ums3.setPixelPower(true);
		ums3.setPixelColor(0xFF0000);
		ums3.setPixelBrightness(255);
	}

	return fullyInitialised;
};

void Protogen::Tick() {
	userControls.Tick(); // Check for button presses

	stateManager.Update(gestureSensor.CheckForBoop()); // Check if the screen should be updated (boop, expression change, blink, maw glitch)

	if(stateManager.redrawNeeded) { // Update the screen if it needs to be
		engine.Update(stateManager); // Turn the state into a bitmap
		matrix.Update(engine.canvas.getBuffer()); // Update the LED matrix
		internalDisplay.Update(engine.canvas.getBuffer(), ums3.getBatteryVoltage()); // Update the internal HUD
	} else if(stateManager.internalOnlyRedrawNeeded) { // Redraw the internal HUD only if needed
		stateManager.internalOnlyRedrawNeeded = false;
		internalDisplay.Update(engine.canvas.getBuffer(), ums3.getBatteryVoltage());
	}
}

// TODO: Implement hardware test
void Protogen::HardwareTest() {
	Serial.println("Halting main loop and performing hardware test due to firmware flag.");
	fan.SetFanSpeed(10);
}