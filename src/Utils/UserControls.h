#pragma once
#include <OneButton.h>
#include "../StateManager/StateManager.h"
#include "../Devices/Display/LEDMatrix.h"
#include "../Devices/Fan/Fan.h"
#include "../Utils/Settings.h"

class UserControls {
	public:
		void Init(StateManager *_state, LEDMatrix *_matrix, Fan *_fan, Settings *_settings);
		void Tick();

	protected:
		OneButton button;
		StateManager *state;
		LEDMatrix *matrix;
		Fan *fan;
		Settings *settings;

		bool unsavedSettings = false;

		void OnClick();
		void OnLongClick();
		void OnDoubleClick();
		void OnMultiClick();
};