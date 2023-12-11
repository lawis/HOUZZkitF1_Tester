#include "SDTScreen.h"
#include "HOUZZkitTester/SDTConfig.h"

static LedScreenManager* s_LedScreenManager = nullptr;  

bool LedScreenManager::init()
{
#if defined(DEVICE_PLUS)
    _ledScreen = new LedScreenPlus();
#elif defined(DEVICE_PRO)
    
#elif defined(DEVICE_CLASSICAL)
    _ledScreen = new LedScreenClassical();
    
#elif defined(DEVICE_AIR)
    _ledScreen = new LedScreenClassical();
#else
    
#endif
    return true;
}

LedScreenManager* LedScreenManager::getInstance()
{
    if (s_LedScreenManager == nullptr)
    {
        s_LedScreenManager = new LedScreenManager();
        s_LedScreenManager->init();
    }
    return s_LedScreenManager;
}

void LedScreenManager::initScreen()
{
    _ledScreen->initScreen();
}

void LedScreenManager::initMatrix(dot2d::DTRGB *data)
{
    _ledScreen->initMatrix(data);
    this->screenSwitch(true);
    this->clear();
    this->show();

}

void LedScreenManager::setBrightness(uint8_t scale)
{
#ifndef TARGET_OS_MAC
    FastLED.setBrightness(MAX_LED_BRIGHTNESS / 100.0 * scale);
#endif
}

void LedScreenManager::closeScreen()
{
#ifndef TARGET_OS_MAC
    FastLED.clear();
    FastLED.show();
    pinMode(LED_PIN,INPUT_PULLDOWN);
#endif
}

void LedScreenManager::clear()
{
#ifndef TARGET_OS_MAC
    FastLED.clear();
#endif
}

dot2d::Size LedScreenManager::getMatrixSize()
{
    return _ledScreen->getMatrixSize();
}

void LedScreenManager::screenSwitch(bool e)
{
#ifndef TARGET_OS_MAC
    if(e)
    {
        pinMode(LED_POWER_PIN,OUTPUT);
        digitalWrite(LED_POWER_PIN,HIGH);
    }else
    {
        pinMode(LED_POWER_PIN,OUTPUT);
        digitalWrite(LED_POWER_PIN,LOW);
    }
#endif
}

//=====================LedScreenClassical==================//

void LedScreenClassical::initScreen()
{

}

void LedScreenClassical::initMatrix(dot2d::DTRGB *data)
{
    dot2d::Size size = this->getMatrixSize();
#ifndef TARGET_OS_MAC
    uint32_t ledCount = size.width * size.height;
    FastLED.addLeds<WS2812Controller800Khz,LED_PIN, GRB>((CRGB* )data,ledCount);
#endif
    dot2d::Director* director = dot2d::Director::getInstance();
    for (uint8_t y = 0; y < size.width; y++)
    {
        for (uint8_t x = 0; x < size.width; x++)
        {
        
            uint16_t order = y*size.width;
            if (y%2==0)
            {
                order+=x;
            }else
            {
                order+=(31-x);
            }
            director->getRenderer()->setMatrixOrder(x,y,order);
        }
    }
}


//=====================LedScreenPlus==================//

LedScreenPlus::Direction LedScreenPlus::relativeDirection(dot2d::Vec2& origin ,dot2d::Vec2& dest)
{
    if (dest.x < 0 || dest.y < 0 || origin.x < 0 || origin.y < 0)
    {
        return Direction::NONE;
    }
    
    if(dest.x == origin.x)
    {
        if(dest.y > origin.y)
        {
            return Direction::DOWN;
        }else if(dest.y < origin.y)
        {
            return Direction::UP;
        }
    }else if(dest.y == origin.y)
    {
        if(dest.x > origin.x)
        {
            return Direction::RIGHT;
        }else if(dest.x < origin.x)
        {
            return Direction::LEFT;
        }
    }
    return Direction::NONE;
}

dot2d::Vec2 LedScreenPlus::getBoardPosByOrder(int16_t order)
{
    dot2d::Vec2 res(-1,-1);
    if (order>= 0 && order < _boardCount)
    {
        res.x = _boardOrder[order*2];
        res.y = _boardOrder[order*2 + 1];
    }
    return res;
}

void LedScreenPlus::setBoardData(uint8_t matrixWidth,uint8_t matrixHeight,uint8_t* order)
{
    _boardWidth = matrixWidth;
    _boardHeight = matrixHeight;
    _boardCount = _boardWidth * _boardHeight;
    uint32_t buffSize = _boardCount * 2;
    _boardOrder = (uint8_t* )malloc(buffSize);
    for (size_t i = 0; i < buffSize; i++)
    {
        _boardOrder[i] = order[i];
    }
}

LedScreenPlus::Direction LedScreenPlus::findDirection(Direction prev,int16_t order)
{
    dot2d::Vec2 selfPos = getBoardPosByOrder(order);
    dot2d::Vec2 prevPos = getBoardPosByOrder(order - 1);
    dot2d::Vec2 nextPos = getBoardPosByOrder(order + 1);
    Direction prevDir = relativeDirection(selfPos,prevPos);
    Direction nextDir = relativeDirection(selfPos,nextPos);
    switch (prevDir)
    {
        case Direction::NONE:
        {
            switch (nextDir)
            {
                case Direction::NONE:
                    return Direction::UP;
                default:
                    return findDirection(Direction::NONE,order+1);
            }
        }
            break;
        case Direction::UP:
        {
            switch (nextDir)
            {
                case Direction::NONE:
                    if (prev != Direction::NONE){return prev;}
                    else {return Direction::UP;}
                case Direction::UP:
                    return Direction::NONE;
                case Direction::RIGHT:
                    return Direction::UP;
                case Direction::DOWN:
                    return findDirection(Direction::NONE,order+1);
                case Direction::LEFT:
                    return Direction::RIGHT;
                default:
                    break;
            }
        }
            break;
        case Direction::RIGHT:
        {
            switch (nextDir)
            {
                case Direction::NONE:
                    if (prev != Direction::NONE){return prev;}
                    else {return Direction::RIGHT;}
                case Direction::UP:
                    return Direction::DOWN;
                case Direction::RIGHT:
                    return Direction::NONE;
                case Direction::DOWN:
                    return Direction::RIGHT;
                case Direction::LEFT:
                    return findDirection(Direction::NONE,order+1);
                default:
                    break;
            }
        }
            break;
        case Direction::DOWN:
        {
            switch (nextDir)
            {
                case Direction::NONE:
                    if (prev != Direction::NONE){return prev;}
                    else {return Direction::DOWN;}
                case Direction::UP:
                    return findDirection(Direction::NONE,order+1);
                case Direction::RIGHT:
                    return Direction::LEFT;
                case Direction::DOWN:
                    return Direction::NONE;
                case Direction::LEFT:
                    return Direction::DOWN;
                default:
                    break;
            }
        }
            break;
        case Direction::LEFT:
        {
            switch (nextDir)
            {
                case Direction::NONE:
                    if (prev != Direction::NONE){return prev;}
                    else {return Direction::LEFT;}
                case Direction::UP:
                    return Direction::LEFT;
                case Direction::RIGHT:
                    return findDirection(Direction::NONE,order+1);
                case Direction::DOWN:
                    return Direction::UP;
                case Direction::LEFT:
                    return Direction::NONE;
                default:
                    break;
            }
        }
            break;
        default:
            break;
    }
    return Direction::NONE;
}

void LedScreenPlus::initScreen()
{
        //TODO 读取矩阵信息
//   uint8_t matrixWidth = 8;
//   uint8_t matrixHeight = 4;
//   uint8_t matrixOrder[] = { 0,3,0,2,0,1,0,0,
//                             1,0,1,1,1,2,1,3,
//                             2,3,2,2,2,1,2,0,
//                             3,0,3,1,3,2,3,3,
//                             4,3,4,2,4,1,4,0,
//                             5,0,5,1,5,2,5,3,
//                             6,3,6,2,6,1,6,0,
//                             7,0,7,1,7,2,7,3};

    uint8_t matrixWidth = 4;    
  uint8_t matrixHeight = 4;
//   uint8_t matrixOrder[] = { 0,1,0,0,
//                             1,0,1,1};

  uint8_t matrixOrder[] = { 0,3,0,2,0,1,0,0,
                            1,0,1,1,1,2,1,3,
                            2,3,2,2,2,1,2,0,
                            3,0,3,1,3,2,3,3};

  //初始化灯珠排序规则
  this->setBoardData(matrixWidth,matrixHeight,matrixOrder);

}

void LedScreenPlus::initMatrix(dot2d::DTRGB *data)
{

#ifndef TARGET_OS_MAC
    dot2d::Size size = this->getMatrixSize();
    uint32_t ledCount = size.width * size.height;
    FastLED.addLeds<WS2812Controller800Khz,LED_PIN, RGB>((CRGB* )data,ledCount);
#endif



    dot2d::Director* director = dot2d::Director::getInstance();
    Direction dir = Direction::NONE;
    for (int32_t i = 0; i < _boardCount; i++)
    {
        dot2d::Vec2 selfPos(getBoardPosByOrder(i));
        dot2d::Vec2 ledPos;
        dir = findDirection(dir,i);
        if (dir == Direction::NONE)
        {
            continue;
        }
        for (int8_t dX = 0; dX < PER_BOARD_WIDTH; dX++)
        {
            for (int8_t dY = 0; dY < PER_BOARD_HEIGHT; dY++)
            {
                uint16_t order = i * (PER_BOARD_WIDTH * PER_BOARD_HEIGHT) + dX * PER_BOARD_WIDTH + dY;
                ledPos.x = selfPos.x * PER_BOARD_WIDTH;
                ledPos.y = selfPos.y * PER_BOARD_WIDTH;
                switch (dir)
                {
                case Direction::UP:
                {
                    ledPos.x += dX;
                    ledPos.y += dY;
                }
                    break;
                case Direction::RIGHT:
                {
                    ledPos.x += (PER_BOARD_HEIGHT - 1 - dY);
                    ledPos.y += dX;
                }
                    break;
                case Direction::DOWN:
                {
                    ledPos.x += (PER_BOARD_WIDTH - 1 - dX);
                    ledPos.y += (PER_BOARD_HEIGHT - 1 - dY);
                }
                    break;
                case Direction::LEFT:
                {
                    ledPos.x += dY;
                    ledPos.y += (PER_BOARD_WIDTH - 1 - dX);
                }
                    break;
                default:
                    break;
                }
                director->getRenderer()->setMatrixOrder(ledPos.x,ledPos.y,order);
            }
        }
    }
    delete _boardOrder;
}
