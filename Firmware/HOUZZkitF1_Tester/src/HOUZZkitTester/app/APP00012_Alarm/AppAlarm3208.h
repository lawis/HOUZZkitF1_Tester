#ifndef __APP_Alarm_3208_H__
#define __APP_Alarm_3208_H__


#include "AppAlarm.h"
#include "HOUZZkitTester/app/AppLayerStyle/StyleIconNumber/LayerStyleIconNum3208.h"

NS_DT_BEGIN


class AppAlarmLayer3208 : public AppAlarmLayer,public LayerStyleIconNum3208Delegate
{
protected:
    
    LayerStyleIconNum3208 *_layerStyle = nullptr;

public:

    bool initLayer() override;

    APP_LAYER_CREATE_WITH_APP(AppAlarmLayer3208,AppAlarm);

    void printAction(SpriteCanvas* canvas) override;

};


NS_DT_END


#endif //__APP_Alarm_3208_H__