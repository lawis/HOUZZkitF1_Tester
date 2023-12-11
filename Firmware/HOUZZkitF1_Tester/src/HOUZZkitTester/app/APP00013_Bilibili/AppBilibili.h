#ifndef __APP_Bilibili_H__
#define __APP_Bilibili_H__

#include "HOUZZkitTester/app/AppBase.h"
#include "HOUZZkitTester/data/SDTData.h"
#include "HOUZZkitTester/app/AppLayerStyle/StyleIconNumber/DataStyleIconNum.h"

NS_DT_BEGIN

#define AppBilibili_APP_ID      13

const uint8_t icon_bilibili[] PROGMEM = 
{
    0x00,0x37,
    0x44,0x47,0x46,0x11,0x08,0x08,0x01,0x00,
    0x00,0x00,0x0f,0x37,0xff,0x2c,0x01,0xf4,
    0x00,0x00,0x08,0x08,0x80,0x00,0x42,0x24,
    0x7e,0x81,0xa5,0xa5,0x81,0x7e,0x2c,0x00,
    0x0a,0x02,0x04,0x04,0x01,0x40,0x20,0x00,
    0x00,0x00,0x2c,0x00,0x0a,0x02,0x05,0x04,
    0x01,0x40,0x20,0x00,0x00,0x00,0x96
};


class AppDataBilibili :public StyleIconNumData, public AppDataTemplate<AppDataBilibili,AppBilibili_APP_ID>
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
    
    AppDataBilibili();

};


class AppRequestBilibili : public AppScheduleTemplate<AppRequestBilibili,AppDataBilibili,AppBilibili_APP_ID>
{

public:

   	bool parseRequest(const String& res);

    void scheduleAction(float dt) override;

};


class AppBilibili : public AppTemplate<AppBilibili,AppDataBilibili,AppRequestBilibili,AppBilibili_APP_ID>
{

protected:

    bool init() override;

};


class AppBilibiliLayer : public AppLayerTemplate<AppBilibiliLayer,AppBilibili,AppDataBilibili>
{
    
private:


protected:


public:


public:

    bool initLayer() override;
    
};


NS_DT_END

#endif //__APP_Bilibili_H__

