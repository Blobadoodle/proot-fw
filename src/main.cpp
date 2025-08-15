#include <Arduino.h>
#include <Protogen.h>
#include <Data/Configuration.h>

Protogen proot;

void setup() {
	proot.Init();
#ifdef HARDWARE_TEST
	proot.HardwareTest();
#endif
}

void loop() {
#ifndef HARDWARE_TEST
	proot.Tick();
#endif
	// delay(10);
}