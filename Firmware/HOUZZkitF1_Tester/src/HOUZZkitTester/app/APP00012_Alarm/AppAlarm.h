#ifndef __APP_Alarm_H__
#define __APP_Alarm_H__

#include "HOUZZkitTester/app/AppBase.h"
#include "HOUZZkitTester/data/SDTData.h"
#include "HOUZZkitTester/device/SDTDevice.h"
#include "HOUZZkitTester/app/AppLayerStyle/StyleIconNumber/DataStyleIconNum.h"
NS_DT_BEGIN

#define AppAlarm_APP_ID      12

class AppAlarmTheme : public AppTheme
{
public:
    

};

class AppDataAlarm :public StyleIconNumData, public AppDataTemplate<AppDataAlarm,AppAlarm_APP_ID>
{
public:

    bool enable;

    uint8_t hour;

    uint8_t minute;

    String musicPath;

    bool alarming;

protected:

    bool subEncode(SDTData::DataSourceType type) override;

    bool subDecode(SDTData::DataSourceType type) override;

    bool detectActive() override;

public:

    AppDataAlarm();

    void setTheme(uint8_t t);

};


class AppScheduleAlarm : public AppScheduleTemplate<AppScheduleAlarm,AppDataAlarm,AppAlarm_APP_ID>
{

protected:

    bool init() override;

public:

	void scheduleAction(float dt) override;

};


class AppAlarm : public AppTemplate<AppAlarm,AppDataAlarm,AppScheduleAlarm,AppAlarm_APP_ID>
{

protected:

    bool init() override;

public:

    DateTime now();

};


class AppAlarmLayer : public AppLayerTemplate<AppAlarmLayer,AppAlarm,AppDataAlarm>
{
    
private:


protected:


public:


public:

    bool initLayer() override;
    
};


NS_DT_END

#endif //__APP_Alarm_H__

