#pragma once

#include <cstdint>
#include <Settings.h>
#include <Data/Constants.h>
#include <arduinoFFT.h>
#include <BLEControl.h>

class Microphone {
	public:
		void Init(Settings *_settings, BLEControl *_ble);

		uint8_t gain = 40; // 40 or 50
		void SetGain(uint8_t newGain);

		void Toggle(bool _enabled);

        void HardwareTest();

		int GetAmplitude();
		void Sample();
		double voicePower = 0;
	protected:
		uint32_t lastSample = 0;
		uint32_t sampleIndex = 0;

		double vReal[FFT_SAMPLES];
		double vImag[FFT_SAMPLES];

		ArduinoFFT<double> FFT = ArduinoFFT<double>(vReal, vImag, FFT_SAMPLES, FFT_SAMPLE_RATE);

		void ComputeVoicePower();

		Settings *settings;
		BLEControl *ble;

		void OnWriteToggle(bool newToggle);

		bool enabled = true;
};