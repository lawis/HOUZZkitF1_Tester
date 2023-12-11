#include "AppClock.h"
#include "AppClock16016.h"
#include "AppClock808.h"
#include "AppClock3208.h"
#include "HOUZZkitTester/SDTSystem.h"

NS_DT_BEGIN

AppDataClock::AppDataClock()
:_theme(0)
,isShowDate(true)
,isShowWeek(true)
,isShowSecond(true)
,is24HourView(true)
,isSundayFirstday(false)
,dayFormat(1)
,timeShowDuration(5)
,dateShowDuration(5)
{   
    appAlias = "时钟";
    canPlay = true;
    openPlay = true;
    isActive = true;
    this->setTheme(_theme);
}


bool AppDataClock::subEncode(SDTData::DataSourceType type)
{
    SDTSystem *sdtSystem = SDTSystem::getInstance();
    enByteBegin();
    pushBit(isShowDate);
    pushBit(isShowWeek);
    pushBit(isShowSecond);
    pushBit(is24HourView);
    pushBit(isSundayFirstday);
    pushBit(sdtSystem->isAutoZone());
    pushByte();
    pushInt8(sdtSystem->manualZone());
    pushInt8(sdtSystem->minuteOffset());
    pushInt8(dayFormat);
    pushUint8(_theme);
    pushUint8(timeShowDuration);
    pushUint8(dateShowDuration);

    // LOG_INT(timeShowDuration);
    // LOG_INT(dateShowDuration);
    return true;
}

bool AppDataClock::subDecode(SDTData::DataSourceType type)
{
    SDTSystem *sdtSystem = SDTSystem::getInstance();
    popByte();
    isShowDate = popBit();
    isShowWeek = popBit();
    isShowSecond = popBit();
    is24HourView = popBit();
    isSundayFirstday = popBit();
    sdtSystem->setIsAutoZone(popBit());
    sdtSystem->setManualZone(popInt8());
    sdtSystem->setMinuteOffset(popInt8());
    dayFormat = popInt8();
    this->setTheme(popUint8());
    timeShowDuration = popUint8(5);
    dateShowDuration = popUint8(5);
    // LOG_STR(appAlias);
    // LOG_INT(showDuration);
    // LOG_INT(appSizeType);
    // LOG_INT(appAlignType);
    // LOG_BOOL(isShowDate);
    // LOG_BOOL(isShowWeek);
    // LOG_BOOL(isShowSecond);
    // LOG_BOOL(is24HourView);
    // LOG_BOOL(isSundayFirstday);
    // LOG_BOOL(isAutoZone);
    // LOG_INT(manualZone);
    // LOG_INT(minuteOffset);
    // LOG_INT(dayFormat);
    // LOG_INT(_theme);
    return true;
}



void AppDataClock::setTheme(uint8_t t)
{
    _theme = t;
    switch (t) {
        case 0:
            appTheme.copyTheme(*(AppManager::getInstance()->globalAppTheme));
            appTheme.weekBgColor = appTheme.bgColor;
            appTheme.weekFgColor = appTheme.fgColor;
            break;
        default:
            AppManager::getInstance()->setTheme((AppManager::GlobalAppThemeType)t,&appTheme);
            appTheme.weekBgColor = appTheme.bgColor;
            appTheme.weekFgColor = appTheme.fgColor;
            break;
    }
}


bool AppScheduleClock::parseRequest(const String& res)
{
    return true;
}

void AppScheduleClock::scheduleAction(float dt)
{

}

bool AppClock::init()
{
    AppDataBase::AppSizeType s = this->getData()->appSizeType;
    if(s == AppDataBase::AppSizeType::AUTO)
    {
        s = this->getAppSizeTypeByScreen();
    }
    switch (s)
    {
     case AppDataBase::AppSizeType::S3208:
     {
         rootLayer = AppClockLayer3208::createWithApp(this,Size(32,8));
     }
         break;
    case AppDataBase::AppSizeType::S16016:
    {
        rootLayer = AppClockLayer16016::createWithApp(this,Size(16,16));
    }
        break;
    case AppDataBase::AppSizeType::S808:
    {
        rootLayer = AppClockLayer808::createWithApp(this,Size(8,8));
    }
        break;
    // case AppDataBase::AppSizeType::S12012:
    // {
        
    // }
    //     break;
    // case AppDataBase::AppSizeType::S32016:
    // {
        
    // }
    //     break;
    default:
    {
        rootLayer = AppClockLayer808::createWithApp(this,Size(8,8));
    }
        break;
    }
    this->addChild(rootLayer);
    return true;
}


DateTime AppClock::now()
{
    return SDTSystem::getInstance()->now();
    // return DEVICE.rtc.now();
}


bool AppClockLayer::initLayer()
{
    return false;
}




NS_DT_END
