#ifndef __APP_Weather_3208_H__
#define __APP_Weather_3208_H__


#include "AppWeather.h"
#include "HOUZZkitTester/app/AppLayerStyle/StyleIconNumber/LayerStyleIconNum3208.h"

NS_DT_BEGIN


class AppWeatherLayer3208 : public AppWeatherLayer ,public LayerStyleIconNum3208Delegate
{
private:
    
    uint8_t _showType = 0;
    
    AppDataWeather::WeatherType _preWeatherType = AppDataWeather::WeatherType::Unknow;

    uint8_t _preShowType = 0;
    
protected:
    
    LayerStyleIconNum3208 *_layerStyle = nullptr;
    
public:

    bool initLayer() override;
    
    void customUpdate(float dt);

    APP_LAYER_CREATE_WITH_APP(AppWeatherLayer3208,AppWeather);
    
    void printAction(SpriteCanvas* canvas) override;
};


NS_DT_END


#endif //__APP_Weather_3208_H__
