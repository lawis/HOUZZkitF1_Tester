#ifndef __APP_CLOCK_16016_H__
#define __APP_CLOCK_16016_H__

#include "AppClock.h"

NS_DT_BEGIN

class AppClockLayer16016 : public AppClockLayer
{
protected:

    TextSprite *_timeText;

    CanvasSprite *_timeSecond;
    
protected:

    void drawSecond(int8_t s,const DTRGB& color);

    void customUpdate(float dt);

public:

    bool initLayer() override;
    
    APP_LAYER_CREATE_WITH_APP(AppClockLayer16016,AppClock);

};


NS_DT_END

#endif //__APP_CLOCK_16016_H__
