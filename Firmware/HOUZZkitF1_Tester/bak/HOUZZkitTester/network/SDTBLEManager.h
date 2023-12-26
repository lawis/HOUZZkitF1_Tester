#ifndef __SDT_BLEMANAGER_H__
#define __SDT_BLEMANAGER_H__

#include <Arduino.h>
#include "Dot2D/dot2d.h"
#include "SDTNetBase.h"
#include "HOUZZkitTester/SDTConfig.h"


#if LBE_USE_PSRAM

#include <NimBLEDevice.h>

class BleManager : public NetBase , NimBLECharacteristicCallbacks , NimBLEServerCallbacks
{
private:

protected:

    NimBLEServer* bleServer = nullptr;

    NimBLEService* bleService = nullptr;

    NimBLECharacteristic *bleTxCharacteristic = nullptr;

    NimBLECharacteristic *bleRxCharacteristic = nullptr;

public:

    void onConnect(NimBLEServer* pServer) {
#if PRINT_DEBUG_INFO
        LOG("BLE onConnect");
        Serial.println("Multi-connect support: start advertising");
#endif
        NimBLEDevice::startAdvertising();
    };

    void onDisconnect(NimBLEServer* pServer) {
#if PRINT_DEBUG_INFO
        LOG("BLE onDisconnect");
        Serial.println("Client disconnected - start advertising");
#endif
        NimBLEDevice::startAdvertising();
    };

    //BLECharacteristicCallbacks override
    void onWrite(NimBLECharacteristic *pCharacteristic);

    BleManager(NetBaseDelegate *delegate);

    ~BleManager();

    bool init();

    void start();

    void stop();

    void free();

    void sendData(std::string& data);

    void sendData(uint8_t* data, size_t length);

    void receivedData(std::string& data);

};

#else 

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

class BleManager : public NetBase , BLECharacteristicCallbacks , BLEServerCallbacks
{
private:

protected:

    BLEServer* bleServer = nullptr;

    BLEService* bleService = nullptr;

    BLECharacteristic *bleTxCharacteristic = nullptr;

    BLECharacteristic *bleRxCharacteristic = nullptr;

public:

    void onConnect(BLEServer* pServer) {
#if PRINT_DEBUG_INFO
        LOG("BLE onConnect");
        // Serial.println("Multi-connect support: start advertising");
#endif
        pServer->startAdvertising();
    };

    void onDisconnect(BLEServer* pServer) {
#if PRINT_DEBUG_INFO
        LOG("BLE onDisconnect");
        // Serial.println("Client disconnected - start advertising");
#endif
        pServer->startAdvertising();
    };

    //BLECharacteristicCallbacks override
    void onWrite(BLECharacteristic *pCharacteristic);

    BleManager(NetBaseDelegate *delegate);

    ~BleManager();

    bool init();

    void start();

    void stop();

    void free();

    void sendData(std::string& data);

    void sendData(uint8_t* data, size_t length);

    void receivedData(std::string& data);

};



#endif




#endif //__SDT_BLEMANAGER_H__