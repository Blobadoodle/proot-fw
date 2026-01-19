#pragma once

#include <cstdint>
#include <Chrono.h>
#include <Settings.h>
#include <Devices/GestureSensor.h>

enum class Focus {
	Expressions,
	Quicksettings
};

enum class QuickSetting {
	RGBBrightness,
	Fan,
	Brightness,
	Mic
};

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
		Focus focus = Focus::Expressions;
		QuickSetting selectedQuickSetting = QuickSetting::RGBBrightness;
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