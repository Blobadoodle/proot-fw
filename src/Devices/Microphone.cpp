#include <Devices/Microphone.h>
#include <Data/Configuration.h>
#include <Data/Constants.h>
#include <Arduino.h>

#define SAMPLE_PERIOD 1000000 / FFT_SAMPLE_RATE

void Microphone::Init(Settings *settings) {
	pinMode(MIC_INPUT, INPUT);
    pinMode(MIC_GAIN_PIN, OUTPUT);
	SetGain(MIC_GAIN_VALUE);
}

void Microphone::SetGain(uint8_t newGain) {
    if(newGain == 40) {
        digitalWrite(MIC_GAIN_PIN, HIGH);
    } else if (newGain == 50) {
        digitalWrite(MIC_GAIN_PIN, LOW);
    } else {
        Serial.printf("Invalid microphone gain: %d. Valid values are 40 or 50.\n", gain);
        return;
    }

	gain = newGain;
}

int Microphone::GetAmplitude() {
    return analogRead(MIC_INPUT);
}

void Microphone::ComputeVoicePower() {
    FFT.dcRemoval();
    FFT.windowing(FFTWindow::Hamming, FFTDirection::Forward);
    FFT.compute(FFTDirection::Forward);
    FFT.complexToMagnitude();

    voicePower = 0;
    uint16_t count = 0;
    for (int i = 0; i < FFT_SAMPLES / 2; i++) {
        double frequency = (i * FFT_SAMPLE_RATE) / FFT_SAMPLES;
        if(frequency >= 300 && frequency <= 3000) { // Human voice range
            voicePower += vReal[i];
            count++;
        }
    }

    voicePower = voicePower / (double)count;
    sampleIndex = 0;
}

void Microphone::Sample() {
    uint32_t now = micros();
    
    if(now - lastSample >= SAMPLE_PERIOD) {
        lastSample = now;
        vReal[sampleIndex] = GetAmplitude() - 2048;
        vImag[sampleIndex] = 0;
        sampleIndex++;
    }

    if(sampleIndex >= FFT_SAMPLES)
        ComputeVoicePower();
}

void Microphone::HardwareTest() {
    uint16_t micInput = analogRead(MIC_INPUT);

    Serial.printf("Mic gain: %d | Mic value: %d\n", gain, micInput);
}