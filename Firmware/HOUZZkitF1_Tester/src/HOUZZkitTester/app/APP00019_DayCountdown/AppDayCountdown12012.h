#ifndef __APP_DayCountdown_12012_H__
#define __APP_DayCountdown_12012_H__


#include "AppDayCountdown.h"

NS_DT_BEGIN


class AppDayCountdownLayer12012 : public AppDayCountdownLayer
{
public:

    bool initLayer() override;

    APP_LAYER_CREATE_WITH_APP(AppDayCountdownLayer12012,AppDayCountdown);

};


NS_DT_END


#endif //__APP_DayCountdown_12012_H__