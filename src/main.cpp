#include <Arduino.h>
#include <Protogen.h>
#include <Data/Configuration.h>

Protogen proot;

void setup() {
	proot.Init();
}

void loop() {
#ifdef HARDWARE_TEST
	proot.HardwareTest();
#else
	proot.Tick();
#endif
}