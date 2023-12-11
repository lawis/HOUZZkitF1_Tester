#include "AppYoutube.h"
#include "AppYoutube12012.h"
#include "AppYoutube32016.h"
#include "AppYoutube3208.h"
#include "AppYoutube808.h"
#include "AppYoutube16016.h"

NS_DT_BEGIN

AppDataYoutube::AppDataYoutube()
:channelID("")
,apiKey("")
,fansCount(0)
{
    scheduleType = AppDataBase::ScheduleType::SCHEDULE;
    scheduleInterval = 15 * 60; //强制每15分钟更新一次
    
    appAlias = "YouTube粉丝数";
    canPlay = true;
    openPlay = true;
    isActive = false;
    
    this->setTheme(theme);
}

bool AppDataYoutube::subEncode(SDTData::DataSourceType type)
{
    enByteBegin();
    pushBit(isCountFormat);
    pushBit(hideIcon);
    pushByte();
    pushUint8(theme);
    pushUint8(scrollType);
    pushString8(channelID.c_str());
    pushString8(apiKey.c_str());
    return true;
}

bool AppDataYoutube::subDecode(SDTData::DataSourceType type)
{
    popByte();
    isCountFormat = popBit();
    hideIcon = popBit();
    this->setTheme(popUint8());
    scrollType = popUint8();
    channelID = popString8().c_str();
    apiKey = popString8().c_str();
    return !decodeError();
}

bool AppDataYoutube::detectActive()
{
    isActive = false;
    if (channelID.length()>10 || apiKey.length()>10) {
        isActive = true;
    }
    return isActive;
}


void AppDataYoutube::setTheme(uint8_t t)
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


bool AppRequestYoutube::parseRequest(const String& res)
{
    uint32_t size = res.length();
    DynamicJsonDocument json(size+256);
    DeserializationError error = deserializeJson(json, res);
    if (error) {
        LOG("ERROR!!! AppYoutube Parse Failed! ");
        return false; 
    }
    this->getData()->fansCount = json["items"][0]["statistics"]["subscriberCount"].as<uint32_t>();
    return true;
}

void AppRequestYoutube::scheduleAction(float dt)
{
    if (!this->getData()->isActive)
    {
        return;
    }
    String url = "https://www.googleapis.com/youtube/v3/channels?part=statistics";
    url += "&id=" + this->getData()->channelID + "&key=" + this->getData()->apiKey;
    this->pushRequestTask(url,true,this,AppScheduleBase::HandleRequestCallback(&AppRequestYoutube::parseRequest));
}


bool AppYoutube::init()
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
        rootLayer = AppYoutubeLayer3208::createWithApp(this,Size(32,8));
    }
        break;
    case AppDataBase::AppSizeType::S16016:
    {
        rootLayer = AppYoutubeLayer16016::createWithApp(this,Size(16,16));
    }
        break;
    case AppDataBase::AppSizeType::S808:
    {
        rootLayer = AppYoutubeLayer808::createWithApp(this,Size(8,8));
    }
        break;
    case AppDataBase::AppSizeType::S12012:
    {
        rootLayer = AppYoutubeLayer12012::createWithApp(this,Size(12,12));
    }
        break;
    case AppDataBase::AppSizeType::S32016:
    {
        rootLayer = AppYoutubeLayer32016::createWithApp(this,Size(32,16));
    }
        break;
    default:
    {
        rootLayer = AppYoutubeLayer::createWithApp(this,Size::ZERO);
    }
        break;
    }
    this->addChild(rootLayer);
    return true;
}


bool AppYoutubeLayer::initLayer()
{
    return true;
}

NS_DT_END
