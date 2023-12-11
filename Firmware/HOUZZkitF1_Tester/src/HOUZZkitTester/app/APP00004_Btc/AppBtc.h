#ifndef __APP_Btc_H__
#define __APP_Btc_H__

#include "HOUZZkitTester/app/AppBase.h"
#include "HOUZZkitTester/data/SDTData.h"
#include "HOUZZkitTester/app/AppLayerStyle/StyleIconNumber/DataStyleIconNum.h"

NS_DT_BEGIN

#define AppBtc_APP_ID      4

class AppDataBtc :public StyleIconNumData, public AppDataTemplate<AppDataBtc,AppBtc_APP_ID>
{
public:
    //增加自定义成员变量
    bool isShowCoinName;
    
    String coinName;
    
    String symbolName;
    
    float price;

protected:


    bool subEncode(SDTData::DataSourceType type) override;

    bool subDecode(SDTData::DataSourceType type) override;
    
    void setTheme(uint8_t t);

public:
    
    AppDataBtc();
};


class AppRequestBtc : public AppScheduleTemplate<AppRequestBtc,AppDataBtc,AppBtc_APP_ID>
{

public:

   	bool parseRequest(const String& res);

    void scheduleAction(float dt) override;

};


class AppBtc : public AppTemplate<AppBtc,AppDataBtc,AppRequestBtc,AppBtc_APP_ID>
{

protected:

    bool init() override;

};

class AppBtcLayer : public AppLayerTemplate<AppBtcLayer,AppBtc,AppDataBtc>
{
private:


protected:


public:



public:

    bool initLayer() override;
};



NS_DT_END

#endif //__APP_Btc_H__

