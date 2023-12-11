#ifndef __APP_DayNumOfYear_H__
#define __APP_DayNumOfYear_H__

#include "HOUZZkitTester/app/AppBase.h"
#include "HOUZZkitTester/data/SDTData.h"
#include "HOUZZkitTester/device/SDTDevice.h"
#include "HOUZZkitTester/app/AppLayerStyle/StyleIconNumber/DataStyleIconNum.h"

NS_DT_BEGIN

#define AppDayNumOfYear_APP_ID      21

class AppDayNumOfYearTheme : public AppTheme
{
public:
    

};

class AppDataDayNumOfYear :public StyleIconNumData, public AppDataTemplate<AppDataDayNumOfYear,AppDayNumOfYear_APP_ID>
{
public:

    bool showAsPercent;

protected:

    bool subEncode(SDTData::DataSourceType type) override;

    bool subDecode(SDTData::DataSourceType type) override;

public:

    AppDataDayNumOfYear();

    void setTheme(uint8_t t);

};


class AppScheduleDayNumOfYear : public AppScheduleTemplate<AppScheduleDayNumOfYear,AppDataDayNumOfYear,AppDayNumOfYear_APP_ID>
{

public:

	void scheduleAction(float dt) override;

};


class AppDayNumOfYear : public AppTemplate<AppDayNumOfYear,AppDataDayNumOfYear,AppScheduleDayNumOfYear,AppDayNumOfYear_APP_ID>
{

protected:

    bool init() override;

public:

    uint16_t dayNum();

    float percentOfYear(float* linePercent,float* dotPercent);

    bool leapYear();

};


class AppDayNumOfYearLayer : public AppLayerTemplate<AppDayNumOfYearLayer,AppDayNumOfYear,AppDataDayNumOfYear>
{
    
private:


protected:


public:


public:

    bool initLayer() override;
    
};


NS_DT_END

#endif //__APP_DayNumOfYear_H__

