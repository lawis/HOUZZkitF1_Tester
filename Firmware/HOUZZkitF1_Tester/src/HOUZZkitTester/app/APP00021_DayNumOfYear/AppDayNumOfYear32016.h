#ifndef __APP_DayNumOfYear_32016_H__
#define __APP_DayNumOfYear_32016_H__


#include "AppDayNumOfYear.h"

NS_DT_BEGIN


class AppDayNumOfYearLayer32016 : public AppDayNumOfYearLayer
{
public:

    bool initLayer() override;

    APP_LAYER_CREATE_WITH_APP(AppDayNumOfYearLayer32016,AppDayNumOfYear);

};


NS_DT_END


#endif //__APP_DayNumOfYear_32016_H__