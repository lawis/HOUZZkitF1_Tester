#ifndef __APP_DayNumOfYear_3208_H__
#define __APP_DayNumOfYear_3208_H__


#include "AppDayNumOfYear.h"
#include "HOUZZkitTester/app/AppLayerStyle/StyleIconNumber/LayerStyleIconNum3208.h"

NS_DT_BEGIN


class AppDayNumOfYearLayer3208 : public AppDayNumOfYearLayer,public LayerStyleIconNum3208Delegate
{
protected:
    
    LayerStyleIconNum3208 *_layerStyle = nullptr;

    CanvasSprite *_percentLine = nullptr;

    bool _preShowAsPercent = false;

public:

    bool initLayer() override;

    APP_LAYER_CREATE_WITH_APP(AppDayNumOfYearLayer3208,AppDayNumOfYear);

    void printAction(SpriteCanvas* canvas) override;

};


NS_DT_END


#endif //__APP_DayNumOfYear_3208_H__