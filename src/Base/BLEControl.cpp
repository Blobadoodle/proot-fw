#include <BLEControl.h>
#include <Data/Configuration.h>
#include <Data/Constants.h>
#include <Data/Expressions.h>
#include <Arduino.h>

void BLEControl::Init() {
    NimBLEDevice::init(BLE_NAME);

    pServer = NimBLEDevice::createServer();
    pServer->setCallbacks(this);
    
    pService = pServer->createService(BLE_PROTOGEN_SERVICE);
    SetupChars();
    pService->start();
    
    pAdvertising = NimBLEDevice::getAdvertising();
    pAdvertising->setName(BLE_NAME);
    pAdvertising->enableScanResponse(false);
    pAdvertising->setAppearance(BLE_APPEARANCE);
    pAdvertising->addServiceUUID(pService->getUUID());
    pAdvertising->start();
}

void BLEControl::SetupChars() {
    // Firmware info char
    String firmwareInfoStr = String(FIRMWARE_NAME) + "," + String(FIRMWARE_VERSION) + "," + String(SETTINGS_MAGIC) + "," + String(LATEST_SETTINGS_REVISION);
    firmInfo = pService->createCharacteristic(BLE_FIRMWARE_CHAR, NIMBLE_PROPERTY::READ);
    firmInfo->setValue(firmwareInfoStr.c_str());

    // Expression char's
    currentExpression = pService->createCharacteristic(BLE_CURRENT_EXPR_CHAR, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::NOTIFY);

    availableExpressions = pService->createCharacteristic(BLE_AVAILABLE_EXPR_CHAR, NIMBLE_PROPERTY::READ);
    String expressionsStr = "";
    for(uint8_t i = 0; i < NUM_OF_EXPRESSIONS; i++) {
        expressionsStr += String(Expressions[i].name);
        if(i != NUM_OF_EXPRESSIONS - 1)
            expressionsStr += ","; // TODO: Delimit by NUL instead
    }
    availableExpressions->setValue(expressionsStr.c_str());
    
    // Quick setting char's (also available from InternalDisplay)
    displayBrightness = pService->createCharacteristic(BLE_DISPLAY_BRIGHTNESS_CHAR, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE);
    rgbBrightness = pService->createCharacteristic(BLE_RGB_BRIGHTNESS_CHAR, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE);
    fanSpeed = pService->createCharacteristic(BLE_FAN_SPEED_CHAR, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE);
    micToggle = pService->createCharacteristic(BLE_MIC_TOGGLE_CHAR, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE);
}

void BLEControl::onDisconnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo, int reason) {
    NimBLEDevice::startAdvertising();
}