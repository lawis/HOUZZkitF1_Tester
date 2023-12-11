#include "AppTimeOfLife3208.h"
#include "Fonts/TomThumb.h"

NS_DT_BEGIN

bool AppTimeOfLifeLayer3208::initLayer()
{
    _timeText = TextSprite::create(Size(32,5),Size(32,5),"",TextSprite::TextAlign::TextAlignLeft,&TomThumb);
    _timeText->setTransparent(true);
    _timeText->setPosition(3,0);
    this->addChild(_timeText);

    _lineCanvas = CanvasSprite::create(32, 2);
    _lineCanvas->setPosition(0,6);
    this->addChild(_lineCanvas);

    this->customUpdate(0);
    this->schedule(DT_SCHEDULE_SELECTOR(AppTimeOfLifeLayer3208::customUpdate),1);

    return true;
}


void AppTimeOfLifeLayer3208::customUpdate(float dt)
{
    AppDataTimeOfLife *data = this->getData();
    _timeText->getSpriteCanvas()->canvasReset();
    _lineCanvas->getSpriteCanvas()->canvasReset();
    _timeText->getSpriteCanvas()->setTextColor(data->appTheme.textColor,data->appTheme.bgColor);
    
    uint16_t days = 0;
    uint8_t hours = 0;
    uint8_t minutes = 0;
    this->getApp()->timeLife(&days,&hours,&minutes);
    
    _timeText->getSpriteCanvas()->printf("%d",days);
    for (uint8_t i = 0; i<24; i++) {
        _timeText->getSpriteCanvas()->drawPixel(21+i%5, 0+i/5, data->appTheme.bgColor);
    }
    for (uint8_t i = 0; i<hours; i++) {
        _timeText->getSpriteCanvas()->drawPixel(21+i%5, 0+i/5, data->appTheme.textColor);
    }
    for (uint8_t i = 0; i<60; i++) {
        _lineCanvas->getSpriteCanvas()->drawPixel(1+i%30, 0+i/30, data->appTheme.bgColor);
    }
    for (uint8_t i = 0; i<minutes; i++) {
        _lineCanvas->getSpriteCanvas()->drawPixel(1+i%30, 0+i/30, data->appTheme.textColor);
    }

}

NS_DT_END
