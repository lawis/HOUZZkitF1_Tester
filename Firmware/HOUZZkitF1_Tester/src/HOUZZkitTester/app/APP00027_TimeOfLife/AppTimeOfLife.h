#ifndef __APP_TimeOfLife_H__
#define __APP_TimeOfLife_H__

#include "HOUZZkitTester/app/AppBase.h"
#include "HOUZZkitTester/data/SDTData.h"

NS_DT_BEGIN

#define AppTimeOfLife_APP_ID      27

class AppTimeOfLifeTheme : public AppTheme
{
public:

    DTRGB bgColor;
    DTRGB fgColor;

};

class AppDataTimeOfLife : public AppDataTemplate<AppDataTimeOfLife,AppTimeOfLife_APP_ID>
{
private:
    
    uint8_t _theme;

public:

    AppTimeOfLifeTheme appTheme;

    uint16_t year;

    uint8_t month;

    uint8_t day;
    
    uint8_t hour;
    
    uint8_t minute;

    //增加自定义成员变量

protected:

    bool subEncode(SDTData::DataSourceType type) override;

    bool subDecode(SDTData::DataSourceType type) override;

public:

    AppDataTimeOfLife();

    void setTheme(uint8_t t);

};


class AppScheduleTimeOfLife : public AppScheduleTemplate<AppScheduleTimeOfLife,AppDataTimeOfLife,AppTimeOfLife_APP_ID>
{

public:

   	bool parseRequest(const String& res);

	void scheduleAction(float dt) override;

};


class AppTimeOfLife : public AppTemplate<AppTimeOfLife,AppDataTimeOfLife,AppScheduleTimeOfLife,AppTimeOfLife_APP_ID>
{

protected:

    bool init() override;

public:

    void timeLife(uint16_t *days,uint8_t *hours,uint8_t *minutes);

};


class AppTimeOfLifeLayer : public AppLayerTemplate<AppTimeOfLifeLayer,AppTimeOfLife,AppDataTimeOfLife>
{
    
private:


protected:


public:


public:

    bool initLayer() override;
    
};


NS_DT_END

#endif //__APP_TimeOfLife_H__

