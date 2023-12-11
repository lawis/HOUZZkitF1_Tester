#ifndef __APP_Weather_H__
#define __APP_Weather_H__

#include "HOUZZkitTester/app/AppBase.h"
#include "HOUZZkitTester/data/SDTData.h"
#include "HOUZZkitTester/app/AppLayerStyle/StyleIconNumber/DataStyleIconNum.h"

NS_DT_BEGIN

#define AppWeather_APP_ID      8

class AppWeatherTheme : public AppTheme
{
public:
    

};

class AppDataWeather :public StyleIconNumData, public AppDataTemplate<AppDataWeather,AppWeather_APP_ID>
{
public:
    enum class WeatherType
    {
        Unknow = -1,
        Sunny = 0,
        Overcast = 1,
        Cloudy = 2,
        Rainy = 3,
        Snowy = 4,
        Foggy = 5,
        Smog = 6,
    };

    //增加自定义成员变量
    String cityCode;
    
    int8_t temperature;

    int8_t humidity;

    int16_t airPressure;

    int16_t aqi;

    WeatherType weatherType;

protected:

    bool subEncode(SDTData::DataSourceType type) override;

    bool subDecode(SDTData::DataSourceType type) override;

    bool detectActive() override;

public:

    AppDataWeather();

    void setTheme(uint8_t t);

};


class AppScheduleWeather : public AppScheduleTemplate<AppScheduleWeather,AppDataWeather,AppWeather_APP_ID>
{

public:

   	bool parseRequest(const String& res);

	void scheduleAction(float dt) override;

};


class AppWeather : public AppTemplate<AppWeather,AppDataWeather,AppScheduleWeather,AppWeather_APP_ID>
{

protected:

    bool init() override;

public:



};


class AppWeatherLayer : public AppLayerTemplate<AppWeatherLayer,AppWeather,AppDataWeather>
{
    
private:


protected:


public:


public:

    bool initLayer() override;
    
};


NS_DT_END

#endif //__APP_Weather_H__

