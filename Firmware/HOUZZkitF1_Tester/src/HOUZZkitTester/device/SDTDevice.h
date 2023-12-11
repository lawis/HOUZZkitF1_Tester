#ifndef __SDT_DEVICE_H__
#define __SDT_DEVICE_H__

#include <Arduino.h>
#include "RTClib.h"
#include "LightDependentResistor.h"
#include "SHT3x.h"
#include "SDTButton.h"
#include "BT201lib.h"
#include "Ticker.h"

#define LDR_PIN             35
#define BASE_BTN_COUNT      4
#define BT201_ENABLE_PIN    2
// #define BT201_MUTE_PIN      15
#define POWER_DETECT_PIN    34
#define CHARGE_DETECT_PIN   25
#define BUZZER_PIN          15
#define BUZZER_CHANNEL 1

#define PS2_DAT             19  //MISO  19
#define PS2_CMD             23  //MOSI  23
#define PS2_SEL              4  //SS     4
#define PS2_CLK             18  //SLK   18

#define PS2_TEST_INTERVAL   10

enum DeviceType
{
    DT_RTC = 1,
    DT_SHT,
    DT_LDR,
    DT_PS2
};

class Device
{
private:

    uint8_t btnPins[BASE_BTN_COUNT] = {26,27,14,13};

    SDTButton* buttons[BASE_BTN_COUNT];

    Ticker *_buzzerTicker = nullptr;
    
public:

    RTC_DS1307 rtc;

    LightDependentResistor ldr;

    SHT3x sht;
    
    BT201 bt201;

private:

    void btInit();
    
public:

    Device();

    ~Device();

    bool initBtChip();

    bool resetBtChipName();

    bool begin();

    void loop();

    void btSpeakerEnable(bool e);

    void tfCardPlay(const std::string& f);

    void tfCardStop();

    void btStartBuzzer();

    void checkButtonClickOnly(bool e);

    uint8_t batteryPower();

    void buzzerBeep(uint32_t duration);

    void exitBuzzerBeep();

};

extern Device DEVICE;

#endif //__SDT_DEVICE_H__
