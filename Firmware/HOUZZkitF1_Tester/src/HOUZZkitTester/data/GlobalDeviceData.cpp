#include "GlobalDeviceData.h"
#include "HOUZZkitTester/SDTConfig.h"
#ifndef TARGET_OS_MAC
#include "esp_efuse.h"
#endif

GlobalDeviceData::GlobalDeviceData()
:newDevice(true)
{
#ifndef TARGET_OS_MAC
    uint8_t efuseByte[16];
    esp_err_t err = esp_efuse_read_block(EFUSE_BLK3,efuseByte,8,16*8);
    if (err != ESP_OK)
    {
        Serial.println("---ERROR!!!---UDID Read Failed.\n");
    }
    char udidStr[37] = { 0 };
    sprintf(udidStr, "%02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X%02X"
    , efuseByte[ 0], efuseByte[ 1], efuseByte[ 2], efuseByte[ 3]
    , efuseByte[ 4], efuseByte[ 5], efuseByte[ 6], efuseByte[ 7]
    , efuseByte[ 8], efuseByte[ 9], efuseByte[10], efuseByte[11]
    , efuseByte[12], efuseByte[13], efuseByte[14], efuseByte[15]);
    UDID = udidStr;
    deviceModel = this->getDeviceModel();
    deviceName = this->getDefaultName();
#endif
}

String GlobalDeviceData::getDeviceModel()
{
#ifndef TARGET_OS_MAC
    // String deviceModel;
    uint8_t efuseByte[6];
    esp_err_t err = esp_efuse_read_block(EFUSE_BLK3,efuseByte,8*17,6*8);
    if (err != ESP_OK)
    {
        Serial.println("---ERROR!!!---UDID Read Failed.\n");
    }
    char seriesName[5] = { 0 };
    memcpy(seriesName,efuseByte,4);
    uint16_t seriesNum =  GetUint16(efuseByte+4);
    return String(seriesName) + String(seriesNum);
#else
    return "Simulator";
#endif
}

String GlobalDeviceData::getDefaultName()
{
    String name = DEVICE_NAME_PREFIX;
    name += this->deviceModel;
    return name;
}



bool GlobalDeviceData::encode(SDTData::DataSourceType type)
{
    enByteBegin();
    pushBit(newDevice);
    pushByte();
    pushString8(deviceName.c_str());
    return true;
}

bool GlobalDeviceData::decode(SDTData::DataSourceType type)
{
    popByte();
    newDevice = popBit();
    deviceName = popString8().c_str();
    return decodeError();
}