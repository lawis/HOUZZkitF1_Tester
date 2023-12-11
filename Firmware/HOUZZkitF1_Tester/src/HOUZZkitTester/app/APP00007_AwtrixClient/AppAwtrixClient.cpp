#include "AppAwtrixClient.h"
#include "AppAwtrixClient3208.h"

NS_DT_BEGIN

AppDataAwtrixClient::AppDataAwtrixClient()
:serverIp("")
,serverPort(7001)
{
    scheduleType = AppDataBase::ScheduleType::NONE;
    scheduleInterval = 0;

    canPlay = true;
    openPlay = true;
    isActive = false;

    appAlias = "Awtrix模拟器";
}

bool AppDataAwtrixClient::subEncode(SDTData::DataSourceType type)
{
    pushString8(serverIp.c_str());
    pushUint16(serverPort);
    return true;
}

bool AppDataAwtrixClient::subDecode(SDTData::DataSourceType type)
{
    serverIp = popString8().c_str();
    serverPort = popUint16();
    return true;
}

bool AppDataAwtrixClient::detectActive()
{
    isActive = false;
    if (serverIp.length()>5) {
        isActive = true;
    }
    return isActive;
}

bool AppAwtrixClient::init()
{
    AppDataBase::AppSizeType type = this->getData()->appSizeType;
    if (type == AppDataBase::AppSizeType::AUTO)
    {
        type = this->getAppSizeTypeByScreen();
    }
    rootLayer = AppAwtrixClientLayer3208::createWithApp(this,Size(32,8));
    this->addChild(rootLayer);
    return true;
}


bool AppAwtrixClientLayer::initLayer()
{
    return true;
}

NS_DT_END