#include "AppWeibo.h"
#include "AppWeibo32016.h"
#include "AppWeibo808.h"
#include "AppWeibo16016.h"
#include "AppWeibo3208.h"
#include "AppWeibo12012.h"


NS_DT_BEGIN

AppDataWeibo::AppDataWeibo()
:uid("")
,fansCount(0)
{
    scheduleType = AppDataBase::ScheduleType::SCHEDULE;
    scheduleInterval = 15 * 60; //强制每15分钟更新一次
    
    appAlias = "微博粉丝数";
    canPlay = true;
    openPlay = true;
    isActive = false;
    
    this->setTheme(theme);
}

bool AppDataWeibo::subEncode(SDTData::DataSourceType type)
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

bool AppDataWeibo::subDecode(SDTData::DataSourceType type)
{
    popByte();
    isCountFormat = popBit();
    hideIcon = popBit();
    this->setTheme(popUint8());
    scrollType = popUint8();
    uid = popString8().c_str();
    return !decodeError();
}

bool AppDataWeibo::detectActive()
{
    isActive = false;
    if (uid.length()>5) {
        isActive = true;
    }
    return isActive;
}


void AppDataWeibo::setTheme(uint8_t t)
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


bool AppRequestWeibo::parseRequest(const String& res)
{
    String p = substringBetween(res,"\"followers_count\":\"","\",");
    int16_t index = p.indexOf("\\u4e07");
    if (index != -1)
    {
        p.replace("\\u4e07","");
        float count = p.toFloat();
        this->getData()->fansCount = count * 10000;
    }else
    {
        this->getData()->fansCount = p.toInt();
    }
    return true;
}

void AppRequestWeibo::scheduleAction(float dt)
{
    if (!this->getData()->isActive)
    {
        return;
    }
    String url = "https://m.weibo.cn/api/container/getIndex?type=uid&value=";
    url += this->getData()->uid;
    this->pushRequestTask(url,true,this,AppScheduleBase::HandleRequestCallback(&AppRequestWeibo::parseRequest));
}


bool AppWeibo::init()
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
        rootLayer = AppWeiboLayer3208::createWithApp(this,Size(32,8));
    }
        break;
    case AppDataBase::AppSizeType::S16016:
    {
        rootLayer = AppWeiboLayer16016::createWithApp(this,Size(16,16));
    }
        break;
    case AppDataBase::AppSizeType::S808:
    {
        rootLayer = AppWeiboLayer808::createWithApp(this,Size(8,8));
    }
        break;
    case AppDataBase::AppSizeType::S12012:
    {
        rootLayer = AppWeiboLayer12012::createWithApp(this,Size(12,12));
    }
        break;
    case AppDataBase::AppSizeType::S32016:
    {
        rootLayer = AppWeiboLayer32016::createWithApp(this,Size(32,16));
    }
        break;
    default:
    {
        rootLayer = AppWeiboLayer::createWithApp(this,Size::ZERO);
    }
        break;
    }
    this->addChild(rootLayer);
    return true;
}


bool AppWeiboLayer::initLayer()
{
    return true;
}

NS_DT_END
