#ifndef __HFT_SCREEN_H__
#define __HFT_SCREEN_H__

#include <Arduino.h>
#include "FastLED.h"

#include <Arduino_GFX_Library.h>
#include <lvgl.h>
#include "ui.h"

#define LED_PIN             16
#define PANEL_COUNT         6

class LedMatrix;
class LcdDisplay;

enum FunctionStatus
{
    FS_CHECK_1 = 1,
    FS_CHECK_2,
    FS_CHECK_3,
    FS_CHECK_4,
    FS_CHECK_5,
    FS_PASS,
    FS_FAIL
};

enum FunctionList
{
    FL_SERVER_CONNECT = 1,
    FL_DEVICE_POWER_ON ,
    FL_DEVICE_CONNECTED,
    FL_DEVICE_ACTIVED,
    FL_HOST_USB,
};

class ScreenManager
{
public:

    LedMatrix* ledMatrix = nullptr;

    LcdDisplay* lcdDisplay = nullptr;

protected:

    bool init();

public:

    ScreenManager(){};

    ~ScreenManager(){};

    static ScreenManager* getInstance();

    void initScreen();

    void showDeviceStatus(FunctionList functionList,FunctionStatus functionStatus);

    void ledCheck();

};


class LedMatrix
{
private:

    struct CRGB* leds = nullptr;

public:

    LedMatrix(){};

    ~LedMatrix();

    void initMatrix();

    void setBrightness(uint8_t scale);// 0-100

    void close();

    void clear();

    void setPixel(int order, struct CRGB color);

    void show(){FastLED.show();}

    uint16_t getLedCount(){return 25;}

};

class LcdDisplay
{
private:
    

public:

    LcdDisplay(){};

    ~LcdDisplay(){};

    void initDisplay();

    void loop();

    void right_slide();

    void left_slide();
};

#endif //__HFT_SCREEN_H__
