#pragma once

#include <NimBLEDevice.h>
#include <unordered_map>

class BLEControl: public NimBLEServerCallbacks, public NimBLECharacteristicCallbacks {
    public:
        void Init();
        void StartAdvertising();
        bool ClientConnected();

        void onDisconnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo, int reason) override;
        void onWrite(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo) override;

        void SetWriteCallback(std::string uuid, std::function<void(NimBLECharacteristic*, NimBLEConnInfo&)>);
        void SetValue(std::string uuid, const char *str);
        void SetValue(std::string uuid, const uint8_t *data, size_t len);
        void Indicate(std::string uuid);
        void Notify(std::string uuid);
    protected:
        NimBLEServer *pServer;
        NimBLEService *pService;
        NimBLEAdvertising *pAdvertising;

        // There must be a better way to do this, right? I don't know NimBLE well.
        void SetupChars();
        std::unordered_map<std::string, NimBLECharacteristic*> charMap;
        std::unordered_map<std::string, std::function<void(NimBLECharacteristic*, NimBLEConnInfo&)>> writeCallbacks;
        NimBLECharacteristic* CreateChar(std::string uuid, uint32_t properties);

        // TODO: Store these only in the unordered map
        NimBLECharacteristic *firmInfo;
        NimBLECharacteristic *boopCounter;
        NimBLECharacteristic *sessionBoopCounter;

        NimBLECharacteristic *currentExpression;
        NimBLECharacteristic *availableExpressions;

        NimBLECharacteristic *displayBrightness;
        NimBLECharacteristic *rgbBrightness;
        NimBLECharacteristic *fanSpeed;
        NimBLECharacteristic *micToggle;
        NimBLECharacteristic *writeSettings;

        NimBLECharacteristic *forceBoop;
        NimBLECharacteristic *forceGlitch;
        NimBLECharacteristic *forceBlink;
};