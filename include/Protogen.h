#pragma once

#include <Settings.h>
#include <Devices/Display/InternalDisplay.h>
#include <Devices/Display/LEDMatrix.h>
#include <Devices/Display/RGBLED.h>
#include <Devices/GestureSensor.h>
#include <Devices/Fan.h>
#include <Devices/Microphone.h>
#include <Devices/Battery.h>
#include <RenderEngine.h>
#include <UserControls.h>
#include <FastLED.h>

class Protogen {
	public:
		Settings settings;
		InternalDisplay internalDisplay;
		LEDMatrix matrix;
		RGBLED rgbled;
		GestureSensor gestureSensor;
		RenderEngine engine;
		StateManager stateManager;
		UserControls userControls;
		Fan fan;
		Microphone mic;
		Battery battery;

		bool fullyInitialised = false;

		CRGB pixel;

		bool Init();
		void Tick();
		void HardwareTest();
};