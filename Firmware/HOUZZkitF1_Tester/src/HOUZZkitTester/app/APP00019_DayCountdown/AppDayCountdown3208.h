#ifndef __APP_DayCountdown_3208_H__
#define __APP_DayCountdown_3208_H__


#include "AppDayCountdown.h"
#include "HOUZZkitTester/app/AppLayerStyle/StyleIconNumber/LayerStyleIconNum3208.h"

NS_DT_BEGIN


class AppDayCountdownLayer3208 : public AppDayCountdownLayer,public LayerStyleIconNum3208Delegate
{
protected:
    
    LayerStyleIconNum3208 *_layerStyle = nullptr;

public:

    bool initLayer() override;

    APP_LAYER_CREATE_WITH_APP(AppDayCountdownLayer3208,AppDayCountdown);

    void printAction(SpriteCanvas* canvas) override;

};


NS_DT_END


#endif //__APP_DayCountdown_3208_H__