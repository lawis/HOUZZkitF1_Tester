#include "AppBtc808.h"
#include "Fonts/TomThumb.h"

NS_DT_BEGIN

bool AppBtcLayer808::initLayer()
{
    _priceText = TextSprite::create(Size(8,8),Size(32,8),"",TextSprite::TextAlign::TextAlignCenter,&TomThumb);
    _priceText->setTransparent(true);
    _priceText->setPosition(0,0);
    this->addChild(_priceText);
    // MoveBy *move1 = MoveBy::create(10,Vec2(-30,0));
    // Place *move2 = Place::create(Vec2(0,0));
    // Sequence *seq = Sequence::create(move1,move2,nullptr);
    // RepeatForever *rep = RepeatForever::create(seq);
    // _priceText->runAction(rep);

    this->customUpdate(0);
    this->schedule(DT_SCHEDULE_SELECTOR(AppBtcLayer808::customUpdate),10);
    this->schedule(DT_SCHEDULE_SELECTOR(AppBtcLayer808::textScroll),0.5);
    return true;
}

void AppBtcLayer808::customUpdate(float dt)
{
    SpriteCanvas *canvas = _priceText->getSpriteCanvas();
    canvas->canvasReset();
    canvas->setCursor(0,7);
    canvas->printf("%.2f",this->getData()->price);
}

void AppBtcLayer808::textScroll(float dt)
{
    _priceText->contentOffset.x --;
    if (-_priceText->contentOffset.x > _priceText->getContentSize().width)
    {
        _priceText->contentOffset.x = 0;
    }
}

NS_DT_END
