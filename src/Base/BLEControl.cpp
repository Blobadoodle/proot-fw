#include <BLEControl.h>
#include <Data/Configuration.h>
#include <Data/Constants.h>
#include <Data/Expressions.h>
#include <Arduino.h>

#define PROPERTY_AUTH_WRITE NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::WRITE_ENC | NIMBLE_PROPERTY::WRITE_AUTHEN
#define PROPERTY_AUTH_READ NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::READ_ENC | NIMBLE_PROPERTY::READ_AUTHEN

void BLEControl::Init() {
    NimBLEDevice::init(BLE_NAME);
    NimBLEDevice::setSecurityAuth(true, true, true);
    NimBLEDevice::setSecurityIOCap(BLE_HS_IO_DISPLAY_ONLY); // TODO: Have Yes/No on HUD
    NimBLEDevice::setSecurityPasskey(BLE_PASSKEY); // TODO: Randomise

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
}

bool BLEControl::ClientConnected() {
    return pServer->getConnectedCount() >= 1;
}

void BLEControl::StartAdvertising() {
    pAdvertising->start();
}

NimBLECharacteristic *BLEControl::CreateChar(std::string uuid, uint32_t properties) {
    NimBLECharacteristic *characteristic = pService->createCharacteristic(uuid, properties);
    characteristic->setCallbacks(this);
    charMap.insert({uuid, characteristic});
    return characteristic;
}

void BLEControl::Indicate(std::string uuid) {
    NimBLECharacteristic *characteristic = charMap.at(uuid);
    characteristic->indicate();
}

void BLEControl::SetValue(std::string uuid, const char *str) {
    NimBLECharacteristic *characteristic = charMap.at(uuid);
    characteristic->setValue(str);
}

void BLEControl::SetValue(std::string uuid, const uint8_t *data, size_t len) {
    NimBLECharacteristic *characteristic = charMap.at(uuid);
    characteristic->setValue(data, len);
}

void BLEControl::SetWriteCallback(std::string uuid, std::function<void(NimBLECharacteristic*, NimBLEConnInfo&)> callback) {
    writeCallbacks.insert({uuid, callback});
}

void BLEControl::onWrite(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo) {
    std::string uuid = pCharacteristic->getUUID().toString();

    auto callback = writeCallbacks.find(uuid);
    if(callback == writeCallbacks.end()) // No callback assigned
        return;

    callback->second(pCharacteristic, connInfo);
}

void BLEControl::SetupChars() {
    // Firmware info char
    String firmwareInfoStr = String(FIRMWARE_NAME) + "," + FIRMWARE_VERSION + "," + SETTINGS_MAGIC + "," + String(LATEST_SETTINGS_REVISION) + "," + __DATE__ + " " + __TIME__; // This too kinda sucks
    firmInfo = CreateChar(BLE_FIRMWARE_CHAR, NIMBLE_PROPERTY::READ);
    firmInfo->setValue(firmwareInfoStr.c_str());

    // Expression char's
    currentExpression = CreateChar(BLE_CURRENT_EXPR_CHAR, PROPERTY_AUTH_READ | PROPERTY_AUTH_WRITE | NIMBLE_PROPERTY::INDICATE);
    const uint8_t data[] = {DEFAULT_EXPRESSION};
    currentExpression->setValue(data, sizeof(data));

    availableExpressions = CreateChar(BLE_AVAILABLE_EXPR_CHAR, PROPERTY_AUTH_READ);
    String expressionsStr = "";
    for(uint8_t i = 0; i < NUM_OF_EXPRESSIONS; i++) {
        expressionsStr += String(Expressions[i].name);
        if(i != NUM_OF_EXPRESSIONS - 1)
            expressionsStr += ","; // TODO: Delimit by NUL instead
    }
    availableExpressions->setValue(expressionsStr.c_str());
    
    // Quick setting char's (also available from InternalDisplay)
    displayBrightness = CreateChar(BLE_DISPLAY_BRIGHTNESS_CHAR, PROPERTY_AUTH_READ | PROPERTY_AUTH_WRITE);
    rgbBrightness = CreateChar(BLE_RGB_BRIGHTNESS_CHAR, PROPERTY_AUTH_READ | PROPERTY_AUTH_WRITE);
    fanSpeed = CreateChar(BLE_FAN_SPEED_CHAR, PROPERTY_AUTH_READ | PROPERTY_AUTH_WRITE);
    micToggle = CreateChar(BLE_MIC_TOGGLE_CHAR, PROPERTY_AUTH_READ | PROPERTY_AUTH_WRITE);
}

void BLEControl::onDisconnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo, int reason) {
    NimBLEDevice::startAdvertising();
}