#ifndef __APP_Chronograph_H__
#define __APP_Chronograph_H__

#include "HOUZZkitTester/app/AppBase.h"
#include "HOUZZkitTester/data/SDTData.h"
#include "HOUZZkitTester/device/SDTDevice.h"

NS_DT_BEGIN

#define AppChronograph_APP_ID      28
#define RECORD_ALL_MAX_NUM          8

class AppChronographTheme : public AppTheme
{
public:
    
    DTRGB recordBgColor;

    DTRGB recordFgColor;

    DTRGB curRecordBgColor;

    DTRGB curRecordFgColor;

};

class AppDataChronograph : public AppDataTemplate<AppDataChronograph,AppChronograph_APP_ID>
{
private:
    
    uint8_t _theme;

public:

    AppChronographTheme appTheme;

    int64_t startMillisecond;

    int64_t tempMillisecond;

    const uint8_t recordMaxNum = RECORD_ALL_MAX_NUM - 1;

    int64_t recordMillisecond[RECORD_ALL_MAX_NUM - 1];

    uint8_t recordNum;

    int8_t recordPointer;

    bool isRunning;

    // int64_t st

protected:

    bool subEncode(SDTData::DataSourceType type) override;

    bool subDecode(SDTData::DataSourceType type) override;

public:

    AppDataChronograph();

    void setTheme(uint8_t t);

    void reset();

    void start();

    void stop();

    void record();

    void next();

    void previous();

};


class AppScheduleChronograph : public AppScheduleTemplate<AppScheduleChronograph,AppDataChronograph,AppChronograph_APP_ID>
{

public:

   	bool parseRequest(const String& res);

	void scheduleAction(float dt) override;

};


class AppChronograph : public AppTemplate<AppChronograph,AppDataChronograph,AppScheduleChronograph,AppChronograph_APP_ID>
{

protected:

    bool init() override;

public:

    void buttonEventEnable() override;

    void buttonEventDisable() override;

};


class AppChronographLayer : public AppLayerTemplate<AppChronographLayer,AppChronograph,AppDataChronograph>
{
    
private:


protected:


public:


public:

    bool initLayer() override;
    
};


NS_DT_END

#endif //__APP_Chronograph_H__

