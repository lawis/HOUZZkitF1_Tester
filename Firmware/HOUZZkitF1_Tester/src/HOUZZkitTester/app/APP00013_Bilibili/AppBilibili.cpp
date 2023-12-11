#include "AppBilibili.h"
#include "AppBilibili3208.h"
#include "AppBilibili32016.h"
#include "AppBilibili808.h"
#include "AppBilibili16016.h"
#include "AppBilibili12012.h"

NS_DT_BEGIN

AppDataBilibili::AppDataBilibili()
:uid("")
,fansCount(0)
{
    scheduleType = AppDataBase::ScheduleType::SCHEDULE;
    scheduleInterval = 15 * 60; //强制每15分钟更新一次
    
    appAlias = "Bilibili粉丝数";
    canPlay = true;
    openPlay = true;
    isActive = false;
    
    this->setTheme(theme);
}

bool AppDataBilibili::subEncode(SDTData::DataSourceType type)
{
    enByteBegin();
    pushBit(isCountFormat);
    pushBit(hideIcon);
    pushByte();
    pushUint8(theme);
    pushUint8(scrollType);
    pushString8(uid.c_str());
    return true;
}

bool AppDataBilibili::subDecode(SDTData::DataSourceType type)
{
    popByte();
    isCountFormat = popBit();
    hideIcon = popBit();
    this->setTheme(popUint8());
    scrollType = popUint8();
    uid = popString8().c_str();
    return !decodeError();
}

bool AppDataBilibili::detectActive()
{
    isActive = false;
    if (uid.length()>5) {
        isActive = true;
    }
    return isActive;
}

void AppDataBilibili::setTheme(uint8_t t)
{
    theme = t;
    switch (t) {
        case 0:
            appTheme.copyTheme(*(AppManager::getInstance()->globalAppTheme));
            break;
        default:
            AppManager::getInstance()->setTheme((AppManager::GlobalAppThemeType)t,&appTheme);
            break;
    }
}

bool AppRequestBilibili::parseRequest(const String& res)
{
    uint32_t size = res.length();
    DynamicJsonDocument json(size+256);
    DeserializationError error = deserializeJson(json, res);
    if (error) {
        LOG("ERROR!!! AppBilibili Parse Failed! ");
        return false; 
    }
    this->getData()->fansCount = json["data"]["follower"].as<uint32_t>();
    return true;
}

void AppRequestBilibili::scheduleAction(float dt)
{
    if (!this->getData()->isActive)
    {
        return;
    }
    String url = "https://api.bilibili.com/x/relation/stat?vmid=";
    url += this->getData()->uid;
    this->pushRequestTask(url,true,this,AppScheduleBase::HandleRequestCallback(&AppRequestBilibili::parseRequest));
}


bool AppBilibili::init()
{
    AppDataBase::AppSizeType type = this->getData()->appSizeType;
    if (type == AppDataBase::AppSizeType::AUTO)
    {
        type = this->getAppSizeTypeByScreen();
    }
    switch (type)
    {
    case AppDataBase::AppSizeType::S3208:
    {
        rootLayer = AppBilibiliLayer3208::createWithApp(this,Size(32,8));
    }
        break;
    case AppDataBase::AppSizeType::S16016:
    {
        rootLayer = AppBilibiliLayer16016::createWithApp(this,Size(16,16));
    }
        break;
    case AppDataBase::AppSizeType::S808:
    {
        rootLayer = AppBilibiliLayer808::createWithApp(this,Size(8,8));
    }
        break;
    case AppDataBase::AppSizeType::S12012:
    {
        rootLayer = AppBilibiliLayer12012::createWithApp(this,Size(12,12));
    }
        break;
    case AppDataBase::AppSizeType::S32016:
    {
        rootLayer = AppBilibiliLayer32016::createWithApp(this,Size(32,16));
    }
        break;
    default:
    {
        rootLayer = AppBilibiliLayer::createWithApp(this,Size::ZERO);
    }
        break;
    }
    this->addChild(rootLayer);
    return true;
}


bool AppBilibiliLayer::initLayer()
{
    return true;
}

NS_DT_END
