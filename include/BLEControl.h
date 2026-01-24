#pragma once

#include <NimBLEDevice.h>

class BLEControl: public NimBLEServerCallbacks {
    public:
        void Init();
        void onDisconnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo, int reason) override;

    protected:
        NimBLEServer *pServer;
        NimBLEService *pService;
        NimBLEAdvertising *pAdvertising;

        // There must be a better way to do this, right? I don't know NimBLE well.
        void SetupChars();
        NimBLECharacteristic *firmInfo;

        NimBLECharacteristic *currentExpression;
        NimBLECharacteristic *availableExpressions;

        NimBLECharacteristic *displayBrightness;
        NimBLECharacteristic *rgbBrightness;
        NimBLECharacteristic *fanSpeed;
        NimBLECharacteristic *micToggle;
};