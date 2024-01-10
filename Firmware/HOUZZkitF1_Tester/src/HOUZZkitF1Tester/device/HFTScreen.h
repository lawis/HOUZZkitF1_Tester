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
    FL_DEVICE_ACTIVATED,
    FL_HOST_USB,
    FL_DDR,
    FL_EMMC,
    FL_RTC,
    FL_IIC_ENCRYPT,
    FL_RS485,
    FL_RS232,
    FL_SSD,
    FL_TF,
    FL_GPIO,
    FL_PWM,
    FL_WAN,
    FL_LAN,
    FL_WIFI,
    FL_BLUETOOTH,
    FL_ZIGBEE,
    FL_HDMI,
    FL_IIC_INTERFACE,
    FL_MICROPHONE,
    FL_JACKPOT,
    FL_MIPI
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

    void setCheckerVersion(const String& ver);

    void setFirmwareVersion(const String& ver);

    void setWanMac(const String& mac);

    void setLanMac(const String& mac);

    void setSnCode(const String& sn);

    void setErrorCode(uint16_t code);

    void setProgress(uint8_t progress);

    void setProgressTitle(const String& title,bool showArrow = false);

    void showArrow(bool e);

    void showProgressTitle(bool e);

    void showProgressBar(bool e);

    void startCheck();

    void waitCheckerConnectNet();
};

#endif //__HFT_SCREEN_H__
