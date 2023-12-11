#ifndef __APP_DayCountdown_808_H__
#define __APP_DayCountdown_808_H__


#include "AppDayCountdown.h"

NS_DT_BEGIN


class AppDayCountdownLayer808 : public AppDayCountdownLayer
{
public:

    bool initLayer() override;

    APP_LAYER_CREATE_WITH_APP(AppDayCountdownLayer808,AppDayCountdown);

};


NS_DT_END


#endif //__APP_DayCountdown_808_H__