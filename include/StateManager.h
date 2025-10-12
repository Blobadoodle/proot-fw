#pragma once

#include <cstdint>
#include <Chrono.h>
#include <Settings.h>

#define FOCUS_EXPRESSION 0 
#define FOCUS_QUICKSETTINGS 1

#define QUICKSETTING_RGB_BRIGHTNESS 0
#define QUICKSETTING_FAN 1
#define QUICKSETTING_BRIGHTNESS 2
#define QUICKSETTING_DEFAULT_EXPRESSION 3

#define SCREEN_MAIN 0
#define SCREEN_SETTINGS 1

class StateManager {
	public:
		void Init(Settings *_settings);
		void Update(bool boopPresent, double voicePower);
		void SetExpression(uint8_t expressionNum);

		uint8_t mawGlitchStep = 0;
		int8_t blinkPos = -8;

		bool isBlinking = false;
		bool isMawGlitching = false;
		bool isBooping = false;

		bool redrawNeeded = false;
		bool internalOnlyRedrawNeeded = false;

		int mawStage = 0;

		uint8_t currentExpression;
		uint8_t lastExpression;

		// Internal display UI stuff
		uint8_t focus = FOCUS_EXPRESSION;
		uint8_t selectedQuickSetting = QUICKSETTING_RGB_BRIGHTNESS;
	protected:
		Chrono blinkTimer = Chrono();
		Chrono blinkFrameTimer = Chrono();

		Chrono glitchTimer = Chrono();
		Chrono glitchFrameTimer = Chrono();
		
		int lastMawStage = 0;
	
		Chrono boopTimer = Chrono();

		uint8_t blinkStep = 0;
		uint32_t checkTime = 0;

		bool hasExpressionChangedInbetweenFrames = false; // long ahh variable name

		uint8_t targetExpression = 0;

		void GlitchMaw();
		void Blink();
};