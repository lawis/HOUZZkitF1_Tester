#ifndef __APP_Alarm_16016_H__
#define __APP_Alarm_16016_H__


#include "AppAlarm.h"

NS_DT_BEGIN


class AppAlarmLayer16016 : public AppAlarmLayer
{
public:

    bool initLayer() override;

    APP_LAYER_CREATE_WITH_APP(AppAlarmLayer16016,AppAlarm);

};


NS_DT_END


#endif //__APP_Alarm_16016_H__