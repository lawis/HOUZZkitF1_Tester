#include "AppClock16016.h"

NS_DT_BEGIN

bool AppClockLayer16016::initLayer()
{
    _timeText = TextSprite::create(Size(16,16),Size(16,16),"",TextSprite::TextAlign::TextAlignCenter,&TomThumb);
    _timeText->setPaddingLeft(2);
    _timeText->getSpriteCanvas()->setTextHSpace(1);
    _timeText->getSpriteCanvas()->setTextVSpace(1);

    _timeText->setTransparent(true);
    _timeText->setPosition(0,0);
    this->addChild(_timeText);

    _timeSecond = CanvasSprite::create(16,16);
    _timeSecond->setTransparent(true);
    this->addChild(_timeSecond);

    DateTime now = DEVICE.rtc.now();
    uint16_t color = _timeText->getSpriteCanvas()->getTextcolor();
    for (size_t i = 0; i <= now.second(); i++)
    {
        this->drawSecond(i,color);
    }
    this->customUpdate(0);
    this->schedule(DT_SCHEDULE_SELECTOR(AppClockLayer16016::customUpdate),0.2);

    // DEVICE.bt201.loopPlayFolder("01");

    return true;
}


void AppClockLayer16016::drawSecond(int8_t s,const DTRGB& color)
{
    if (s>=0 && s < 15)
    {
        _timeSecond->getSpriteCanvas()->writePixel(s,0,color);
    }else if(s >=15 && s < 30)
    {
        _timeSecond->getSpriteCanvas()->writePixel(15,s-15,color);
    }else if(s >=30 && s < 45)
    {
        _timeSecond->getSpriteCanvas()->writePixel((45-s),15,color);
    }else if(s >=45 && s < 60)
    {
        _timeSecond->getSpriteCanvas()->writePixel(0,(60-s),color);
    }
}

void AppClockLayer16016::customUpdate(float dt)
{
    AppDataClock *data = this->getData();
    DateTime now = this->getApp()->now();

    // LOG("%04d/%02d/%02d %02d:%02d:%02d",now.year(),now.month(),now.day(),now.hour(),now.minute(),now.second());

    // LOG("Tem:%.2f Hum:%.2f",DEVICE.sht.GetTemperature(),DEVICE.sht.GetAbsHumidity());

    // LOG("LDR:%.2f",DEVICE.ldr.getCurrentLux());

    SpriteCanvas *canvas = _timeText->getSpriteCanvas();
    DTRGB color = canvas->getTextcolor();
    canvas->canvasReset();
    canvas->setCursor(0,7);
    if(data->is24HourView)
    {
        canvas->printf("%02d\n%02d",now.hour(),now.minute());
    }else
    {
        canvas->printf("%02d\n%02d",now.twelveHour(),now.minute());
    }

    _timeSecond->getSpriteCanvas()->canvasReset();
    if(data->isShowSecond)
    {
        for (uint8_t i = 0; i < 60; i++)
        {
            this->drawSecond(i,DTRGB(20,0,20));
        }
        uint8_t second = now.second();
        for (uint8_t i = 0; i <= second; i++)
        {
            this->drawSecond(i,color);
        }
    }
}

NS_DT_END
