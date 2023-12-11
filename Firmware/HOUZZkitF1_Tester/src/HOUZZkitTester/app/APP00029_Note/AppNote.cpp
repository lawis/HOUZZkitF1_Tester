#include "AppNote.h"
#include "AppNote3208.h"


NS_DT_BEGIN

AppDataNote::AppDataNote()
:_theme(0)
,message("天天快乐")
,scrollType(0)
,alignment(0)
,offsetX(0)
,hasRefresh(true)
,scrollForward(true)
{
    scheduleType = AppDataBase::ScheduleType::NONE;
    scheduleInterval = 0;

    appAlias = "便签";
    canPlay = true;
    openPlay = true;
    isActive = true;
    this->setTheme(_theme);
}

bool AppDataNote::subEncode(SDTData::DataSourceType type)
{
    pushUint8(_theme);
    pushString8(message.c_str());
    pushUint8(scrollType);
    pushUint8(alignment);
    return true;
}

bool AppDataNote::subDecode(SDTData::DataSourceType type)
{
    this->setTheme(popUint8());
    message = popString8().c_str();
    scrollType = popUint8();
    alignment = popUint8();
    hasRefresh = true;
    return true;
}


void AppDataNote::setTheme(uint8_t t)
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


bool AppScheduleNote::parseRequest(const String& res)
{
    return true;
}

void AppScheduleNote::scheduleAction(float dt)
{
    
}


bool AppNote::init()
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
        rootLayer = AppNoteLayer3208::createWithApp(this,Size(32,8));
    }
        break;
    default:
    {
        rootLayer = AppNoteLayer::createWithApp(this,Size::ZERO);
    }
        break;
    }
    this->addChild(rootLayer);
    return true;
}


bool AppNoteLayer::initLayer()
{
    return true;
}

NS_DT_END