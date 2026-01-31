#pragma once

#include <cstdint>
#include <Chrono.h>
#include <Devices/GestureSensor.h>
#include <BLEControl.h>

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
		void Init(GestureSensor *_gestureSensor, BLEControl *_ble);
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
		bool midTransition = false;
		uint8_t transitionFrame;

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

		Chrono transitionFrameTimer = Chrono();
		
		int lastMawStage = 0;
	
		GestureSensor *gestureSensor;
		Chrono boopTimer = Chrono();

		uint8_t glitchStep = 0;
		uint8_t blinkStep = 0;

		bool hasExpressionChangedInbetweenFrames = false; // long ahh variable name

		bool forceNextBlink = false;
		bool forceNextGlitch = false;

		BLEControl *ble;

		void Glitch();
		void Blink();
		void _SetExpression(uint8_t expressionNum);

		void StartBlinking();
		void StartGlitching();
};