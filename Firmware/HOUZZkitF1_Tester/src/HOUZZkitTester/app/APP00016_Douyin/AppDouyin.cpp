#include "AppDouyin.h"
#include "AppDouyin32016.h"
#include "AppDouyin3208.h"
#include "AppDouyin12012.h"
#include "AppDouyin808.h"
#include "AppDouyin16016.h"


NS_DT_BEGIN

AppDataDouyin::AppDataDouyin()
:uid("")
,fansCount(0)
{
    scheduleType = AppDataBase::ScheduleType::SCHEDULE;
    scheduleInterval = 15 * 60; //强制每15分钟更新一次
    
    appAlias = "抖音粉丝数";
    canPlay = true;
    openPlay = true;
    isActive = false;
    
    this->setTheme(theme);
}

bool AppDataDouyin::subEncode(SDTData::DataSourceType type)
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

bool AppDataDouyin::subDecode(SDTData::DataSourceType type)
{
    popByte();
    isCountFormat = popBit();
    hideIcon = popBit();
    this->setTheme(popUint8());
    scrollType = popUint8();
    uid = popString8().c_str();
    return !decodeError();
}

bool AppDataDouyin::detectActive()
{
    isActive = false;
    if (uid.length()>5) {
        isActive = true;
    }
    return isActive;
}

void AppDataDouyin::setTheme(uint8_t t)
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


bool AppRequestDouyin::parseRequest(const String& res)
{
    String p = substringBetween(res,"\"follower_count\":",",");
    this->getData()->fansCount = p.toInt();
    return true;
}

void AppRequestDouyin::scheduleAction(float dt)
{
    if (!this->getData()->isActive)
    {
        return;
    }
    String url = "https://www.iesdouyin.com/web/api/v2/user/info/?sec_uid=";
    url += this->getData()->uid;
    this->pushRequestTask(url,true,this,AppScheduleBase::HandleRequestCallback(&AppRequestDouyin::parseRequest));
}

bool AppDouyin::init()
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
        rootLayer = AppDouyinLayer3208::createWithApp(this,Size(32,8));
    }
        break;
    case AppDataBase::AppSizeType::S16016:
    {
        rootLayer = AppDouyinLayer16016::createWithApp(this,Size(16,16));
    }
        break;
    case AppDataBase::AppSizeType::S808:
    {
        rootLayer = AppDouyinLayer808::createWithApp(this,Size(8,8));
    }
        break;
    case AppDataBase::AppSizeType::S12012:
    {
        rootLayer = AppDouyinLayer12012::createWithApp(this,Size(12,12));
    }
        break;
    case AppDataBase::AppSizeType::S32016:
    {
        rootLayer = AppDouyinLayer32016::createWithApp(this,Size(32,16));
    }
        break;
    default:
    {
        rootLayer = AppDouyinLayer::createWithApp(this,Size::ZERO);
    }
        break;
    }
    this->addChild(rootLayer);
    return true;
}


bool AppDouyinLayer::initLayer()
{
    return true;
}

NS_DT_END
