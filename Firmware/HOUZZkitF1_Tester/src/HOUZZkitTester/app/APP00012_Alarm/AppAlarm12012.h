#ifndef __APP_Alarm_12012_H__
#define __APP_Alarm_12012_H__


#include "AppAlarm.h"

NS_DT_BEGIN


class AppAlarmLayer12012 : public AppAlarmLayer
{
public:

    bool initLayer() override;

    APP_LAYER_CREATE_WITH_APP(AppAlarmLayer12012,AppAlarm);

};


NS_DT_END


#endif //__APP_Alarm_12012_H__