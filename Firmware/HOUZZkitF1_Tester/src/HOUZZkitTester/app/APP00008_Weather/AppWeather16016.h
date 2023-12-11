#ifndef __APP_Weather_16016_H__
#define __APP_Weather_16016_H__


#include "AppWeather.h"

NS_DT_BEGIN


class AppWeatherLayer16016 : public AppWeatherLayer
{
public:

    bool initLayer() override;

    APP_LAYER_CREATE_WITH_APP(AppWeatherLayer16016,AppWeather);

};


NS_DT_END


#endif //__APP_Weather_16016_H__