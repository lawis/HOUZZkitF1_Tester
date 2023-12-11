#include "AppScoreBoard3208.h"
#include "Fonts/TomThumb.h"

NS_DT_BEGIN

bool AppScoreBoardLayer3208::initLayer()
{
    redSide = TextSprite::create(Size(12,5),Size(12,5),"",TextSprite::TextAlign::TextAlignRight,&TomThumb);
    redSide->getSpriteCanvas()->setTextColor(DTRGB(255,0,0));
    
    redSide->setTransparent(true);
    redSide->setPosition(3,2);
    this->addChild(redSide);

    blueSide = TextSprite::create(Size(12,5),Size(12,5),"",TextSprite::TextAlign::TextAlignLeft,&TomThumb);
    blueSide->getSpriteCanvas()->setTextColor(DTRGB(0,0,255));
    blueSide->setTransparent(true);
    blueSide->setPosition(18,2);
    this->addChild(blueSide);

    
    middle = CanvasSprite::create(2, 5);
    middle->setPosition(15,2);
    middle->getSpriteCanvas()->drawLine(0, 2, 1, 2, DTRGB(255, 255, 255));
    this->addChild(middle);

    winCountLine = CanvasSprite::create(32,1);
    winCountLine->setPosition(0,0);
    this->addChild(winCountLine);

    this->numberUpdate(0);
    this->schedule(DT_SCHEDULE_SELECTOR(AppScoreBoardLayer3208::numberUpdate),0.2);

    return true;
}


void AppScoreBoardLayer3208::numberUpdate(float dt)
{
    redSide->getSpriteCanvas()->canvasReset();
    redSide->getSpriteCanvas()->printf("%03d",this->getData()->redScore);
    
    blueSide->getSpriteCanvas()->canvasReset();
    blueSide->getSpriteCanvas()->printf("%03d",this->getData()->blueScore);

    winCountLine->getSpriteCanvas()->canvasReset();
    for (uint8_t i = 0; i < this->getData()->redWinCount; i++)
    {
        winCountLine->getSpriteCanvas()->drawLine(13-i*4,0,11-i*4,0,DTRGB(255,0,0));
    }
    
    for (uint8_t i = 0; i < this->getData()->blueWinCount; i++)
    {
        winCountLine->getSpriteCanvas()->drawLine(18+i*4,0,20+i*4,0,DTRGB(0,0,255));
    }

}

NS_DT_END
