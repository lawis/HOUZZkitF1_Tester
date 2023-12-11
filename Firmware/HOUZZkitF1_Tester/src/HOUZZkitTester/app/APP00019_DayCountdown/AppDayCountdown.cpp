#include "AppDayCountdown.h"
#include "AppDayCountdown12012.h"
#include "AppDayCountdown3208.h"
#include "AppDayCountdown16016.h"
#include "AppDayCountdown32016.h"
#include "AppDayCountdown808.h"
#include "HOUZZkitTester/SDTSystem.h"

NS_DT_BEGIN

AppDataDayCountdown::AppDataDayCountdown()
:enable(false)
,year(2020)
,month(9)
,day(6)
,musicPath("daycd.mp3")
,alarming(false)
{
    scheduleType = AppDataBase::ScheduleType::SCHEDULE;
    scheduleInterval = 1 * 60;

    appAlias = "日期倒计时";
    canPlay = true;
    openPlay = true;
    isActive = false;

    this->setTheme(theme);
}

bool AppDataDayCountdown::subEncode(SDTData::DataSourceType type)
{
    enByteBegin();
    pushBit(enable);
    pushByte();
    pushUint16(year);
    pushUint8(month);
    pushUint8(day);
    pushString8(musicPath.c_str());
    pushUint8(theme);
    return true;
}

bool AppDataDayCountdown::subDecode(SDTData::DataSourceType type)
{
    popByte();
    enable = popBit();
    year = popUint16();
    month = popUint8();
    day = popUint8();
    musicPath = popString8().c_str();
    this->setTheme(popUint8());
    return !decodeError();
}

bool AppDataDayCountdown::detectActive()
{
    isActive = false;
    if (enable) {
        isActive = true;
    }
    return isActive;
}

void AppDataDayCountdown::setTheme(uint8_t t)
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


bool AppScheduleDayCountdown::init()
{
    auto listener = EventListenerButton::create();
    listener ->onBtnClick = [&](int8_t keyCode , Event * event ){
        // LOG("onBtnClick keyCode:%d",keyCode);
        if (this->getData()->alarming)
        {
            //停止播放音乐
#ifndef TARGET_OS_MAC
            DEVICE.bt201.localPlayMode(0);
#else
            LOG("停止播放音乐");
#endif
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this );
    return true;
}

void AppScheduleDayCountdown::scheduleAction(float dt)
{
    DateTime now = SDTSystem::getInstance()->now();
    if (now.year() == this->getData()->year &&
        now.month() == this->getData()->month &&
        now.day() == this->getData()->day &&
        now.hour() >= 8 ) //TODO 用户自主输入几点开始提醒
    {
        if (!this->getData()->alarming)
        {
            this->getData()->alarming = true;
            //播放音乐
#ifndef TARGET_OS_MAC
            String audioPath = "/ALARM/";
            audioPath+= this->getData()->musicPath;
            DEVICE.tfCardPlay(audioPath.c_str());
#else
            LOG("播放音乐");
#endif
        }
    }else
    {
        this->getData()->alarming = false;
    }
}


bool AppDayCountdown::init()
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
        rootLayer = AppDayCountdownLayer3208::createWithApp(this,Size(32,8));
    }
        break;
    case AppDataBase::AppSizeType::S16016:
    {
        rootLayer = AppDayCountdownLayer16016::createWithApp(this,Size(16,16));
    }
        break;
    case AppDataBase::AppSizeType::S808:
    {
        rootLayer = AppDayCountdownLayer808::createWithApp(this,Size(8,8));
    }
        break;
    case AppDataBase::AppSizeType::S12012:
    {
        rootLayer = AppDayCountdownLayer12012::createWithApp(this,Size(12,12));
    }
        break;
    case AppDataBase::AppSizeType::S32016:
    {
        rootLayer = AppDayCountdownLayer32016::createWithApp(this,Size(32,16));
    }
        break;
    default:
    {
        rootLayer = AppDayCountdownLayer::createWithApp(this,Size::ZERO);
    }
        break;
    }
    this->addChild(rootLayer);
    return true;
}

DateTime AppDayCountdown::now()
{
    return SDTSystem::getInstance()->now();
}

bool AppDayCountdownLayer::initLayer()
{
    return true;
}

NS_DT_END
