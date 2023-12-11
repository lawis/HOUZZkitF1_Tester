#ifndef __APP_LightIndex_H__
#define __APP_LightIndex_H__

#include "HOUZZkitTester/app/AppBase.h"
#include "HOUZZkitTester/data/SDTData.h"
#include "HOUZZkitTester/device/SDTDevice.h"
#include "HOUZZkitTester/app/AppLayerStyle/StyleIconNumber/DataStyleIconNum.h"

NS_DT_BEGIN

#define AppLightIndex_APP_ID      20

class AppLightIndexTheme : public AppTheme
{
public:
    

};

class AppDataLightIndex :public StyleIconNumData, public AppDataTemplate<AppDataLightIndex,AppLightIndex_APP_ID>
{
protected:

    bool subEncode(SDTData::DataSourceType type) override;

    bool subDecode(SDTData::DataSourceType type) override;

public:

    AppDataLightIndex();

    void setTheme(uint8_t t);

};


class AppScheduleLightIndex : public AppScheduleTemplate<AppScheduleLightIndex,AppDataLightIndex,AppLightIndex_APP_ID>
{

public:

	void scheduleAction(float dt) override;

};


class AppLightIndex : public AppTemplate<AppLightIndex,AppDataLightIndex,AppScheduleLightIndex,AppLightIndex_APP_ID>
{

protected:

    bool init() override;

public:



};


class AppLightIndexLayer : public AppLayerTemplate<AppLightIndexLayer,AppLightIndex,AppDataLightIndex>
{
    
private:


protected:


public:


public:

    bool initLayer() override;
    
};


NS_DT_END

#endif //__APP_LightIndex_H__

