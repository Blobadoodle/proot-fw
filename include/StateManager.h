#pragma once

#include <cstdint>
#include <Chrono.h>
#include <Settings.h>
#include <Devices/GestureSensor.h>

#define FOCUS_EXPRESSION 0 
#define FOCUS_QUICKSETTINGS 1

#define QUICKSETTING_RGB_BRIGHTNESS 0
#define QUICKSETTING_FAN 1
#define QUICKSETTING_BRIGHTNESS 2
#define QUICKSETTING_MIC 3

#define SCREEN_MAIN 0
#define SCREEN_SETTINGS 1

class StateManager {
	public:
		void Init(Settings *settings, GestureSensor *_gestureSensor);
		void Update(bool boopPresent, double voicePower);
		void SetExpression(uint8_t expressionNum);

		uint8_t glitchPos = 0;
		int8_t blinkPos = -4;

		bool isBlinking = false;
		bool isGlitching = false;
		bool isBooping = false;

		bool redrawNeeded = false;
		bool internalOnlyRedrawNeeded = false;
		bool firstDrawDone = false;

		int mawStage = 0;

		uint8_t currentExpression;
		uint8_t lastExpression;

		// Internal display UI stuff
		uint8_t focus = FOCUS_EXPRESSION;
		uint8_t selectedQuickSetting = QUICKSETTING_RGB_BRIGHTNESS;
	protected:
		uint32_t nextBlinkTime = 0;
		Chrono blinkTimer = Chrono();
		Chrono blinkFrameTimer = Chrono();
		Chrono blinkShutTimer = Chrono();

		uint32_t nextGlitchTime = 0;
		Chrono glitchTimer = Chrono();
		Chrono glitchFrameTimer = Chrono();
		
		int lastMawStage = 0;
	
		GestureSensor *gestureSensor;
		Chrono boopTimer = Chrono();

		uint8_t glitchStep = 0;
		uint8_t blinkStep = 0;

		bool hasExpressionChangedInbetweenFrames = false; // long ahh variable name

		uint8_t targetExpression = 0;

		void Glitch();
		void Blink();
};