#include "AppWeather.h"
#include "AppWeather3208.h"
#include "AppWeather16016.h"
#include "AppWeather12012.h"
#include "AppWeather32016.h"
#include "AppWeather808.h"


NS_DT_BEGIN

AppDataWeather::AppDataWeather()
:cityCode("")
,temperature(0)
,humidity(0)
,airPressure(0)
,aqi(0)
,weatherType(WeatherType::Unknow)
{
    scheduleType = AppDataBase::ScheduleType::SCHEDULE;
    scheduleInterval = 15 * 60;

    appAlias = "实时天气";
    canPlay = true;
    openPlay = true;
    isActive = false;

    this->setTheme(theme);
}

bool AppDataWeather::subEncode(SDTData::DataSourceType type)
{
    pushUint8(theme);
    pushString8(cityCode.c_str());
    return true;
}

bool AppDataWeather::subDecode(SDTData::DataSourceType type)
{
    this->setTheme(popUint8());
    cityCode = popString8().c_str();
    return true;
}

bool AppDataWeather::detectActive()
{
    isActive = false;
    if (cityCode.length()>5) {
        isActive = true;
    }
    return isActive;
}

void AppDataWeather::setTheme(uint8_t t)
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


bool AppScheduleWeather::parseRequest(const String& res)
{
    uint32_t size = res.length();
    DynamicJsonDocument json(size+256);
    DeserializationError error = deserializeJson(json, res.substring(res.indexOf("{")));
    if (error) {
        LOG("ERROR!!! AppWeather Parse Failed! ");
        return false; 
    }
    uint16_t weatherType = json["weathercode"].as<String>().substring(1).toInt();
    this->getData()->temperature = json["temp"].as<int>();
    String sd = json["sd"].as<String>();
    this->getData()->humidity = sd.substring(0,sd.length()-1).toInt();
    this->getData()->airPressure = json["qy"].as<int>();
    this->getData()->aqi = json["aqi"].as<int>();
    
    switch (weatherType) {
        case 0:
            this->getData()->weatherType = dot2d::AppDataWeather::WeatherType::Sunny;
            break;
        case 2:
            this->getData()->weatherType = dot2d::AppDataWeather::WeatherType::Overcast;
            break;
        case 1:
            this->getData()->weatherType = dot2d::AppDataWeather::WeatherType::Cloudy;
            break;
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 19:
        case 21:
        case 22:
        case 23:
        case 24:
        case 25:
            this->getData()->weatherType = dot2d::AppDataWeather::WeatherType::Rainy;
            break;
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
        case 26:
        case 27:
        case 28:
            this->getData()->weatherType = dot2d::AppDataWeather::WeatherType::Snowy;
            break;
        case 18:
            this->getData()->weatherType = dot2d::AppDataWeather::WeatherType::Foggy;
            break;
        case 20:
        case 29:
        case 30:
        case 31:
        case 53:
            this->getData()->weatherType = dot2d::AppDataWeather::WeatherType::Smog;
            break;
        default:
            this->getData()->weatherType = dot2d::AppDataWeather::WeatherType::Unknow;
            break;
    }
    return true;
}

void AppScheduleWeather::scheduleAction(float dt)
{
    if (!this->getData()->isActive)
    {
        return;
    }
    String url = "http://d1.weather.com.cn/sk_2d/";
    url += this->getData()->cityCode + ".html";
    WebRequestTask *task = this->pushRequestTask(url,false,this,AppScheduleBase::HandleRequestCallback(&AppScheduleWeather::parseRequest));
    task->headerMap["Referer"] = url;
}


bool AppWeather::init()
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
        rootLayer = AppWeatherLayer3208::createWithApp(this,Size(32,8));
    }
        break;
    case AppDataBase::AppSizeType::S16016:
    {
        rootLayer = AppWeatherLayer16016::createWithApp(this,Size(16,16));
    }
        break;
    case AppDataBase::AppSizeType::S808:
    {
        rootLayer = AppWeatherLayer808::createWithApp(this,Size(8,8));
    }
        break;
    case AppDataBase::AppSizeType::S12012:
    {
        rootLayer = AppWeatherLayer12012::createWithApp(this,Size(12,12));
    }
        break;
    case AppDataBase::AppSizeType::S32016:
    {
        rootLayer = AppWeatherLayer32016::createWithApp(this,Size(32,16));
    }
        break;
    default:
    {
        rootLayer = AppWeatherLayer::createWithApp(this,Size::ZERO);
    }
        break;
    }
    this->addChild(rootLayer);
    return true;
}


bool AppWeatherLayer::initLayer()
{
    return true;
}

NS_DT_END
