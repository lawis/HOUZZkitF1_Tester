#ifndef __SDT_GLABAL_DEVICE_DATA_H__
#define __SDT_GLABAL_DEVICE_DATA_H__

#include "SDTData.h"

class GlobalDeviceData : public SDTDataTemplate<GlobalDeviceData>
{
public:

    bool newDevice;

    String UDID;

    String deviceModel;

    String deviceName;

protected:

    bool encode(SDTData::DataSourceType type) override;

    bool decode(SDTData::DataSourceType type) override;

public:

    GlobalDeviceData();

    String getDeviceModel();

    String getDefaultName();

};

#endif //__SDT_GLABAL_DEVICE_DATA_H__
