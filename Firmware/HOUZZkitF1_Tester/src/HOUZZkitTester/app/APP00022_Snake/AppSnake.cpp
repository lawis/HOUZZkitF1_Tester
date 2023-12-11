#include "AppSnake.h"


NS_DT_BEGIN

AppDataSnake::AppDataSnake()
{
    appAlias = "贪食蛇";
    canPlay = true;
    openPlay = true;
    isActive = true;
}

bool AppDataSnake::subEncode(SDTData::DataSourceType type)
{
    return true;
}

bool AppDataSnake::subDecode(SDTData::DataSourceType type)
{
    return true;
}


bool AppScheduleSnake::parseRequest(const String& res)
{
    return true;
}

void AppScheduleSnake::scheduleAction(float dt)
{
    
}


bool AppSnake::init()
{
    Size size = AppLayerBase::getSizeByAppSizeType(this->getData()->appSizeType);
    rootLayer = AppSnakeLayer::createWithApp(this,size);
    this->addChild(rootLayer);
    return true;
}

void AppSnake::buttonEventEnable()
{
    // LOG("AppSnake::buttonEventEnable");
    DEVICE.checkButtonClickOnly(true);
    rootLayer->resumeEventDispatcher();
}

void AppSnake::buttonEventDisable()
{
    // LOG("AppSnake::buttonEventDisable");
    DEVICE.checkButtonClickOnly(false);
    rootLayer->pauseEventDispatcher();
}

AppSnakeLayer::~AppSnakeLayer()
{

}

bool AppSnakeLayer::initLayer()
{
    Size size = this->getContentSize();
    CanvasSprite *canvasSprite = CanvasSprite::create(size.width,size.height);
    this->addChild(canvasSprite);
    this->scheduleUpdate();
    canvas = canvasSprite->getSpriteCanvas();

    auto listener = EventListenerButton::create();
    listener ->onBtnClick = [&](int8_t keyCode , Event * event ){
        if (keyCode >= 0 && keyCode <= 3)
        {
            d_controll = (Direction)keyCode;
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority ( listener, this );
    this->pauseEventDispatcher();
    this->initSnake();
    this->createFood();
    this->schedule(DT_SCHEDULE_SELECTOR(AppSnakeLayer::snakeUpdate),0.2);
    return true;
}

void AppSnakeLayer::snakeUpdate(float dt)
{
    Size size = this->getContentSize();
    Direction nextDir = snake.dir;
    int8_t dirDiff = abs((int8_t)nextDir - (int8_t)d_controll);
    if (dirDiff != 0 && dirDiff != 2)
    {
        nextDir = d_controll;
        snake.dir = nextDir;
    }
    Vec2 nextPos = snake.body[0];
    switch (nextDir)
    {
    case Direction::D_UP:{nextPos.y--;if(nextPos.y<0)nextPos.y=size.height-1;}break;
    case Direction::D_RIGHT:{nextPos.x++;if(nextPos.x>=size.width)nextPos.x=0;}break;
    case Direction::D_DOWN:{nextPos.y++;if(nextPos.y>=size.height)nextPos.y=0;}break;
    case Direction::D_LEFT:{nextPos.x--;if(nextPos.x<0)nextPos.x=size.width-1;}break;
    default:
        break;
    }
    for (uint16_t i = 0; i < snake.length; i++)
    {
        Vec2 tempPos = snake.body[i];
        snake.body[i] = nextPos;
        nextPos = tempPos;
    }
    if (snake.body[0] == food)
    {
        snake.body[snake.length] = nextPos;
        snake.length++;
        this->createFood();
    }
    for (uint16_t i = 4; i < snake.length; i++)
    {
        if (snake.body[0]==snake.body[i])
        {
            this->initSnake();
            this->createFood();
            break;
        }
    }
    //清楚屏幕内容
    canvas->canvasReset();
    //绘制蛇身
    for (uint16_t i = 0; i < snake.length; i++)
    {
        canvas->drawPixel(snake.body[i].x,snake.body[i].y,DTRGB(200,200,200));
    }
    //绘制食物
    canvas->drawPixel(food.x,food.y,DTRGB(200,0,0));
}

void AppSnakeLayer::initSnake()
{
    snake.length = 3;
    snake.dir = Direction::D_RIGHT;
    for (uint16_t i = 0; i < snake.length; i++)
    {
        snake.body[i] = Vec2(10-i,3);
    }
    d_controll = Direction::D_RIGHT;
}

bool AppSnakeLayer::createFood()
{
    //TODO 当蛇身体过于长时，完全随机生成食物位置，可能导致死循环，可以通过维护一个无蛇身位置表来优化
    Size size = this->getContentSize();
    bool pass = true;
    do
    {
        food.x = random(0,size.width-1);
        food.y = random(0,size.height-1);
        pass = true;
        for (uint16_t i = 0; i < snake.length; i++)
        {
            if (food == snake.body[i])
            {
                pass = false;
            }
        }
    } while (!pass);
    return true;
}

NS_DT_END