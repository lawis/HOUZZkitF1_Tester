#ifndef __APP_Stock_H__
#define __APP_Stock_H__

#include "HOUZZkitTester/app/AppBase.h"
#include "HOUZZkitTester/data/SDTData.h"
#include "HOUZZkitTester/app/AppLayerStyle/StyleIconNumber/DataStyleIconNum.h"

NS_DT_BEGIN

#define AppStock_APP_ID      17

class AppStockTheme : public AppTheme
{
public:
    

};

class AppDataStock :public StyleIconNumData,public AppDataTemplate<AppDataStock,AppStock_APP_ID>
{
public:

    bool isShowStockCode;

    String stockCode;
    
    String symbolName;

    float price;

    //增加自定义成员变量

protected:

    bool subEncode(SDTData::DataSourceType type) override;

    bool subDecode(SDTData::DataSourceType type) override;

public:

    AppDataStock();

    void setTheme(uint8_t t);

};


class AppScheduleStock : public AppScheduleTemplate<AppScheduleStock,AppDataStock,AppStock_APP_ID>
{

public:

   	bool parseRequest(const String& res);

	void scheduleAction(float dt) override;

};


class AppStock : public AppTemplate<AppStock,AppDataStock,AppScheduleStock,AppStock_APP_ID>
{

protected:

    bool init() override;

public:



};


class AppStockLayer : public AppLayerTemplate<AppStockLayer,AppStock,AppDataStock>
{
    
private:


protected:


public:


public:

    bool initLayer() override;
    
};


NS_DT_END

#endif //__APP_Stock_H__

