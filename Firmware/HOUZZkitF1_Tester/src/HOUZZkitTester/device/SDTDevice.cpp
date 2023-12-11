#include "SDTDevice.h"
#include "../../Dot2D/dot2d.h"
#include "SPI.h"
#include "../SDTConfig.h"
#include "../SDTSystem.h"

Device DEVICE;

void click(void *btn)
{
    SDTButton *button = (SDTButton *)btn;
    dot2d::EventButton event(button->type(),dot2d::EventButton::ButtonEventCode::BEC_CLICK);
    auto dispatcher = dot2d::Director::getInstance()->getEventDispatcher();
    dispatcher->dispatchEvent(&event);
    
}

void doubleClick(void *btn)
{
    SDTButton *button = (SDTButton *)btn;
    dot2d::EventButton event(button->type(),dot2d::EventButton::ButtonEventCode::BEC_DOUBLE_CLICK);
    auto dispatcher = dot2d::Director::getInstance()->getEventDispatcher();
    dispatcher->dispatchEvent(&event);
}

void longPressStart(void *btn)
{
    SDTButton *button = (SDTButton *)btn;
    dot2d::EventButton event(button->type(),dot2d::EventButton::ButtonEventCode::BEC_LONG_PRESS_START);
    auto dispatcher = dot2d::Director::getInstance()->getEventDispatcher();
    dispatcher->dispatchEvent(&event);
}

void longPressStop(void *btn)
{
    SDTButton *button = (SDTButton *)btn;
    dot2d::EventButton event(button->type(),dot2d::EventButton::ButtonEventCode::BEC_LONG_PRESS_STOP);
    auto dispatcher = dot2d::Director::getInstance()->getEventDispatcher();
    dispatcher->dispatchEvent(&event);
}

void duringLongPress(void *btn)
{
    SDTButton *button = (SDTButton *)btn;
    dot2d::EventButton event(button->type(),dot2d::EventButton::ButtonEventCode::BEC_DURING_LONG_PRESS);
    auto dispatcher = dot2d::Director::getInstance()->getEventDispatcher();
    dispatcher->dispatchEvent(&event);
}

void Device::btInit()
{
    // bt201.begin(BT201_ENABLE_PIN,BT201_MUTE_PIN,Serial2);
    delay(200);
}

void buzzerStop()
{
    ledcDetachPin(BUZZER_PIN);
}

void exitBuzzer()
{
    ledcAttachPin(BUZZER_PIN, BUZZER_CHANNEL);
    ledcWriteNote(BUZZER_CHANNEL, NOTE_B, 4);
    delay(100);
    ledcDetachPin(BUZZER_PIN);
    delay(100);
    ledcAttachPin(BUZZER_PIN, BUZZER_CHANNEL);
    ledcWriteNote(BUZZER_CHANNEL, NOTE_B, 4);
    delay(100);
    ledcDetachPin(BUZZER_PIN);
}

Device::Device()
:rtc()
,ldr(LDR_PIN, 2000, LightDependentResistor::GL5537_2)
,sht()
{
    ldr.setPhotocellPositionOnGround(false);
    for (int8_t i = 0; i < BASE_BTN_COUNT; i++)
    {
        buttons[i] = new SDTButton(btnPins[i],true,true);
        switch (i)
        {
        case 0:
            buttons[i]->setBtnType(BBT_LEFT);
            break;
        case 1:
            buttons[i]->setBtnType(BBT_ENTER);
            break;
        case 2:
            buttons[i]->setBtnType(BBT_RIGHT);
            break;
        case 3:
            buttons[i]->setBtnType(BBT_BACK);
            break;
        default:
            break;
        }
        buttons[i]->attachClick(click,buttons[i]);
        buttons[i]->attachDoubleClick(doubleClick,buttons[i]);
        buttons[i]->attachLongPressStart(longPressStart,buttons[i]);
        buttons[i]->attachLongPressStop(longPressStop,buttons[i]);
        buttons[i]->attachDuringLongPress(duringLongPress,buttons[i]);
    }
    pinMode(POWER_DETECT_PIN,INPUT);
    pinMode(CHARGE_DETECT_PIN,INPUT);
    pinMode(BUZZER_PIN,OUTPUT);
    digitalWrite(BUZZER_PIN,LOW);

    _buzzerTicker = new Ticker();
}

Device::~Device()
{
    for (int8_t i = 0; i < BASE_BTN_COUNT; i++)
    {
        delete buttons[i];
    }
}

bool Device::initBtChip()
{
    // bt201.begin(BT201_ENABLE_PIN,BT201_MUTE_PIN,Serial2);
    // delay(1000);
    // bt201.setPromptTone(0);
    // delay(200);
    // bt201.setBTCall(0);
    // delay(200);
    // bt201.setBTBle(0);
    // delay(200);
    // bt201.setBTName(SDTSystem::getInstance()->deviceName().c_str());
    // delay(200);
    // bt201.setMusicTriggerMode(1);
    // delay(200);
    // bt201.softReset();
    // delay(200);
    // bt201.end();
    return true;
}

bool Device::resetBtChipName()
{
    // bt201.begin(BT201_ENABLE_PIN,BT201_MUTE_PIN,Serial2);
    // delay(1000);
    // bt201.setBTName(SDTSystem::getInstance()->deviceName().c_str());
    // delay(200);
    return true;
}

bool Device::begin()
{
    //初始化时钟模块
    if(rtc.begin())
    {
        if (!rtc.isrunning())
		{
			rtc.adjust(DateTime(2020,9,6,12,00,00));
		}
    }else
    {
        LOG("ERROR!!! RTC BOOT ERROR");
    }

    //初始化温湿度模块
    sht.Begin();

    //初始化音频模块
    // this->btInit();

    return true;
}

void Device::loop()
{
    for (int8_t i = 0; i < BASE_BTN_COUNT; i++)
    {
        buttons[i]->tick();
    }
    // if (_ps2xEnable && this->ps2Available() && _ps2LoopDelay <= 0)
    // {
    //     ps2x.read_gamepad(false, 0);
    //     for (size_t i = 0; i < 20; i++)
    //     {
    //         _ps2LoopDelay = PS2_TEST_INTERVAL;
    //         if(ps2x.Button(psButtonConstants[i]))
    //         {
    //             uint8_t btnAnalog = 0;
    //             if (_ps2xPressures && psButtonAnalogConstants[i])
    //             {
    //                 btnAnalog = ps2x.Analog(psButtonAnalogConstants[i]);
    //             }
    //             dot2d::EventPS2 event(psButtonConstants[i],btnAnalog,dot2d::EventPS2::PS2EventCode::PEC_CLICK);
    //             auto dispatcher = dot2d::Director::getInstance()->getEventDispatcher();
    //             dispatcher->dispatchEvent(&event);
    //         }
    //     }

    //     if(_ps2JoystickEnable)
    //     {
    //         for (size_t i = 0; i < 4; i++)
    //         {
    //             uint8_t joystickAnalog = ps2x.Analog(psJoystickAnalogConstants[i]);
    //             dot2d::EventPS2 event(psJoystickAnalogConstants[i],joystickAnalog,dot2d::EventPS2::PS2EventCode::PEC_JOYSTICK);
    //             auto dispatcher = dot2d::Director::getInstance()->getEventDispatcher();
    //             dispatcher->dispatchEvent(&event);
    //         }
    //     }
    // }
    // int32_t now = millis();
    // _ps2LoopDelay -= (now - _ps2LoopLast);
    // _ps2LoopLast = now;
    // bt201.loop();
}

void Device::btSpeakerEnable(bool e)
{
    bt201.hardwareMute(true);
    bt201.setBTEdr(e?1:0);
    delay(200);
    bt201.softReset();
    delay(1000);
    bt201.hardwareMute(false);
}

#ifdef DEVICE_AIR

bool deleteBuzzerTask = false;

void buzzer(void *pvParameters)
{
  for (size_t i = 0; i < 60; i++)
  {
      ledcAttachPin(BUZZER_PIN, BUZZER_CHANNEL);
      ledcWriteNote(BUZZER_CHANNEL, NOTE_B, 4);
      delay(200);
      ledcWriteNote(BUZZER_CHANNEL, NOTE_B, 4);
      delay(200);
      ledcDetachPin(BUZZER_PIN);
      delay(600);
      if (deleteBuzzerTask)
      {
          vTaskDelete(NULL);
      }
  }
  vTaskDelete(NULL);
}

void btBuzzer(void *pvParameters)
{
    ledcAttachPin(BUZZER_PIN, BUZZER_CHANNEL);
    ledcWriteNote(BUZZER_CHANNEL, NOTE_C, 4);
    delay(150);
    ledcWriteNote(BUZZER_CHANNEL, NOTE_G, 4);
    delay(150);
    ledcWriteNote(BUZZER_CHANNEL, NOTE_A, 4);
    delay(150);
    ledcDetachPin(BUZZER_PIN);
    delay(600);
    vTaskDelete(NULL);
}

#endif


void Device::tfCardPlay(const std::string& f)
{
#ifdef DEVICE_AIR
    deleteBuzzerTask = false;
    xTaskCreatePinnedToCore(buzzer,"buzzer",4096,NULL,1,NULL,0);
#else
    bt201.setMode(4);
    delay(200);
    bt201.setVolume(12);
    delay(200);
    bt201.localPlayByPath(f);
#endif
}

void Device::tfCardStop()
{
#ifdef DEVICE_AIR
    deleteBuzzerTask = true;
#else
    bt201.localPlayMode(0);
#endif
}

void Device::btStartBuzzer()
{
#ifdef DEVICE_AIR
    xTaskCreatePinnedToCore(btBuzzer,"btBuzzer",4096,NULL,1,NULL,0);
#endif
}

void Device::checkButtonClickOnly(bool e)
{
    if (e)
    {
        for (int8_t i = 0; i < BASE_BTN_COUNT; i++)
        {
            buttons[i]->setClickTicks(50);
        }
    }else
    {
        for (int8_t i = 0; i < BASE_BTN_COUNT; i++)
        {
            buttons[i]->setClickTicks(400);
        }
    }
}


uint8_t Device::batteryPower()
{
    uint16_t analog = analogRead(POWER_DETECT_PIN);
    return map(analog,0,1024,0,100);
}

void Device::buzzerBeep(uint32_t duration)
{
    ledcAttachPin(BUZZER_PIN, BUZZER_CHANNEL);
    ledcWriteNote(BUZZER_CHANNEL, NOTE_B, 4);
    _buzzerTicker->once_ms(duration,buzzerStop);
}

void Device::exitBuzzerBeep()
{
    _buzzerTicker->once(0,exitBuzzer);
}