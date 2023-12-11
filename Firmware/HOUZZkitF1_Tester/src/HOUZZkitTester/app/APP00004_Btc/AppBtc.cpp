#include "AppBtc.h"
#include "AppBtc12012.h"
#include "AppBtc808.h"
#include "AppBtc3208.h"
#include "AppBtc16016.h"
#include "AppBtc32016.h"

NS_DT_BEGIN

AppDataBtc::AppDataBtc()
:isShowCoinName(true)
,coinName("bitcoin")
,symbolName("BTC")
,price(0.0)
{
    scheduleType = AppDataBase::ScheduleType::SCHEDULE;
    scheduleInterval = 1 * 60; //强制每分钟更新一次
    
    appAlias = "比特币实时价格";
    canPlay = true;
    openPlay = true;
    isActive = true;
    
    this->setTheme(theme);
}

bool AppDataBtc::subEncode(SDTData::DataSourceType type)
{
    enByteBegin();
    pushBit(isShowCoinName);
    pushBit(hideIcon);
    pushByte();
    pushUint8(theme);
    pushUint8(scrollType);
    pushString8(coinName.c_str());
    return true;
}

bool AppDataBtc::subDecode(SDTData::DataSourceType type)
{
    popByte();
    isShowCoinName = popBit();
    hideIcon = popBit();
    this->setTheme(popUint8());
    scrollType = popUint8();
    coinName = popString8().c_str();
    return !decodeError();
}

void AppDataBtc::setTheme(uint8_t t)
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


bool AppRequestBtc::parseRequest(const String& res)
{
    uint32_t size = res.length();
    DynamicJsonDocument json(size+256);
    DeserializationError error = deserializeJson(json, res);
    if (error) {
        LOG("ERROR!!! AppBtc Parse Failed! ");
        return false; 
    }
    this->getData()->price = json["data"]["priceUsd"].as<float>();
    this->getData()->symbolName = json["data"]["symbol"].as<String>();
    return true;
}

void AppRequestBtc::scheduleAction(float dt)
{
    String url = "https://api.coincap.io/v2/assets/";
    url += this->getData()->coinName;
    this->pushRequestTask(url,true,this,AppScheduleBase::HandleRequestCallback(&AppRequestBtc::parseRequest));
}


bool AppBtc::init()
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
        rootLayer = AppBtcLayer3208::createWithApp(this,Size(32,8));
    }
        break;
    case AppDataBase::AppSizeType::S16016:
    {
        rootLayer = AppBtcLayer16016::createWithApp(this,Size(16,16));
    }
        break;
    case AppDataBase::AppSizeType::S808:
    {
        rootLayer = AppBtcLayer808::createWithApp(this,Size(8,8));
    }
        break;
    case AppDataBase::AppSizeType::S12012:
    {
        rootLayer = AppBtcLayer12012::createWithApp(this,Size(12,12));
    }
        break;
    case AppDataBase::AppSizeType::S32016:
    {
        rootLayer = AppBtcLayer32016::createWithApp(this,Size(32,16));
    }
        break;
    default:
    {
        rootLayer = AppBtcLayer::createWithApp(this,Size::ZERO);
    }
        break;
    }
    this->addChild(rootLayer);
    return true;
}


bool AppBtcLayer::initLayer()
{
    return true;
}

NS_DT_END
