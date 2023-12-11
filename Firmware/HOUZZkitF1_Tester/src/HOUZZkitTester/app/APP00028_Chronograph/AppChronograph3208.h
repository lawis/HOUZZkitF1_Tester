#ifndef __APP_Chronograph_3208_H__
#define __APP_Chronograph_3208_H__


#include "AppChronograph.h"

NS_DT_BEGIN


class AppChronographLayer3208 : public AppChronographLayer
{
protected:

    TextSprite *_timeText;

    CanvasSprite *_recordCanvas;

public:

    bool initLayer() override;

    APP_LAYER_CREATE_WITH_APP(AppChronographLayer3208,AppChronograph);

    void update(float dt);

};


NS_DT_END


#endif //__APP_Chronograph_3208_H__