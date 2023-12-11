#ifndef __APP_Douyin_H__
#define __APP_Douyin_H__

#include "HOUZZkitTester/app/AppBase.h"
#include "HOUZZkitTester/data/SDTData.h"
#include "HOUZZkitTester/app/AppLayerStyle/StyleIconNumber/DataStyleIconNum.h"

NS_DT_BEGIN

#define AppDouyin_APP_ID      16





class AppDataDouyin :public StyleIconNumData, public AppDataTemplate<AppDataDouyin,AppDouyin_APP_ID>
{
public:

    String uid;
    
    int32_t fansCount;

protected:

    bool subEncode(SDTData::DataSourceType type) override;

    bool subDecode(SDTData::DataSourceType type) override;

    bool detectActive() override;

    void setTheme(uint8_t t);

public:
    
    AppDataDouyin();
};


class AppRequestDouyin : public AppScheduleTemplate<AppRequestDouyin,AppDataDouyin,AppDouyin_APP_ID>
{

public:

   	bool parseRequest(const String& res);

    void scheduleAction(float dt) override;

};


class AppDouyin : public AppTemplate<AppDouyin,AppDataDouyin,AppRequestDouyin,AppDouyin_APP_ID>
{

protected:

    bool init() override;

};


class AppDouyinLayer : public AppLayerTemplate<AppDouyinLayer,AppDouyin,AppDataDouyin>
{
    
private:


protected:


public:


public:

    bool initLayer() override;
    
};


NS_DT_END

#endif //__APP_Douyin_H__

