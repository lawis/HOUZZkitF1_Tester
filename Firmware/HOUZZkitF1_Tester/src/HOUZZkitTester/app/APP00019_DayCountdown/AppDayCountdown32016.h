#ifndef __APP_DayCountdown_32016_H__
#define __APP_DayCountdown_32016_H__


#include "AppDayCountdown.h"

NS_DT_BEGIN


class AppDayCountdownLayer32016 : public AppDayCountdownLayer
{
public:

    bool initLayer() override;

    APP_LAYER_CREATE_WITH_APP(AppDayCountdownLayer32016,AppDayCountdown);

};


NS_DT_END


#endif //__APP_DayCountdown_32016_H__