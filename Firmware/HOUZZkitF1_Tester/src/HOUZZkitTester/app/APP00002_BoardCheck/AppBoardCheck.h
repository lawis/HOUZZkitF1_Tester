#ifndef __APP_BoardCheck_H__
#define __APP_BoardCheck_H__

#include "HOUZZkitTester/app/AppBase.h"
#include "HOUZZkitTester/data/SDTData.h"

NS_DT_BEGIN

#define AppBoardCheck_APP_ID      2

class AppDataBoardCheck : public AppDataTemplate<AppDataBoardCheck,AppBoardCheck_APP_ID>
{
public:

    //增加自定义成员变量
    enum class BoardCheckType : uint8_t
    {
        RGB_FILL        = 1,
        DOT_SHOW        = 2,
        RAINBOW_FILL    = 3
    };

    BoardCheckType boardCheckType;

protected:

    bool subEncode(SDTData::DataSourceType type) override;

    bool subDecode(SDTData::DataSourceType type) override;


public:
    
    AppDataBoardCheck();

};


class AppScheduleBoardCheck : public AppScheduleTemplate<AppScheduleBoardCheck,AppDataBoardCheck,AppBoardCheck_APP_ID>
{

public:

	void scheduleAction(float dt);

};


class AppBoardCheck : public AppTemplate<AppBoardCheck,AppDataBoardCheck,AppScheduleBoardCheck,AppBoardCheck_APP_ID>
{


protected:

    bool init() override;

};



class AppBoardCheckLayer : public AppLayerTemplate<AppBoardCheckLayer,AppBoardCheck,AppDataBoardCheck>
{
protected:

    SpriteCanvas *canvas = nullptr;

    uint16_t index = 0;

    uint8_t color = 0;

public:



protected:



public:

    bool initLayer() override;

    void customUpdate(float dt);

};


NS_DT_END

#endif //__APP_BoardCheck_H__

