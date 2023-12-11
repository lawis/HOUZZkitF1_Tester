#ifndef __SDT_SCREEN_H__
#define __SDT_SCREEN_H__

#include <Arduino.h>
#include "Dot2D/dot2d.h"

#ifndef TARGET_OS_MAC
#include "FastLED.h"
#include "FastLED_NeoMatrix.h"
#endif

#define LED_PIN             32
#define PER_BOARD_WIDTH     4
#define PER_BOARD_HEIGHT    4
#define LED_POWER_PIN       33

class LedScreen;

class LedScreenManager
{
private:

    LedScreen* _ledScreen = nullptr;

protected:

    bool init();

public:

    LedScreenManager(){};

    ~LedScreenManager(){};

    static LedScreenManager* getInstance();

    void initScreen();

    void initMatrix(dot2d::DTRGB *data);

    void setBrightness(uint8_t scale);// 0-100

    void closeScreen();

    void clear();
#ifndef TARGET_OS_MAC
    INLINE void show(){FastLED.show();}
#else
    INLINE void show(){};
#endif

    dot2d::Size getMatrixSize();

    void screenSwitch(bool e);

};


class LedScreen
{
public:

    virtual void initScreen() = 0;

    virtual void initMatrix(dot2d::DTRGB *data) = 0;

    virtual dot2d::Size getMatrixSize() = 0;

};

class LedScreenClassical : public LedScreen
{

public:

    LedScreenClassical(){};

    ~LedScreenClassical(){};

    void initScreen() override;

    void initMatrix(dot2d::DTRGB *data) override;

    dot2d::Size getMatrixSize()override {return dot2d::Size(32,8);}
};


class LedScreenPlus : public LedScreen
{

private:

    uint8_t _boardWidth = 0;

    uint8_t _boardHeight = 0;

    uint8_t _boardCount = 0;

    uint8_t* _boardOrder = nullptr;

protected:



public:

    enum class Direction
    {
        NONE = -1,
        UP = 0,
        RIGHT = 1,
        DOWN = 2,
        LEFT = 3
    };

private:

    Direction relativeDirection(dot2d::Vec2& origin ,dot2d::Vec2& dest);

    dot2d::Vec2 getBoardPosByOrder(int16_t order);
    
public:

    LedScreenPlus(){};

    ~LedScreenPlus(){};

    void setBoardData(uint8_t matrixWidth,uint8_t matrixHeight,uint8_t* order);

    void initScreen() override;

    void initMatrix(dot2d::DTRGB *data) override;

    //prev 上一块板子自身的方向
    Direction findDirection(Direction prev,int16_t order);

    dot2d::Size getMatrixSize()override {return dot2d::Size(_boardWidth * PER_BOARD_WIDTH,_boardHeight * PER_BOARD_HEIGHT);}
};

#endif //__SDT_SCREEN_H__
