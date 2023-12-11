#include "AppAlarm.h"
#include "AppAlarm12012.h"
#include "AppAlarm16016.h"
#include "AppAlarm32016.h"
#include "AppAlarm808.h"
#include "AppAlarm3208.h"
#include "HOUZZkitTester/SDTSystem.h"

NS_DT_BEGIN

AppDataAlarm::AppDataAlarm()
:enable(false)
,hour(0)
,minute(0)
,musicPath("alarm.mp3")
,alarming(false)
{
    scheduleType = AppDataBase::ScheduleType::SCHEDULE;
    scheduleInterval = 1;

    appAlias = "闹铃";
    canPlay = true;
    openPlay = true;
    isActive = false;

    this->setTheme(theme);
}

bool AppDataAlarm::subEncode(SDTData::DataSourceType type)
{
    enByteBegin();
    pushBit(enable);
    pushByte();
    pushUint8(hour);
    pushUint8(minute);
    pushString8(musicPath.c_str());
    pushUint8(theme);
    return true;
}

bool AppDataAlarm::subDecode(SDTData::DataSourceType type)
{
    popByte();
    enable = popBit();
    hour = popUint8();
    minute = popUint8();
    musicPath = popString8().c_str();
    this->setTheme(popUint8());
    return !decodeError();
}

bool AppDataAlarm::detectActive()
{
    isActive = false;
    if (enable) {
        isActive = true;
    }
    return isActive;
}

void AppDataAlarm::setTheme(uint8_t t)
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

bool AppScheduleAlarm::init()
{
    auto listener = EventListenerButton::create();
    listener ->onBtnClick = [&](int8_t keyCode , Event * event ){
        // LOG("onBtnClick keyCode:%d",keyCode);
        if (this->getData()->alarming)
        {
            //停止播放音乐
#ifndef TARGET_OS_MAC
            DEVICE.tfCardStop();
#else
            LOG("停止播放音乐");
#endif
        }
        
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority ( listener, this );
    return true;
}

void AppScheduleAlarm::scheduleAction(float dt)
{
    DateTime now = SDTSystem::getInstance()->now();
    if (now.hour() == this->getData()->hour &&
        now.minute() == this->getData()->minute )
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


bool AppAlarm::init()
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
        rootLayer = AppAlarmLayer3208::createWithApp(this,Size(32,8));
    }
        break;
    case AppDataBase::AppSizeType::S16016:
    {
        rootLayer = AppAlarmLayer16016::createWithApp(this,Size(16,16));
    }
        break;
    case AppDataBase::AppSizeType::S808:
    {
        rootLayer = AppAlarmLayer808::createWithApp(this,Size(8,8));
    }
        break;
    case AppDataBase::AppSizeType::S12012:
    {
        rootLayer = AppAlarmLayer12012::createWithApp(this,Size(12,12));
    }
        break;
    case AppDataBase::AppSizeType::S32016:
    {
        rootLayer = AppAlarmLayer32016::createWithApp(this,Size(32,16));
    }
        break;
    default:
    {
        rootLayer = AppAlarmLayer::createWithApp(this,Size::ZERO);
    }
        break;
    }
    this->addChild(rootLayer);
    return true;
}

DateTime AppAlarm::now()
{
    return SDTSystem::getInstance()->now();
}

bool AppAlarmLayer::initLayer()
{
    return true;
}

NS_DT_END
