#pragma once

#include "../Utils/Settings.h"
#include "../Devices/Display/InternalDisplay.h"
#include "../Devices/Display/LEDMatrix.h"
#include "../Devices/GestureSensor/GestureSensor.h"
#include "../Devices/Fan/Fan.h"
#include "../RenderEngine/RenderEngine.h"
#include "../Utils/UserControls.h"
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