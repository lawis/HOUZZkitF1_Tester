#ifndef __APP_DayCountdown_H__
#define __APP_DayCountdown_H__

#include "HOUZZkitTester/app/AppBase.h"
#include "HOUZZkitTester/data/SDTData.h"
#include "HOUZZkitTester/device/SDTDevice.h"
#include "HOUZZkitTester/app/AppLayerStyle/StyleIconNumber/DataStyleIconNum.h"

NS_DT_BEGIN

#define AppDayCountdown_APP_ID      19

class AppDayCountdownTheme : public AppTheme
{
public:
    

};

class AppDataDayCountdown :public StyleIconNumData, public AppDataTemplate<AppDataDayCountdown,AppDayCountdown_APP_ID>
{
public:

    bool enable;

    uint16_t year;

    uint8_t month;

    uint8_t day;

    String musicPath;

    bool alarming;

protected:

    bool subEncode(SDTData::DataSourceType type) override;

    bool subDecode(SDTData::DataSourceType type) override;

    bool detectActive() override;

public:

    AppDataDayCountdown();

    void setTheme(uint8_t t);

};


class AppScheduleDayCountdown : public AppScheduleTemplate<AppScheduleDayCountdown,AppDataDayCountdown,AppDayCountdown_APP_ID>
{

protected:

    bool init() override;

public:

	void scheduleAction(float dt) override;

};


class AppDayCountdown : public AppTemplate<AppDayCountdown,AppDataDayCountdown,AppScheduleDayCountdown,AppDayCountdown_APP_ID>
{

protected:

    bool init() override;

public:

    DateTime now();

};


class AppDayCountdownLayer : public AppLayerTemplate<AppDayCountdownLayer,AppDayCountdown,AppDataDayCountdown>
{
    
private:


protected:


public:


public:

    bool initLayer() override;
    
};


NS_DT_END

#endif //__APP_DayCountdown_H__

