#ifndef __APP_Youtube_H__
#define __APP_Youtube_H__

#include "HOUZZkitTester/app/AppBase.h"
#include "HOUZZkitTester/data/SDTData.h"
#include "HOUZZkitTester/app/AppLayerStyle/StyleIconNumber/DataStyleIconNum.h"
NS_DT_BEGIN

#define AppYoutube_APP_ID      14


const uint8_t icon_youtube[] PROGMEM =
{
    0x00,0x4a,
    0x44,0x47,0x46,0x11,0x08,0x08,0x42,0x00,
    0x00,0x00,0xff,0x00,0x00,0xff,0xff,0xff,
    0x2c,0x00,0x05,0x00,0x01,0x08,0x06,0x80,
    0x00,0x15,0x54,0x55,0x55,0x55,0x55,0x55,
    0x55,0x55,0x55,0x15,0x54,0x2c,0x00,0x05,
    0x03,0x03,0x01,0x02,0x80,0x20,0x00,0x00,
    0xa0,0x2c,0x00,0x05,0x03,0x02,0x01,0x04,
    0x80,0x20,0x00,0x00,0xaa,0x2c,0x01,0xf4,
    0x04,0x03,0x01,0x02,0x80,0x20,0x00,0x00,
    0xa0,0x96
};

class AppDataYoutube :public StyleIconNumData,public AppDataTemplate<AppDataYoutube,AppYoutube_APP_ID> 
{
public:

    String channelID;

    String apiKey;
    
    int32_t fansCount;

protected:

    bool subEncode(SDTData::DataSourceType type) override;

    bool subDecode(SDTData::DataSourceType type) override;

    bool detectActive() override;

    void setTheme(uint8_t t);

public:
    
    AppDataYoutube();
    
};


class AppRequestYoutube : public AppScheduleTemplate<AppRequestYoutube,AppDataYoutube,AppYoutube_APP_ID>
{

public:

   	bool parseRequest(const String& res);

    void scheduleAction(float dt) override;

};


class AppYoutube : public AppTemplate<AppYoutube,AppDataYoutube,AppRequestYoutube,AppYoutube_APP_ID>
{

protected:

    bool init() override;

};


class AppYoutubeLayer : public AppLayerTemplate<AppYoutubeLayer,AppYoutube,AppDataYoutube>
{
    
private:


protected:


public:


public:

    bool initLayer() override;
    
};


NS_DT_END

#endif //__APP_Youtube_H__

