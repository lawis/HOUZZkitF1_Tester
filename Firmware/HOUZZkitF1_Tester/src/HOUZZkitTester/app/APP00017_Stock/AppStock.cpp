#include "AppStock.h"
#include "AppStock808.h"
#include "AppStock32016.h"
#include "AppStock12012.h"
#include "AppStock16016.h"
#include "AppStock3208.h"


NS_DT_BEGIN

AppDataStock::AppDataStock()
:isShowStockCode(true)
,stockCode("sh000001")
,symbolName("SZ")
,price(0.0)
{
    scheduleType = AppDataBase::ScheduleType::SCHEDULE;
    scheduleInterval = 1 * 60; //强制每分钟更新一次
    
    appAlias = "上证指数";
    canPlay = true;
    openPlay = true;
    isActive = true;
    
    this->setTheme(theme);
}

bool AppDataStock::subEncode(SDTData::DataSourceType type)
{
    enByteBegin();
    pushBit(isShowStockCode);
    pushBit(hideIcon);
    pushByte();
    pushUint8(scrollType);
    pushString8(stockCode.c_str());
    pushString8(symbolName.c_str());
    pushUint8(theme);
    return true;
}

bool AppDataStock::subDecode(SDTData::DataSourceType type)
{
    popByte();
    isShowStockCode = popBit();
    hideIcon = popBit();
    scrollType = popUint8();
    stockCode = popString8().c_str();
    symbolName = popString8().c_str();
    this->setTheme(popUint8());
    return !decodeError();
}

void AppDataStock::setTheme(uint8_t t)
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


bool AppScheduleStock::parseRequest(const String& res)
{
    uint16_t index = res.indexOf(",");
    index = res.indexOf(",",index+1);
    index = res.indexOf(",",index+1);
    uint16_t lastIndex = res.indexOf(",",index+1);
    String p = res.substring(index+1, lastIndex);
    this->getData()->price = p.toFloat();
    return true;
}

void AppScheduleStock::scheduleAction(float dt)
{
    String url = "https://hq.sinajs.cn/list=";
    url += this->getData()->stockCode;
    this->pushRequestTask(url,true,this,AppScheduleBase::HandleRequestCallback(&AppScheduleStock::parseRequest));
}


bool AppStock::init()
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
        rootLayer = AppStockLayer3208::createWithApp(this,Size(32,8));
    }
        break;
    case AppDataBase::AppSizeType::S16016:
    {
        rootLayer = AppStockLayer16016::createWithApp(this,Size(16,16));
    }
        break;
    case AppDataBase::AppSizeType::S808:
    {
        rootLayer = AppStockLayer808::createWithApp(this,Size(8,8));
    }
        break;
    case AppDataBase::AppSizeType::S12012:
    {
        rootLayer = AppStockLayer12012::createWithApp(this,Size(12,12));
    }
        break;
    case AppDataBase::AppSizeType::S32016:
    {
        rootLayer = AppStockLayer32016::createWithApp(this,Size(32,16));
    }
        break;
    default:
    {
        rootLayer = AppStockLayer::createWithApp(this,Size::ZERO);
    }
        break;
    }
    this->addChild(rootLayer);
    return true;
}


bool AppStockLayer::initLayer()
{
    return true;
}

NS_DT_END
