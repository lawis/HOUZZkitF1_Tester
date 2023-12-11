#ifndef __APP_CLOCK_808_H__
#define __APP_CLOCK_808_H__


#include "AppClock.h"

NS_DT_BEGIN

class AppClockLayer808 : public AppClockLayer
{
private:

    LayerColor* _colorBg = nullptr;

    uint8_t turnCounter = 0;

protected:

    TextSprite *_timeText;

    CanvasSprite *_timeSecond;

public:

    bool showMonth = false;

public:

    bool initLayer() override;

    void customUpdate(float dt);
    
    APP_LAYER_CREATE_WITH_APP(AppClockLayer808,AppClock);

};


NS_DT_END


#endif //__APP_CLOCK_808_H__
