#include "UserControls.h"
#include <Data/Configuration.h>
#include <Data/Expressions.h>

void UserControls::OnClick() {
	if(state->focus == Focus::Expressions) {
		#ifdef CYCLE_ON_ONE
		if(state->lastExpression >= NUM_OF_EXPRESSIONS - 1)
			state->SetExpression(0);
		else
			state->SetExpression(state->lastExpression + 1);
		#else
		state->SetExpression(0);
		#endif
	} else {
		switch(state->selectedQuickSetting) {
			case QuickSetting::RGBBrightness:
				if(rgbled->brightness >= 15)
					rgbled->SetBrightness(1);
				else
					rgbled->SetBrightness(rgbled->brightness + 1);
				settings->data.defaultRgbBrightness = rgbled->brightness;
				break;		

			case QuickSetting::Fan:
				if(fan->fanSpeed >= 10)
					fan->SetFanSpeed(0);
				else
					fan->SetFanSpeed(fan->fanSpeed + 1);
				settings->data.defaultFanSpeed = fan->fanSpeed;
				break;

			case QuickSetting::Brightness:
				if(matrix->brightness >= 15)
					matrix->SetBrightness(1);
				else
					matrix->SetBrightness(matrix->brightness + 1);
				settings->data.defaultBrightness = matrix->brightness;
				break;

			case QuickSetting::Mic:
				settings->data.micToggle = !settings->data.micToggle;
				mic->Toggle(settings->data.micToggle);
		}
		state->internalOnlyRedrawNeeded = true;
		unsavedSettings = true;
	}
}

void UserControls::OnLongClick() {
	if(state->focus == Focus::Expressions) {
		state->selectedQuickSetting = QuickSetting::RGBBrightness;
		state->focus = Focus::Quicksettings;
	} else {
		state->focus = Focus::Expressions;
		if(unsavedSettings) {
			unsavedSettings = false;
			settings->WriteSettings();
		}
	}
	state->internalOnlyRedrawNeeded = true;
}

void UserControls::OnDoubleClick() {
	if(state->focus == Focus::Quicksettings) {
		if(state->selectedQuickSetting >= QuickSetting::Mic)
			state->selectedQuickSetting = QuickSetting::RGBBrightness;
		else
			state->selectedQuickSetting = (QuickSetting)((uint8_t)state->selectedQuickSetting + 1);

		state->internalOnlyRedrawNeeded = true;
	} else {
		state->SetExpression(1);
	}
}

void UserControls::OnMultiClick() {
	if(state->focus != Focus::Expressions)
		return;

	uint8_t numClicks = button.getNumberClicks();

	if(numClicks <= NUM_OF_EXPRESSIONS)
		state->SetExpression(numClicks - 1);
}

void UserControls::Init(StateManager *_state, LEDMatrix *_matrix, Fan *_fan, Settings *_settings, RGBLED *_rgbled, Microphone *_mic) {
	state = _state;
	matrix = _matrix;
	fan = _fan;
	settings = _settings;
	rgbled = _rgbled;
	mic = _mic;

	button.setup(
		BUTTON_PIN,
		INPUT_PULLUP, // Pullup
		true // Active low
	);

	button.attachClick([](void *scope) { ((UserControls*) scope)->OnClick();}, this);
	button.attachLongPressStart([](void *scope) { ((UserControls*) scope)->OnLongClick();}, this);
	button.attachDoubleClick([](void *scope) { ((UserControls*) scope)->OnDoubleClick();}, this);
	button.attachMultiClick([](void *scope) { ((UserControls*) scope)->OnMultiClick();}, this);
}

void UserControls::Tick() {
	button.tick();
}