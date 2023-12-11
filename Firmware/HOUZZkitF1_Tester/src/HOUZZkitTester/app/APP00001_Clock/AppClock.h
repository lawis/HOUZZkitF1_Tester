#ifndef __APP_CLOCK_H__
#define __APP_CLOCK_H__

#include "HOUZZkitTester/app/AppBase.h"
#include "HOUZZkitTester/data/SDTData.h"
#include "Fonts/TomThumb.h"
#include "HOUZZkitTester/device/SDTDevice.h"

NS_DT_BEGIN

#define AppClock_APP_ID      1

class AppClockTheme : public AppTheme
{
public:
    
    DTRGB weekBgColor;
    DTRGB weekFgColor;

};

class AppDataClock : public AppDataTemplate<AppDataClock,AppClock_APP_ID>
{
private:
    
    uint8_t _theme;
    
public:
    
    AppClockTheme appTheme;

    bool isShowDate;

    bool isShowWeek;

    bool isShowSecond;

    bool is24HourView;

    bool isSundayFirstday;

    int8_t dayFormat;

    uint8_t timeShowDuration;

    uint8_t dateShowDuration;

protected:

    bool subEncode(SDTData::DataSourceType type) override;

    bool subDecode(SDTData::DataSourceType type) override;

    
public:
    
    void setTheme(uint8_t t);
    
    AppDataClock();

};


class AppScheduleClock : public AppScheduleTemplate<AppScheduleClock,AppDataClock,AppClock_APP_ID>
{

public:

   	bool parseRequest(const String& res);

	void scheduleAction(float dt) override;

};



class AppClock : public AppTemplate<AppClock,AppDataClock,AppScheduleClock,AppClock_APP_ID>
{

protected:

    bool init() override;
    
public:
    
    DateTime now();

};



class AppClockLayer : public AppLayerTemplate<AppClockLayer,AppClock,AppDataClock>
{
    
private:


protected:
    
    virtual void showDate(const DateTime& now,const AppDataClock& data){};
    
    virtual void showTime(const DateTime& now,const AppDataClock& data){};
    
    virtual void showWeek(const DateTime& now,const AppDataClock& data){};

public:


public:

    bool initLayer() override;
    
};



NS_DT_END

#endif //__APP_CLOCK_H__

