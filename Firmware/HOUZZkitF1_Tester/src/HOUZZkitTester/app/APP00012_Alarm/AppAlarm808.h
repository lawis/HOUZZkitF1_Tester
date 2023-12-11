#ifndef __APP_Alarm_808_H__
#define __APP_Alarm_808_H__


#include "AppAlarm.h"

NS_DT_BEGIN


class AppAlarmLayer808 : public AppAlarmLayer
{
public:

    bool initLayer() override;

    APP_LAYER_CREATE_WITH_APP(AppAlarmLayer808,AppAlarm);

};


NS_DT_END


#endif //__APP_Alarm_808_H__