#ifndef __SDT_GLABAL_USER_DATA_H__
#define __SDT_GLABAL_USER_DATA_H__

#include "SDTData.h"

class GlobalUserData : public SDTDataTemplate<GlobalUserData>
{
public:

    bool uninitDevice;

    String wifiSsid;

    String wifiPwd;

    String token;

protected:

    bool encode(SDTData::DataSourceType type) override;

    bool decode(SDTData::DataSourceType type) override;

public:

    GlobalUserData();

};


#endif //__SDT_GLABAL_USER_DATA_H__
