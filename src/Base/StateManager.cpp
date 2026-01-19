#include "StateManager.h"
#include <Data/Configuration.h>
#include <Arduino.h>

void StateManager::GlitchMaw() {
	if(glitchFrameTimer.hasPassed(GLITCH_FRAME_TIME)) {
		redrawNeeded = true;
		mawGlitchStep += 1;

		if(mawGlitchStep == 2) {
			isMawGlitching = false;
			mawGlitchStep = 0;
			redrawNeeded = true;
			glitchTimer.restart();
			glitchFrameTimer.stop();
			return;
		}

		glitchFrameTimer.restart();
	}
}

void StateManager::Blink() {
	if(!blinkFrameTimer.hasPassed(BLINK_FRAME_TIME))
		return;

	if(blinkStep == 4) {
		if(blinkShutTimer.hasPassed(BLINK_SHUT_TIME))
			blinkShutTimer.stop();
		else
			return;
	}

	redrawNeeded = true;
	blinkStep++;

	if(blinkStep < 4)
		blinkPos = blinkStep - 4;
	else
		blinkPos = -(blinkStep - 4);

	if(blinkStep == 4) {
		lastExpression = targetExpression;
		if(!isBooping)
			currentExpression = targetExpression;
		blinkShutTimer.restart();
	}

	if(blinkStep > 8) {
		isBlinking = false;

		blinkTimer.restart();
		blinkFrameTimer.stop();
		blinkStep = 0;
		blinkPos = -4;
	} else {
		blinkFrameTimer.restart();
	}
}

void StateManager::Update(bool boopPresent, double voicePower) {
	redrawNeeded = false;

	if(isMawGlitching) {
		GlitchMaw();
	} else if(glitchTimer.hasPassed(GLITCH_TIME)) {
		isMawGlitching = true;
		redrawNeeded = true;
	
		glitchFrameTimer.start();
		glitchTimer.stop();
	}
	
	if(isBlinking) {
		Blink();
	} else if(blinkTimer.hasPassed(BLINK_TIME) || hasExpressionChangedInbetweenFrames) {
		isBlinking = true;
		redrawNeeded = true;

		blinkFrameTimer.start();
		blinkTimer.stop();
	}

	// Fixes flashing every time BOOP_COOLDOWN is finished
	if(!boopPresent && isBooping && gestureSensor->HardCheckForBoop())
		boopTimer.restart();

	if(boopPresent && !isBooping) {
		currentExpression = EXPRESSION_VWV;
		isBooping = true;
		boopTimer.restart();
		redrawNeeded = true;
	} else if(!boopPresent && isBooping && boopTimer.hasPassed(BOOP_COOLDOWN)) {
		currentExpression = lastExpression;
		isBooping = false;
		redrawNeeded = true;
	}

	lastMawStage = mawStage;
	if(voicePower > MAW_THRESHOLD_3)
		mawStage = MAW_THRESHOLD_3;
	else if (voicePower > MAW_THRESHOLD_2)
		mawStage = MAW_THRESHOLD_2;
	else if (voicePower > MAW_THRESHOLD_1)
		mawStage = MAW_THRESHOLD_1;
	else
		mawStage = 0;

	if(lastMawStage != mawStage)
		redrawNeeded = true;	

	if(hasExpressionChangedInbetweenFrames)
		hasExpressionChangedInbetweenFrames = false;
}

void StateManager::Init(Settings *settings, GestureSensor *_gestureSensor) {
	blinkFrameTimer.stop();
	blinkShutTimer.stop();
	glitchFrameTimer.stop();
	boopTimer.stop();

	currentExpression = DEFAULT_EXPRESSION;
	lastExpression = currentExpression;
	targetExpression = currentExpression;

	gestureSensor = _gestureSensor;
}

void StateManager::SetExpression(uint8_t expressionNum) {
	hasExpressionChangedInbetweenFrames = true;
	targetExpression = expressionNum;
}