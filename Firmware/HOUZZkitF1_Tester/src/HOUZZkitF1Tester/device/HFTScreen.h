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
