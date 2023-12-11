#ifndef __APP_DayNumOfYear_12012_H__
#define __APP_DayNumOfYear_12012_H__


#include "AppDayNumOfYear.h"

NS_DT_BEGIN


class AppDayNumOfYearLayer12012 : public AppDayNumOfYearLayer
{
public:

    bool initLayer() override;

    APP_LAYER_CREATE_WITH_APP(AppDayNumOfYearLayer12012,AppDayNumOfYear);

};


NS_DT_END


#endif //__APP_DayNumOfYear_12012_H__