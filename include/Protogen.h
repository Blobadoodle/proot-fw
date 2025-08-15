#pragma once

#include <Settings.h>
#include <Devices/Display/InternalDisplay.h>
#include <Devices/Display/LEDMatrix.h>
#include <Devices/GestureSensor.h>
#include <Devices/Fan.h>
#include <RenderEngine.h>
#include <UserControls.h>
#include <UMS3.h>

class Protogen {
	public:
		UMS3 ums3;
		Settings settings;
		InternalDisplay internalDisplay;
		LEDMatrix matrix;
		GestureSensor gestureSensor;
		RenderEngine engine;
		StateManager stateManager;
		UserControls userControls;
		Fan fan;

		bool fullyInitialised = false;

		bool Init();
		void Tick();
		void HardwareTest();
};