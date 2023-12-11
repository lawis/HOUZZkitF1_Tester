#ifndef __APP_Weibo_H__
#define __APP_Weibo_H__

#include "HOUZZkitTester/app/AppBase.h"
#include "HOUZZkitTester/data/SDTData.h"
#include "HOUZZkitTester/app/AppLayerStyle/StyleIconNumber/DataStyleIconNum.h"

NS_DT_BEGIN

#define AppWeibo_APP_ID      15






class AppDataWeibo :public StyleIconNumData, public AppDataTemplate<AppDataWeibo,AppWeibo_APP_ID>
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
    
    AppDataWeibo();

};


class AppRequestWeibo : public AppScheduleTemplate<AppRequestWeibo,AppDataWeibo,AppWeibo_APP_ID>
{

public:

   	bool parseRequest(const String& res);

    void scheduleAction(float dt) override;

};


class AppWeibo : public AppTemplate<AppWeibo,AppDataWeibo,AppRequestWeibo,AppWeibo_APP_ID>
{

protected:

    bool init() override;

};


class AppWeiboLayer : public AppLayerTemplate<AppWeiboLayer,AppWeibo,AppDataWeibo>
{
    
private:


protected:


public:


public:

    bool initLayer() override;
    
};


NS_DT_END

#endif //__APP_Weibo_H__

