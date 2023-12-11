#include "AppClock808.h"

NS_DT_BEGIN

bool AppClockLayer808::initLayer()
{
    _colorBg = LayerColor::create(DTRGB(0,0,0),8,8);
    this->addChild(_colorBg);

    _timeText = TextSprite::create(Size(8,5),Size(8,5),"",TextSprite::TextAlign::TextAlignLeft,&TomThumb);
    _timeText->setPaddingLeft(0);
    _timeText->getSpriteCanvas()->setTextHSpace(1);
    _timeText->setTransparent(true);
    _timeText->setPosition(0,3);
    this->addChild(_timeText);

    _timeSecond = CanvasSprite::create(6,2);
    _timeSecond->setTransparent(true);
    _timeSecond->setPosition(1,0);
    this->addChild(_timeSecond);

    this->customUpdate(0);
    this->schedule(DT_SCHEDULE_SELECTOR(AppClockLayer808::customUpdate),0.2);
    return true;
}

void AppClockLayer808::customUpdate(float dt)
{
    turnCounter++;
    if(turnCounter >=10)
    {
        showMonth = !showMonth;
        if (showMonth)
        {
            _colorBg->setColor(DTRGB(50,0,0));
        }else
        {
            _colorBg->setColor(DTRGB(0,0,0));
        }
        turnCounter = 0;
    }

    DateTime now = DEVICE.rtc.now();
    uint16_t color = _timeText->getSpriteCanvas()->getTextcolor();

    _timeText->getSpriteCanvas()->canvasReset();
    _timeSecond->getSpriteCanvas()->canvasReset();

    if(!showMonth)
    {
        _timeText->getSpriteCanvas()->printf("%02d",now.minute()); 
        uint8_t h = now.twelveHour();
        for (size_t i = 1; i <= h; i++)
        {
            _timeSecond->getSpriteCanvas()->drawPixel((i-1)%6,(i-1)/6,color);
        }
    }else
    {
        _timeText->getSpriteCanvas()->printf("%02d",now.day()); 
        uint8_t h = now.month();
        for (size_t i = 1; i <= h; i++)
        {
            _timeSecond->getSpriteCanvas()->drawPixel((i-1)%6,(i-1)/6,color);
        }
    }
    
}


NS_DT_END
