#ifndef __APP_DayCountdown_16016_H__
#define __APP_DayCountdown_16016_H__


#include "AppDayCountdown.h"

NS_DT_BEGIN


class AppDayCountdownLayer16016 : public AppDayCountdownLayer
{
public:

    bool initLayer() override;

    APP_LAYER_CREATE_WITH_APP(AppDayCountdownLayer16016,AppDayCountdown);

};


NS_DT_END


#endif //__APP_DayCountdown_16016_H__