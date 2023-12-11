#include "GlobalUserData.h"
#include "HOUZZkitTester/SDTConfig.h"
#ifndef TARGET_OS_MAC
#include "esp_efuse.h"
#endif
GlobalUserData::GlobalUserData()
:uninitDevice(true)
,wifiSsid("")
,wifiPwd("")
,token("")
{

}
    

bool GlobalUserData::encode(SDTData::DataSourceType type)
{
    enByteBegin();
    pushBit(uninitDevice);
    pushByte();
    pushString8(wifiSsid.c_str());
    pushString8(wifiPwd.c_str());
    pushString8(token.c_str());
    return true;
}

bool GlobalUserData::decode(SDTData::DataSourceType type)
{
    popByte();
    uninitDevice = popBit();
    wifiSsid = popString8().c_str();
    wifiPwd = popString8().c_str();
    token = popString8().c_str();
    return decodeError();
}