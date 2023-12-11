#include "AppChronograph3208.h"
#include "Fonts/TomThumb.h"

NS_DT_BEGIN

bool AppChronographLayer3208::initLayer()
{
    _timeText = TextSprite::create(Size(32,5),Size(32,5),"",TextSprite::TextAlign::TextAlignCenter,&TomThumb);
    _timeText->setTransparent(true);
    _timeText->setPosition(1,1);
    this->addChild(_timeText);

    _recordCanvas = CanvasSprite::create(32, 1);
    _recordCanvas->setPosition(0,7);
    this->addChild(_recordCanvas);

    this->scheduleUpdate();
    return true;
}

void AppChronographLayer3208::update(float dt)
{
    _timeText->getSpriteCanvas()->canvasReset();
    AppDataChronograph *data = this->getData();
    _timeText->getSpriteCanvas()->setTextColor(data->appTheme.textColor, data->appTheme.bgColor);
    int64_t diff = this->getData()->tempMillisecond;
    if (this->getData()->isRunning)
    {
        diff += millis() - this->getData()->startMillisecond;
    }else
    {
        if (this->getData()->recordPointer < this->getData()->recordNum)
        {
            diff = this->getData()->recordMillisecond[this->getData()->recordPointer];
        }
    }
    int64_t sec = diff / 1000;
    _timeText->getSpriteCanvas()->printf("%03lld:%02lld.%02lld",sec/60,sec%60,diff%1000/10);

    _recordCanvas->getSpriteCanvas()->canvasReset();
    for (int8_t i = 0; i <= this->getData()->recordNum; i++)
    {
        if (i == this->getData()->recordNum)
        {
            if (this->getData()->recordPointer == i)
            {
                _recordCanvas->getSpriteCanvas()->drawLine(1+4*i, 0, 3+4*i, 0, this->getData()->appTheme.curRecordFgColor);
            }else{
                _recordCanvas->getSpriteCanvas()->drawLine(1+4*i, 0, 3+4*i, 0, this->getData()->appTheme.curRecordBgColor);
            }
        }else
        {
            if (this->getData()->recordPointer == i)
            {
                _recordCanvas->getSpriteCanvas()->drawLine(1+4*i, 0, 3+4*i, 0, this->getData()->appTheme.recordFgColor);
            }else{
                _recordCanvas->getSpriteCanvas()->drawLine(1+4*i, 0, 3+4*i, 0, this->getData()->appTheme.recordBgColor);
            }
        }
    }

}

NS_DT_END