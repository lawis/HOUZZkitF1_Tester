#include "LayerStyleIconNum3208.h"
#include "HOUZZkitTester/SDTIconLib.h"
#include "Fonts/TomThumb.h"

NS_DT_BEGIN

bool LayerStyleIconNum3208::initWithData(const uint8_t* buffer,uint32_t size,StyleIconNumData* data,LayerStyleIconNum3208Delegate* delegate,uint32_t textWidth)
{
    _data = data;
    _delegate = delegate;

    _icon = FrameSprite::create(buffer,size);
    this->addChild(_icon);
    _icon->setAutoSwitch(true);

    _numberText = TextSprite::create(Size(24,5),Size(textWidth,5),"",TextSprite::TextAlign::TextAlignCenter,&TomThumb);
    _numberText->setTransparent(true);
    _numberText->setPaddingLeft(1);
    _numberText->setPosition(8,_textPositonY);
    this->addChild(_numberText);
    
    this->numberUpdate(0);
    this->schedule(DT_SCHEDULE_SELECTOR(LayerStyleIconNum3208::numberUpdate),1);

    this->resetLayout();

    return true;
}

void LayerStyleIconNum3208::setIcon(const uint8_t* buffer,uint32_t size)
{
    _icon->getSpriteFrame()->init(buffer, size);
}

void LayerStyleIconNum3208::setContentOffsetX(int16_t x)
{
    _numberText->contentOffset.x = x;
}

void LayerStyleIconNum3208::setTextPositionY(int32_t y)
{
    _textPositonY = y;
    this->resetLayout();
}

void LayerStyleIconNum3208::numberUpdate(float dt)
{
    SpriteCanvas *canvas = _numberText->getSpriteCanvas();
    _numberText->setAutoScroll((TextSprite::ScrollType)_data->scrollType,3,0.2);
    canvas->setTextColor(_data->appTheme.textColor, _data->appTheme.bgColor);
    canvas->canvasReset();
    if (_delegate) {
        _delegate->printAction(canvas);
    }
    if (_preHideIcon != _data->hideIcon)
    {
        this->resetLayout();
    }
}

void LayerStyleIconNum3208::resetLayout()
{
    _preHideIcon = _data->hideIcon;
    _icon->setVisible(!_data->hideIcon);
    if (_data->hideIcon)
    {
        _numberText->setContentSize(Size(32,5));
        _numberText->setPosition(0,_textPositonY);
    }else
    {
        _numberText->setContentSize(Size(24,5));
        _numberText->setPosition(8,_textPositonY);
    }
}

NS_DT_END
