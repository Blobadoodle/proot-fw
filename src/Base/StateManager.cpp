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
	if(blinkFrameTimer.hasPassed(BLINK_FRAME_TIME)) {
		redrawNeeded = true;
		blinkStep++;

		if(blinkStep < 8)
			blinkPos = blinkStep - 8;
		else
			blinkPos = -(blinkStep - 8);

		if(blinkStep == 8) {
			lastExpression = targetExpression;
			if(!isBooping)
				currentExpression = targetExpression;
		}

		if(blinkStep == 17) {
			isBlinking = false;

			blinkTimer.restart();
			blinkFrameTimer.stop();
			blinkStep = 0;
			blinkPos = -8;
			return;
		}

		blinkFrameTimer.restart();
	}
}

void StateManager::Update(bool boopPresent) {
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

	// TODO: Make this not flash every time BOOP_COOLDOWN is finished
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

	if(hasExpressionChangedInbetweenFrames)
		hasExpressionChangedInbetweenFrames = false;
}

void StateManager::Init(Settings *settings) {
	blinkFrameTimer.stop();
	glitchFrameTimer.stop();
	boopTimer.stop();

	currentExpression = settings->data.defaultExpression;
	lastExpression = currentExpression;
	targetExpression = currentExpression;
}

void StateManager::SetExpression(uint8_t expressionNum) {
	hasExpressionChangedInbetweenFrames = true;
	targetExpression = expressionNum;
}