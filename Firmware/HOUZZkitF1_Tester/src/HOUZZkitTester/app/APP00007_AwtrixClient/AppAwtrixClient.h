#ifndef __APP_AwtrixClient_H__
#define __APP_AwtrixClient_H__

#include "HOUZZkitTester/app/AppBase.h"
#include "HOUZZkitTester/data/SDTData.h"

NS_DT_BEGIN

#define AppAwtrixClient_APP_ID      7

class AppDataAwtrixClient : public AppDataTemplate<AppDataAwtrixClient,AppAwtrixClient_APP_ID>
{
public:

    String serverIp;

    uint16_t serverPort;

protected:

    bool subEncode(SDTData::DataSourceType type) override;

    bool subDecode(SDTData::DataSourceType type) override;

    bool detectActive() override;

public:

    AppDataAwtrixClient();

};


class AppScheduleAwtrixClient : public AppScheduleTemplate<AppScheduleAwtrixClient,AppDataAwtrixClient,AppAwtrixClient_APP_ID>
{

public:

};


class AppAwtrixClient : public AppTemplate<AppAwtrixClient,AppDataAwtrixClient,AppScheduleAwtrixClient,AppAwtrixClient_APP_ID>
{

protected:

    bool init() override;

public:



};


class AppAwtrixClientLayer : public AppLayerTemplate<AppAwtrixClientLayer,AppAwtrixClient,AppDataAwtrixClient>
{
    
private:


protected:


public:


public:

    bool initLayer() override;
    
};


NS_DT_END

#endif //__APP_AwtrixClient_H__

