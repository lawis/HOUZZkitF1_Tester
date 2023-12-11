#ifndef __APP_Weather_808_H__
#define __APP_Weather_808_H__


#include "AppWeather.h"

NS_DT_BEGIN


class AppWeatherLayer808 : public AppWeatherLayer
{
public:

    bool initLayer() override;

    APP_LAYER_CREATE_WITH_APP(AppWeatherLayer808,AppWeather);

};


NS_DT_END


#endif //__APP_Weather_808_H__