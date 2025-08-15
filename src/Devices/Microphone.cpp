#include <Devices/Microphone.h>
#include <Data/Configuration.h>
#include <Arduino.h>

void Microphone::Init(Settings *settings) {
	pinMode(MIC_INPUT, INPUT);
    pinMode(MIC_GAIN, OUTPUT);
	SetGain(settings->data.defaultMicGain);
}

void Microphone::SetGain(uint8_t newGain) {
    if(newGain == 40) {
        digitalWrite(MIC_GAIN, HIGH);
    } else if (newGain == 50) {
        digitalWrite(MIC_GAIN, HIGH);
    } else {
        Serial.printf("Invalid microphone gain: %d. Valid values are 40 or 50.\n", gain);
        return;
    }

	gain = newGain;
}

void Microphone::HardwareTest() {
    uint16_t micInput = analogRead(MIC_INPUT);

    Serial.printf("Mic gain: %d | Mic value: %d\n", gain, micInput);
}