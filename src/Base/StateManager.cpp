#include "StateManager.h"
#include <Data/Configuration.h>
#include <Arduino.h>

void StateManager::Glitch() {
	if(glitchFrameTimer.hasPassed(GLITCH_FRAME_TIME)) {
		redrawNeeded = true;
		glitchStep++;

		if(glitchStep <= GLITCH_FRAME_NUM / 2)
			glitchPos++;
		else
			glitchPos--;
		
		if(glitchStep == GLITCH_FRAME_NUM) {
			isGlitching = false;
			glitchStep = 0;
			glitchPos = 0;
			redrawNeeded = true;
			glitchTimer.restart();
			glitchFrameTimer.stop();
			nextGlitchTime = random(GLITCH_TIME_MIN, GLITCH_TIME_MAX);
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
		blinkShutTimer.restart();
	}

	if(blinkStep > 8) {
		isBlinking = false;

		nextBlinkTime = random(BLINK_TIME_MIN, BLINK_TIME_MAX);
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

	if(isGlitching) {
		Glitch();
	} else if(glitchTimer.hasPassed(nextGlitchTime)) {
		isGlitching = true;
		redrawNeeded = true;
	
		glitchFrameTimer.start();
		glitchTimer.stop();
	}
	
	if(isBlinking) {
		Blink();
	} else if(blinkTimer.hasPassed(nextBlinkTime)) {
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
		midTransition = true;
		transitionFrame = 1;
		transitionFrameTimer.restart();
		isBooping = true;
		boopTimer.restart();
		redrawNeeded = true;
	} else if(!boopPresent && isBooping && boopTimer.hasPassed(BOOP_COOLDOWN)) {
		currentExpression = lastExpression;
		isBooping = false;
		redrawNeeded = true;
		midTransition = true;
		transitionFrame = 1;
		transitionFrameTimer.restart();
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

	if(hasExpressionChangedInbetweenFrames) {
		hasExpressionChangedInbetweenFrames = false;
		redrawNeeded = true;
	}

	if(midTransition) {
		if(transitionFrame >= TRANSITION_FRAMES) {
			midTransition = false;
			transitionFrameTimer.stop();
		} else if(transitionFrameTimer.hasPassed(TRANSITION_FRAME_TIME)) {
			transitionFrame++;
			transitionFrameTimer.restart();
			redrawNeeded = true;
		}
	}
}

void StateManager::Init(Settings *settings, GestureSensor *_gestureSensor) {
	blinkFrameTimer.stop();
	blinkShutTimer.stop();
	glitchFrameTimer.stop();
	boopTimer.stop();
	transitionFrameTimer.stop();
	nextGlitchTime = random(GLITCH_TIME_MIN, GLITCH_TIME_MAX);
	nextBlinkTime = random(BLINK_TIME_MIN, BLINK_TIME_MAX);

	currentExpression = DEFAULT_EXPRESSION;
	lastExpression = currentExpression;

	gestureSensor = _gestureSensor;
}

void StateManager::SetExpression(uint8_t expressionNum) {
	if(expressionNum == lastExpression) return;

	hasExpressionChangedInbetweenFrames = true;
	midTransition = true;
	transitionFrame = 1;
	transitionFrameTimer.restart();
	currentExpression = expressionNum;
	lastExpression = expressionNum;
}