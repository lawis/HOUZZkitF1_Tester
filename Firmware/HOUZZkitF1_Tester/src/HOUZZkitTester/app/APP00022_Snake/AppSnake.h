#ifndef __APP_Snake_H__
#define __APP_Snake_H__

#include "HOUZZkitTester/app/AppBase.h"
#include "HOUZZkitTester/data/SDTData.h"

NS_DT_BEGIN

#define AppSnake_APP_ID      22

class AppDataSnake : public AppDataTemplate<AppDataSnake,AppSnake_APP_ID>
{
public:

    //增加自定义成员变量

protected:

    bool subEncode(SDTData::DataSourceType type) override;

    bool subDecode(SDTData::DataSourceType type) override;

public:

    AppDataSnake();

};


class AppScheduleSnake : public AppScheduleTemplate<AppScheduleSnake,AppDataSnake,AppSnake_APP_ID>
{

public:

   	bool parseRequest(const String& res);

	void scheduleAction(float dt) override;

};


class AppSnake : public AppTemplate<AppSnake,AppDataSnake,AppScheduleSnake,AppSnake_APP_ID>
{

protected:

    bool init() override;

public:

    void buttonEventEnable() override;

    void buttonEventDisable() override;
    
};


class AppSnakeLayer : public AppLayerTemplate<AppSnakeLayer,AppSnake,AppDataSnake>
{
    
private:


protected:

    SpriteCanvas * canvas = nullptr;

public:

    enum class Direction
    {
        D_NONE = -1,
        D_UP = 0,
        D_RIGHT = 1,
        D_DOWN = 2,
        D_LEFT = 3
    };

    virtual ~AppSnakeLayer();

public:

    virtual bool initLayer() override;

    void snakeUpdate(float dt);

protected:

    Direction d_controll;

    struct Snake
    {
        Direction dir;
        uint16_t length;
        Vec2 body[256];
    }snake;

    Vec2 food;

    void initSnake();

    bool createFood();
};


NS_DT_END

#endif //__APP_Snake_H__

