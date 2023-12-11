#include "AppDayNumOfYear.h"
#include "AppDayNumOfYear12012.h"
#include "AppDayNumOfYear16016.h"
#include "AppDayNumOfYear32016.h"
#include "AppDayNumOfYear808.h"
#include "AppDayNumOfYear3208.h"
#include "HOUZZkitTester/SDTSystem.h"

NS_DT_BEGIN

AppDataDayNumOfYear::AppDataDayNumOfYear()
{
    scheduleType = AppDataBase::ScheduleType::NONE;
    scheduleInterval = 0;

    appAlias = "一年中的第几天";
    canPlay = true;
    openPlay = true;
    isActive = true;

    this->setTheme(theme);
}

bool AppDataDayNumOfYear::subEncode(SDTData::DataSourceType type)
{
    pushUint8(theme);
    enByteBegin();
    pushBit(showAsPercent);
    pushByte();
    return true;
}

bool AppDataDayNumOfYear::subDecode(SDTData::DataSourceType type)
{
    this->setTheme(popUint8());
    popByte();
    showAsPercent = popBit();
    return true;
}

void AppDataDayNumOfYear::setTheme(uint8_t t)
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

void AppScheduleDayNumOfYear::scheduleAction(float dt)
{
    
}


bool AppDayNumOfYear::init()
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
        rootLayer = AppDayNumOfYearLayer3208::createWithApp(this,Size(32,8));
    }
        break;
    case AppDataBase::AppSizeType::S16016:
    {
        rootLayer = AppDayNumOfYearLayer16016::createWithApp(this,Size(16,16));
    }
        break;
    case AppDataBase::AppSizeType::S808:
    {
        rootLayer = AppDayNumOfYearLayer808::createWithApp(this,Size(8,8));
    }
        break;
    case AppDataBase::AppSizeType::S12012:
    {
        rootLayer = AppDayNumOfYearLayer12012::createWithApp(this,Size(12,12));
    }
        break;
    case AppDataBase::AppSizeType::S32016:
    {
        rootLayer = AppDayNumOfYearLayer32016::createWithApp(this,Size(32,16));
    }
        break;
    default:
    {
        rootLayer = AppDayNumOfYearLayer::createWithApp(this,Size::ZERO);
    }
        break;
    }
    this->addChild(rootLayer);
    return true;
}

uint16_t AppDayNumOfYear::dayNum()
{
    DateTime now = SDTSystem::getInstance()->now();
    DateTime today = DateTime(now.year(), now.month(), now.day());
    DateTime firstDayOfYear = DateTime(now.year(), 1, 1);
    TimeSpan diff = TimeSpan(today.unixtime() - firstDayOfYear.unixtime());
    int16_t diffDay = diff.days();
    return diffDay + 1;
}

bool AppDayNumOfYear::leapYear()
{
    DateTime now = SDTSystem::getInstance()->now();
    uint16_t year = now.year();
    if (year % 100 == 0)
    {
        if (year % 400 == 0)
        {
            return true;
        }else
        {
            return false;
        }
    }else if(year % 4 == 0)
    {
        return true;
    }
    return false;
}

float AppDayNumOfYear::percentOfYear(float* linePercent,float* dotPercent)
{
    float daysOfWholeYear = leapYear()?366.0f:365.0f;
    uint16_t dayNumOfYear = dayNum()-1;

    DateTime now = SDTSystem::getInstance()->now();
    uint32_t secondsOfDay = now.hour()*60*60;
    *linePercent = now.minute() / 60.0f;
    *dotPercent = (now.minute() % 3 * 60 + now.second()) / 180.0f;
    return dayNumOfYear / daysOfWholeYear + secondsOfDay / 86400.0f /daysOfWholeYear; 
}



bool AppDayNumOfYearLayer::initLayer()
{
    return true;
}

NS_DT_END
