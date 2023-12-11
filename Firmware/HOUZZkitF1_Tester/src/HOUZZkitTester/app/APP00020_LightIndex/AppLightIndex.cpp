#include "AppLightIndex.h"
#include "AppLightIndex16016.h"
#include "AppLightIndex808.h"
#include "AppLightIndex32016.h"
#include "AppLightIndex3208.h"
#include "AppLightIndex12012.h"


NS_DT_BEGIN

AppDataLightIndex::AppDataLightIndex()
{
    scheduleType = AppDataBase::ScheduleType::NONE;
    scheduleInterval = 0;

    appAlias = "环境光指数";
    canPlay = true;
    openPlay = true;
    isActive = true;
    this->setTheme(theme);
}

bool AppDataLightIndex::subEncode(SDTData::DataSourceType type)
{
    pushUint8(theme);
    return true;
}

bool AppDataLightIndex::subDecode(SDTData::DataSourceType type)
{
    this->setTheme(popUint8());
    return true;
}


void AppDataLightIndex::setTheme(uint8_t t)
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


void AppScheduleLightIndex::scheduleAction(float dt)
{
    
}


bool AppLightIndex::init()
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
        rootLayer = AppLightIndexLayer3208::createWithApp(this,Size(32,8));
    }
        break;
    case AppDataBase::AppSizeType::S16016:
    {
        rootLayer = AppLightIndexLayer16016::createWithApp(this,Size(16,16));
    }
        break;
    case AppDataBase::AppSizeType::S808:
    {
        rootLayer = AppLightIndexLayer808::createWithApp(this,Size(8,8));
    }
        break;
    case AppDataBase::AppSizeType::S12012:
    {
        rootLayer = AppLightIndexLayer12012::createWithApp(this,Size(12,12));
    }
        break;
    case AppDataBase::AppSizeType::S32016:
    {
        rootLayer = AppLightIndexLayer32016::createWithApp(this,Size(32,16));
    }
        break;
    default:
    {
        rootLayer = AppLightIndexLayer::createWithApp(this,Size::ZERO);
    }
        break;
    }
    this->addChild(rootLayer);
    return true;
}


bool AppLightIndexLayer::initLayer()
{
    return true;
}

NS_DT_END