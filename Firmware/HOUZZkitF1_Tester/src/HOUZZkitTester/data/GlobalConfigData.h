#ifndef __SDT_GLABAL_CONFIG_DATA_H__
#define __SDT_GLABAL_CONFIG_DATA_H__

#include "SDTData.h"

class GlobalConfigData : public SDTDataTemplate<GlobalConfigData>
{
protected:

    bool isAutoZone;

    int8_t manualZone;
    
    int8_t autoZone;

    int8_t minuteOffset;

    friend class SDTSystem;

public:
    
    bool btSpeakerEnable;

    bool autoBrighten;

    uint8_t brightness;    // 0-100

    uint8_t volume;        // 0-30

    uint8_t theme;

    String mqttServerIp;

    uint16_t mqttServerPort;          

protected:

    bool encode(SDTData::DataSourceType type) override;

    bool decode(SDTData::DataSourceType type) override;

public:

    GlobalConfigData();



};

#endif //__SDT_GLABAL_CONFIG_DATA_H__
