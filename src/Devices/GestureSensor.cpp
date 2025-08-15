#include <Arduino.h>
#include <Data/Configuration.h>
#include <Devices/GestureSensor.h>

bool GestureSensor::Init() {
	pinMode(APDS_INT, INPUT_PULLUP);

	if(!apds.begin()) {
		Serial.println("Failed to init APDS9960 gesture sensor.");
		return false;
	}

	apds.enableProximity(true);
	apds.setProximityInterruptThreshold(0, BOOP_THRESHOLD);
	apds.enableProximityInterrupt();

	return true;
}

bool GestureSensor::CheckForBoop() {
	if(!digitalRead(APDS_INT)) {
		apds.clearInterrupt();
		return true;
	} else {
		return false;
	}
}

void GestureSensor::HardwareTest() {
	apds.enableColor(true);
	apds.enableGesture(true);

	uint16_t r, g, b, c;
	apds.getColorData(&r, &g, &b, &c);
	Serial.printf("Color Data - R: %d, G: %d, B: %d, C: %d\n", r, g, b, c);

	Serial.println("Wave your hand in front of the sensor to test gesture detection.");
	uint8_t gesture;
	do {
		gesture = apds.readGesture();
		switch (gesture) {
			case APDS9960_UP:
				Serial.println("Gesture: UP");
				break;
			case APDS9960_DOWN:
				Serial.println("Gesture: DOWN");
				break;
			case APDS9960_LEFT:
				Serial.println("Gesture: LEFT");
				break;
			case APDS9960_RIGHT:
				Serial.println("Gesture: RIGHT");
		}
	} while (gesture == 0); // Stop when gesture is detected
}