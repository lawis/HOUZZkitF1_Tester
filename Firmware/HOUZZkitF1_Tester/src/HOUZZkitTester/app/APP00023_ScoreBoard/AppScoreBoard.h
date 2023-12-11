#ifndef __APP_ScoreBoard_H__
#define __APP_ScoreBoard_H__

#include "HOUZZkitTester/app/AppBase.h"
#include "HOUZZkitTester/data/SDTData.h"
#include "HOUZZkitTester/device/SDTDevice.h"
#include "HOUZZkitTester/app/AppLayerStyle/StyleIconNumber/DataStyleIconNum.h"

NS_DT_BEGIN

#define AppScoreBoard_APP_ID      23

class AppScoreBoardTheme : public AppTheme
{
public:
    

};

class AppDataScoreBoard : public StyleIconNumData,public AppDataTemplate<AppDataScoreBoard,AppScoreBoard_APP_ID>
{
public:

    int16_t redScore;

    int16_t blueScore;

    int8_t redWinCount;

    int8_t blueWinCount;

protected:

    bool subEncode(SDTData::DataSourceType type) override;

    bool subDecode(SDTData::DataSourceType type) override;

public:

    AppDataScoreBoard();

    void setTheme(uint8_t t);

    void redAddScore(int8_t v);

    void blueAddScore(int8_t v);

    void redAddWinCount(int8_t v);

    void blueAddWinCount(int8_t v);

};


class AppScheduleScoreBoard : public AppScheduleTemplate<AppScheduleScoreBoard,AppDataScoreBoard,AppScoreBoard_APP_ID>
{

public:

	void scheduleAction(float dt) override;

};


class AppScoreBoard : public AppTemplate<AppScoreBoard,AppDataScoreBoard,AppScheduleScoreBoard,AppScoreBoard_APP_ID>
{

protected:

    bool init() override;

public:



};


class AppScoreBoardLayer : public AppLayerTemplate<AppScoreBoardLayer,AppScoreBoard,AppDataScoreBoard>
{
    
private:


protected:


public:


public:

    bool initLayer() override;
    
};


NS_DT_END

#endif //__APP_ScoreBoard_H__

