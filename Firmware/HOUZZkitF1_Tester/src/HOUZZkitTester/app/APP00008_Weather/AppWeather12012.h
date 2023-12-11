#ifndef __APP_Weather_12012_H__
#define __APP_Weather_12012_H__


#include "AppWeather.h"

NS_DT_BEGIN


class AppWeatherLayer12012 : public AppWeatherLayer
{
public:

    bool initLayer() override;

    APP_LAYER_CREATE_WITH_APP(AppWeatherLayer12012,AppWeather);

};


NS_DT_END


#endif //__APP_Weather_12012_H__