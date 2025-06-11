#include "UserControls.h"
#include <Configuration.h>
#include "../Data/Expressions.h"

void UserControls::OnClick() {
	if(state->focus == FOCUS_EXPRESSION) {
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
			case QUICKSETTING_MIC_GAIN:
				if(settings->data.defaultMicGain == 40)
					settings->data.defaultMicGain = 50;
				else
					settings->data.defaultMicGain = 40;
				break;
			case QUICKSETTING_FAN:
				if(fan->fanSpeed >= 10)
					fan->SetFanSpeed(0);
				else
					fan->SetFanSpeed(fan->fanSpeed + 1);
				settings->data.defaultFanSpeed = fan->fanSpeed;
				break;
			case QUICKSETTING_BRIGHTNESS:
				if(matrix->brightness >= 15)
					matrix->SetBrightness(1);
				else
					matrix->SetBrightness(matrix->brightness + 1);
				settings->data.defaultBrightness = matrix->brightness;
				break;
			case QUICKSETTING_DEFAULT_EXPRESSION:
				if(settings->data.defaultExpression >= NUM_OF_EXPRESSIONS - 1)
					settings->data.defaultExpression = 0;
				else
					settings->data.defaultExpression++;
		}
		state->internalOnlyRedrawNeeded = true;
		unsavedSettings = true;
	}
}

void UserControls::OnLongClick() {
	if(state->focus == FOCUS_EXPRESSION) {
		state->selectedQuickSetting = QUICKSETTING_MIC_GAIN;
		state->focus = FOCUS_QUICKSETTINGS;
	} else {
		state->focus = FOCUS_EXPRESSION;
		if(unsavedSettings) {
			unsavedSettings = false;
			settings->WriteSettings();
		}
	}
	state->internalOnlyRedrawNeeded = true;
}

void UserControls::OnDoubleClick() {
	if(state->focus == FOCUS_QUICKSETTINGS) {
		if(state->selectedQuickSetting >= QUICKSETTING_DEFAULT_EXPRESSION)
			state->selectedQuickSetting = QUICKSETTING_MIC_GAIN;
		else
			state->selectedQuickSetting++;

		state->internalOnlyRedrawNeeded = true;
	} else {
		state->SetExpression(1);
	}
}

void UserControls::OnMultiClick() {
	if(state->focus != FOCUS_EXPRESSION)
		return;

	uint8_t numClicks = button.getNumberClicks();

	if(numClicks <= NUM_OF_EXPRESSIONS)
		state->SetExpression(numClicks - 1);
}

void UserControls::Init(StateManager *_state, LEDMatrix *_matrix, Fan *_fan, Settings *_settings) {
	state = _state;
	matrix = _matrix;
	fan = _fan;
	settings = _settings;

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