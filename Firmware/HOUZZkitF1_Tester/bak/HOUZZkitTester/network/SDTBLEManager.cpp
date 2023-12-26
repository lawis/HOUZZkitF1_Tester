#include "SDTBLEManager.h"
#include "../SDTSystem.h"
#include "HOUZZkitTester/SDTConfig.h"


// BLE UDID
#define BLE_UUID "6E123456-B5A3-F393-E0A9-E50E28DCCA9E"
// RX串口标识
#define CHARACTERISTIC_UUID_RX "6E123457-B5A3-F393-E0A9-E50E24DCCA9E"
// TX串口标识
#define CHARACTERISTIC_UUID_TX "6E123458-B5A3-F393-E0A9-E50E24DCCA9E"

#if LBE_USE_PSRAM

BleManager::BleManager(NetBaseDelegate *delegate)
:NetBase(delegate)
{

}

BleManager::~BleManager()
{
    

}

bool BleManager::init()
{
    NimBLEDevice::init(SDTSystem::getInstance()->name().c_str());

    bleServer = NimBLEDevice::createServer();
    bleServer->setCallbacks(this);

    bleService = bleServer->createService(BLE_UUID);

    bleTxCharacteristic = bleService->createCharacteristic(
        CHARACTERISTIC_UUID_TX,NIMBLE_PROPERTY::NOTIFY);
    // bleTxCharacteristic->addDescriptor(new BLE2902());

    bleRxCharacteristic = bleService->createCharacteristic(
        CHARACTERISTIC_UUID_RX,NIMBLE_PROPERTY::WRITE);

    bleRxCharacteristic->setCallbacks(this);

    BLEAdvertising *pAdvertising = bleServer->getAdvertising();
    BLEAdvertisementData oAdvertisementCustom = BLEAdvertisementData();

#if defined(DEVICE_PLUS)
    oAdvertisementCustom.setManufacturerData(std::string("YBY0906BOARD", 11));
#elif defined(DEVICE_PRO)
    oAdvertisementCustom.setManufacturerData(std::string("YBY0906PRO", 10));
#elif defined(DEVICE_CLASSICAL)
    oAdvertisementCustom.setManufacturerData(std::string("YBY0906CLASSICAL", 16));
#else
    oAdvertisementCustom.setManufacturerData(std::string("YBY0906NONE", 11));
#endif
    pAdvertising->setAdvertisementData(oAdvertisementCustom);

    return true;
}

void BleManager::onWrite(NimBLECharacteristic *pCharacteristic)
{
    std::string res = pCharacteristic->getValue();
    this->receivedData(res);
}

void BleManager::start()
{
    bleService->start();
    bleServer->getAdvertising()->start();
}

void BleManager::stop()
{
    // bleService->stop();
    bleServer->getAdvertising()->stop();
}

void BleManager::free()
{

    BLEDevice::deinit(true);
    delete bleTxCharacteristic;
    delete bleRxCharacteristic;
    bleServer->removeService(bleService);
    delete bleService;
    // delete bleServer;
    
}

void BleManager::sendData(std::string& data)
{
    bleTxCharacteristic->setValue(data);
    bleTxCharacteristic->notify();
}

void BleManager::sendData(uint8_t* data, size_t length)
{
    bleTxCharacteristic->setValue(data,length);
    bleTxCharacteristic->notify();
}

void BleManager::receivedData(std::string& data)
{
    if(_delegate)
    {
        _delegate->dataParse(data,this);
    }
}

#else


BleManager::BleManager(NetBaseDelegate *delegate)
:NetBase(delegate)
{

}

BleManager::~BleManager()
{
    

}

bool BleManager::init()
{
    BLEDevice::init(SDTSystem::getInstance()->deviceName().c_str());

    bleServer = BLEDevice::createServer();
    bleServer->setCallbacks(this);

    bleService = bleServer->createService(BLE_UUID);

    bleTxCharacteristic = bleService->createCharacteristic(
        CHARACTERISTIC_UUID_TX,BLECharacteristic::PROPERTY_NOTIFY);
    // bleTxCharacteristic->addDescriptor(new BLE2902());

    bleRxCharacteristic = bleService->createCharacteristic(
        CHARACTERISTIC_UUID_RX,BLECharacteristic::PROPERTY_WRITE);

    bleRxCharacteristic->setCallbacks(this);

    BLEAdvertising *pAdvertising = bleServer->getAdvertising();
    BLEAdvertisementData oAdvertisementCustom = BLEAdvertisementData();

    String manufacturer = "YBY0906";
    manufacturer += SDTSystem::getInstance()->deviceModel();
    oAdvertisementCustom.setManufacturerData(manufacturer.c_str());
    pAdvertising->setAdvertisementData(oAdvertisementCustom);

    return true;
}

void BleManager::onWrite(BLECharacteristic *pCharacteristic)
{
    std::string res = pCharacteristic->getValue();
    this->receivedData(res);
}

void BleManager::start()
{
    bleService->start();
    bleServer->getAdvertising()->start();
}

void BleManager::stop()
{
    // bleService->stop();
    bleServer->getAdvertising()->stop();
}

void BleManager::free()
{

    BLEDevice::deinit(true);
    delete bleTxCharacteristic;
    delete bleRxCharacteristic;
    bleServer->removeService(bleService);
    delete bleService;
    // delete bleServer;
    
}

void BleManager::sendData(std::string& data)
{
    bleTxCharacteristic->setValue(data);
    bleTxCharacteristic->notify();
}

void BleManager::sendData(uint8_t* data, size_t length)
{
    uint32_t sendLength = 20;
    uint32_t dataPointer = 0;
    while (sendLength>0)
    {
        if (length>20)
        {
            sendLength = 20;
        }else
        {
            sendLength = length;
        }
        length -= sendLength;
        bleTxCharacteristic->setValue(data+dataPointer,sendLength);
        bleTxCharacteristic->notify();
        delay(25);
        dataPointer += sendLength;
        if (length == 0)
        {
            break;
        }
    };
}

void BleManager::receivedData(std::string& data)
{
    if(_delegate)
    {
        _delegate->dataParse(data,this);
    }
}


#endif