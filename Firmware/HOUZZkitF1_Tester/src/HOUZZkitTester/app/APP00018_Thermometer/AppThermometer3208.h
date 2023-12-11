#ifndef __APP_Thermometer_3208_H__
#define __APP_Thermometer_3208_H__


#include "AppThermometer.h"
#include "HOUZZkitTester/app/AppLayerStyle/StyleIconNumber/LayerStyleIconNum3208.h"

NS_DT_BEGIN


class AppThermometerLayer3208 : public AppThermometerLayer,public LayerStyleIconNum3208Delegate
{
private:
    
    bool _temAndHumSwitch = true;

    bool _preTemAndHumSwitch = true;

protected:
    
    LayerStyleIconNum3208 *_layerStyle = nullptr;
    
public:

    bool initLayer() override;

    void customUpdate(float dt);

    APP_LAYER_CREATE_WITH_APP(AppThermometerLayer3208,AppThermometer);

    void printAction(SpriteCanvas* canvas) override;
};


NS_DT_END


#endif //__APP_Thermometer_3208_H__