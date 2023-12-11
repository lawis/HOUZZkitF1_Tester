#include "AppThermometer.h"
#include "AppThermometer3208.h"
#include "AppThermometer16016.h"
#include "AppThermometer808.h"
#include "AppThermometer12012.h"
#include "AppThermometer32016.h"


NS_DT_BEGIN

AppDataThermometer::AppDataThermometer()
{
    scheduleType = AppDataBase::ScheduleType::NONE;
    scheduleInterval = 0;

    appAlias = "室内温湿度";
    canPlay = true;
    openPlay = true;
    isActive = true;

    this->setTheme(theme);
}

bool AppDataThermometer::subEncode(SDTData::DataSourceType type)
{
    pushUint8(theme);
    return true;
}

bool AppDataThermometer::subDecode(SDTData::DataSourceType type)
{
    this->setTheme(popUint8());
    return true;
}


void AppDataThermometer::setTheme(uint8_t t)
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


bool AppScheduleThermometer::parseRequest(const String& res)
{
    return true;
}

void AppScheduleThermometer::scheduleAction(float dt)
{
    
}


bool AppThermometer::init()
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
        rootLayer = AppThermometerLayer3208::createWithApp(this,Size(32,8));
    }
        break;
    case AppDataBase::AppSizeType::S16016:
    {
        rootLayer = AppThermometerLayer16016::createWithApp(this,Size(16,16));
    }
        break;
    case AppDataBase::AppSizeType::S808:
    {
        rootLayer = AppThermometerLayer808::createWithApp(this,Size(8,8));
    }
        break;
    case AppDataBase::AppSizeType::S12012:
    {
        rootLayer = AppThermometerLayer12012::createWithApp(this,Size(12,12));
    }
        break;
    case AppDataBase::AppSizeType::S32016:
    {
        rootLayer = AppThermometerLayer32016::createWithApp(this,Size(32,16));
    }
        break;
    default:
    {
        rootLayer = AppThermometerLayer::createWithApp(this,Size::ZERO);
    }
        break;
    }
    this->addChild(rootLayer);
    return true;
}


bool AppThermometerLayer::initLayer()
{
    return true;
}

NS_DT_END