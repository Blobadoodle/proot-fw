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