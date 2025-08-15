#pragma once

#include <cstdint>
#include <Settings.h>

class Microphone {
	public:
		void Init(Settings *settings);

		uint8_t gain = 0; // 40 or 50
		void SetGain(uint8_t newGain);

        void HardwareTest();

        // TODO: FFT spectrum analyser output
        // TODO: Audio level meter output (3 mouth stages)
};