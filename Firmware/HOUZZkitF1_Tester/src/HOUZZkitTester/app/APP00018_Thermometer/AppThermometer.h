#ifndef __APP_Thermometer_H__
#define __APP_Thermometer_H__

#include "HOUZZkitTester/app/AppBase.h"
#include "HOUZZkitTester/data/SDTData.h"
#include "HOUZZkitTester/app/AppLayerStyle/StyleIconNumber/DataStyleIconNum.h"

NS_DT_BEGIN

#define AppThermometer_APP_ID      18

class AppThermometerTheme : public AppTheme
{
public:
    

};

class AppDataThermometer :public StyleIconNumData, public AppDataTemplate<AppDataThermometer,AppThermometer_APP_ID>
{
public:
    //增加自定义成员变量


protected:

    bool subEncode(SDTData::DataSourceType type) override;

    bool subDecode(SDTData::DataSourceType type) override;

public:

    AppDataThermometer();

    void setTheme(uint8_t t);

};


class AppScheduleThermometer : public AppScheduleTemplate<AppScheduleThermometer,AppDataThermometer,AppThermometer_APP_ID>
{

public:

   	bool parseRequest(const String& res);

	void scheduleAction(float dt) override;

};


class AppThermometer : public AppTemplate<AppThermometer,AppDataThermometer,AppScheduleThermometer,AppThermometer_APP_ID>
{

protected:

    bool init() override;

public:



};


class AppThermometerLayer : public AppLayerTemplate<AppThermometerLayer,AppThermometer,AppDataThermometer>
{
    
private:


protected:


public:


public:

    bool initLayer() override;
    
};


NS_DT_END

#endif //__APP_Thermometer_H__

