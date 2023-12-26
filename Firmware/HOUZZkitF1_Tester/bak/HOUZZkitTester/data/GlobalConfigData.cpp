#include "GlobalConfigData.h"

GlobalConfigData::GlobalConfigData()
:isAutoZone(true)
,manualZone(8)
,autoZone(8)
,minuteOffset(0)
,btSpeakerEnable(true)
,autoBrighten(false)
,brightness(80)
,volume(15)
,theme(0)
,mqttServerIp("")
,mqttServerPort(0)
{

}

bool GlobalConfigData::encode(SDTData::DataSourceType type)
{
    if (type == SDTData::DataSourceType::Net)
    {
        
    }else
    {
        enByteBegin();
        pushBit(isAutoZone);
        pushBit(btSpeakerEnable);
        pushBit(autoBrighten);
        pushByte();
        pushInt8(manualZone);
        pushInt8(autoZone);
        pushInt8(minuteOffset);
        pushUint8(brightness);
        pushUint8(volume);
        pushUint8(theme);
        pushString8(mqttServerIp.c_str());
        pushUint16(mqttServerPort);
    }
    return true;
}

bool GlobalConfigData::decode(SDTData::DataSourceType type)
{
    if (type == SDTData::DataSourceType::Net)
    {
        
    }else
    {
        popByte();
        isAutoZone = popBit();
        btSpeakerEnable = popBit();
        autoBrighten = popBit();
        manualZone = popInt8();
        autoZone = popInt8();
        minuteOffset = popInt8();
        brightness = popUint8();
        volume = popUint8();
        theme = popUint8();
        mqttServerIp = popString8().c_str();
        mqttServerPort = popUint16();
    }
    return true;
}