#include "AppTimeOfLife.h"
#include "AppTimeOfLife32016.h"
#include "AppTimeOfLife808.h"
#include "AppTimeOfLife12012.h"
#include "AppTimeOfLife16016.h"
#include "AppTimeOfLife3208.h"
#include "HOUZZkitTester/SDTSystem.h"
#include <ctime>
NS_DT_BEGIN

AppDataTimeOfLife::AppDataTimeOfLife()
:_theme(0)
,year(2021)
,month(10)
,day(1)
,hour(10)
,minute(0)
{
    scheduleType = AppDataBase::ScheduleType::NONE;
    scheduleInterval = 0;

    appAlias = "人生时间";
    canPlay = true;
    openPlay = true;
    isActive = true;
    this->setTheme(_theme);
}

bool AppDataTimeOfLife::subEncode(SDTData::DataSourceType type)
{
    pushUint16(year);
    pushUint8(month);
    pushUint8(day);
    pushUint8(hour);
    pushUint8(minute);
    pushUint8(_theme);
    return true;
}

bool AppDataTimeOfLife::subDecode(SDTData::DataSourceType type)
{
    year = popUint16();
    month = popUint8();
    day = popUint8();
    hour = popUint8();
    minute = popUint8();
    this->setTheme(popUint8());
    return true;
}

void AppDataTimeOfLife::setTheme(uint8_t t)
{
    _theme = t;
    switch (t) {
        case 0:
            appTheme.copyTheme(*(AppManager::getInstance()->globalAppTheme));
            break;
        default:
            AppManager::getInstance()->setTheme((AppManager::GlobalAppThemeType)t,&appTheme);
            break;
    }
}


bool AppScheduleTimeOfLife::parseRequest(const String& res)
{
    return true;
}

void AppScheduleTimeOfLife::scheduleAction(float dt)
{
    
}


bool AppTimeOfLife::init()
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
        rootLayer = AppTimeOfLifeLayer3208::createWithApp(this,Size(32,8));
    }
        break;
    case AppDataBase::AppSizeType::S16016:
    {
        rootLayer = AppTimeOfLifeLayer16016::createWithApp(this,Size(16,16));
    }
        break;
    case AppDataBase::AppSizeType::S808:
    {
        rootLayer = AppTimeOfLifeLayer808::createWithApp(this,Size(8,8));
    }
        break;
    case AppDataBase::AppSizeType::S12012:
    {
        rootLayer = AppTimeOfLifeLayer12012::createWithApp(this,Size(12,12));
    }
        break;
    case AppDataBase::AppSizeType::S32016:
    {
        rootLayer = AppTimeOfLifeLayer32016::createWithApp(this,Size(32,16));
    }
        break;
    default:
    {
        rootLayer = AppTimeOfLifeLayer::createWithApp(this,Size::ZERO);
    }
        break;
    }
    this->addChild(rootLayer);
    return true;
}


void AppTimeOfLife::timeLife(uint16_t *days,uint8_t *hours,uint8_t *minutes)
{
    DateTime now = SDTSystem::getInstance()->now();
    struct std::tm a = {0,this->getData()->minute,this->getData()->hour,this->getData()->day,this->getData()->month,this->getData()->year-1900}; /* June 24, 2004 */
    struct std::tm b = {0,now.minute(),now.hour(),now.day(),now.month(),now.year()-1900}; /* July 5, 2004 */
    std::time_t x = std::mktime(&a);
    std::time_t y = std::mktime(&b);
    *days = 0;
    *hours = 0;
    *minutes = 0;
    if ( x != (std::time_t)(-1) && y != (std::time_t)(-1) )
    {
        double difference = std::difftime(y, x) / (60 * 60 * 24);
        *days = difference + 1;
        *hours = (uint32_t)std::difftime(y, x) % (60 * 60 * 24) / (60 * 60);
        *minutes = (uint32_t)std::difftime(y, x) % (60 * 60) / ( 60);
    }
}


bool AppTimeOfLifeLayer::initLayer()
{
    return true;
}

NS_DT_END
