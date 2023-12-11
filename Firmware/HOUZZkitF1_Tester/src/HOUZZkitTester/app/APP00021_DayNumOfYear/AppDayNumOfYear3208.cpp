#include "AppDayNumOfYear3208.h"

NS_DT_BEGIN

const uint8_t icon_day_num_of_year[] PROGMEM =
{
    0x00,0x2b,
    0x44,0x47,0x46,0x11,0x08,0x08,0x43,0x00,
    0x00,0x00,0xff,0xff,0xff,0x15,0x55,0xff,
    0xff,0x11,0x03,0x2c,0x00,0x3c,0x01,0x00,
    0x07,0x08,0x40,0x00,0xff,0xfd,0x55,0x55,
    0xa9,0x56,0x55,0x5a,0x95,0x56,0x55,0xa9,
    0x55,0x55,0x96
};

bool AppDayNumOfYearLayer3208::initLayer()
{
    _layerStyle = LayerStyleIconNum3208::create();
    _layerStyle->initWithData(icon_day_num_of_year+2,GetUint16(icon_day_num_of_year),this->getData(),this);
    this->addChild(_layerStyle);
    // this->getData()->hideIcon = true;
    if (this->getData()->showAsPercent)
    {
        _layerStyle->setTextPositionY(1);
    }
    _preShowAsPercent = this->getData()->showAsPercent;

    _percentLine = CanvasSprite::create(20, 1);
    _percentLine->setPosition(10,7);
    this->addChild(_percentLine);

    return true;
}

void AppDayNumOfYearLayer3208::printAction(SpriteCanvas* canvas)
{
    if (_preShowAsPercent != this->getData()->showAsPercent)
    {
        if (this->getData()->showAsPercent)
        {
            _layerStyle->setTextPositionY(1);
        }else
        {
            _layerStyle->setTextPositionY(2);
        }
        _preShowAsPercent = this->getData()->showAsPercent;
        if (_percentLine) {
            _percentLine->getSpriteCanvas()->canvasReset();
        }
         
    }
    
    if (this->getData()->showAsPercent)
    {
        float linePercent = 0;
        float dotPercent = 0;
        canvas->printf("%02.2f%%",this->getApp()->percentOfYear(&linePercent,&dotPercent)*100);
        if (_percentLine) {
             _percentLine->getSpriteCanvas()->drawLine(0,0,19,0,DTRGB(30,30,30));
             _percentLine->getSpriteCanvas()->drawLine(0,0,linePercent*20-1,0,DTRGB(255,255,255));
            _percentLine->getSpriteCanvas()->drawPixel(linePercent*20, 0, DTRGB(30 + 225*dotPercent,30 + 225*dotPercent,30 + 225*dotPercent));
        }

    }else
    {
        canvas->printf("%d",this->getApp()->dayNum());
    }
    
}

NS_DT_END
