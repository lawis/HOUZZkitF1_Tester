#ifndef __APP_CLOCK_3208_H__
#define __APP_CLOCK_3208_H__


#include "AppClock.h"

NS_DT_BEGIN

class AppClockLayer3208 : public AppClockLayer
{
private:
    
    bool _dateAndTimeSwitch = false;
    
    bool _showColon = false;
    
    uint8_t _preSecond = 0;

protected:

    TextSprite *_timeText;

    CanvasSprite *_weekCanvas;

protected:

    void showDate(const DateTime& now,const AppDataClock& data) override;
    
    void showTime(const DateTime& now,const AppDataClock& data) override;
    
    void showWeek(const DateTime& now,const AppDataClock& data) override;

public:

    bool initLayer() override;

    void customUpdate(float dt);
    
    void dateAndTimeSwitch(float dt);
    
    APP_LAYER_CREATE_WITH_APP(AppClockLayer3208,AppClock);

};


NS_DT_END


#endif //__APP_CLOCK_3208_H__
