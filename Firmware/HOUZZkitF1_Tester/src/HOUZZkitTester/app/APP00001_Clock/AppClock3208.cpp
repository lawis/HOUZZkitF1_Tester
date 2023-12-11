#include "AppClock3208.h"

NS_DT_BEGIN

bool AppClockLayer3208::initLayer()
{
    _timeText = TextSprite::create(Size(32,5),Size(32,5),"",TextSprite::TextAlign::TextAlignCenter,&TomThumb);
    _timeText->setTransparent(true);
    _timeText->setPosition(1,1);
    this->addChild(_timeText);

    _weekCanvas = CanvasSprite::create(32, 1);
    _weekCanvas->setPosition(0,7);
    this->addChild(_weekCanvas);
    
    this->customUpdate(0);
    
    this->schedule(DT_SCHEDULE_SELECTOR(AppClockLayer3208::customUpdate),0.2);

    this->scheduleOnce(DT_SCHEDULE_SELECTOR(AppClockLayer3208::dateAndTimeSwitch),this->getData()->timeShowDuration);
    return true;
}

void AppClockLayer3208::dateAndTimeSwitch(float dt)
{
    _dateAndTimeSwitch = !_dateAndTimeSwitch;
    if (_dateAndTimeSwitch)
    {
        this->scheduleOnce(DT_SCHEDULE_SELECTOR(AppClockLayer3208::dateAndTimeSwitch),this->getData()->dateShowDuration);
    }else
    {
        this->scheduleOnce(DT_SCHEDULE_SELECTOR(AppClockLayer3208::dateAndTimeSwitch),this->getData()->timeShowDuration);
    }
    
}

void AppClockLayer3208::showDate(const DateTime& now,const AppDataClock& data)
{
    if(data.dayFormat == 2)
    {
        _timeText->getSpriteCanvas()->printf("%02d/%02d",now.day(),now.month());
    }else //if (data.dayFormat == 1)
    {
        _timeText->getSpriteCanvas()->printf("%02d/%02d",now.month(),now.day());
    }
}

void AppClockLayer3208::showTime(const DateTime& now,const AppDataClock& data)
{
    uint8_t h = now.hour();
    if (!data.is24HourView) {
        h = now.twelveHour();
    }
    if (data.isShowSecond) {
        _timeText->getSpriteCanvas()->printf("%02d:%02d:%02d",h,now.minute(),now.second());
    }else
    {
        if (_preSecond != now.second()) {
            _showColon = !_showColon;
            _preSecond = now.second();
        }
        if (_showColon) {
            _timeText->getSpriteCanvas()->printf("%02d:%02d",h,now.minute());
        }else
        {
            _timeText->getSpriteCanvas()->printf("%02d %02d",h,now.minute());
        }
    }
}

void AppClockLayer3208::showWeek(const DateTime& now,const AppDataClock& data)
{
    int8_t week = now.dayOfTheWeek();
    if (!data.isSundayFirstday) {week--;}
    if (week < 0) {week = 6;}
    for (uint8_t i = 0; i < 7; i++) {
        if (i == week) {
            _weekCanvas->getSpriteCanvas()->drawLine(3+4*i, 0, 5+4*i, 0, data.appTheme.weekFgColor);
        }else
        {
            _weekCanvas->getSpriteCanvas()->drawLine(3+4*i, 0, 5+4*i, 0, data.appTheme.weekBgColor);
        }
    }
}

void AppClockLayer3208::customUpdate(float dt)
{
    AppDataClock *data = this->getData();
    DateTime now = this->getApp()->now();
    
    _timeText->getSpriteCanvas()->canvasReset();
    _weekCanvas->getSpriteCanvas()->canvasReset();
    _timeText->getSpriteCanvas()->setTextColor(data->appTheme.textColor, data->appTheme.bgColor);
    
    if (_dateAndTimeSwitch && data->isShowDate)
    {
        this->showDate(now,*data);
    }else
    {
        this->showTime(now,*data);
    }
    if (data->isShowWeek)
    {
        this->showWeek(now,*data);
    }
}


NS_DT_END
