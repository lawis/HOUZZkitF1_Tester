#include "dtSpriteFrame.h"
#include <stdlib.h>
#include <string>
#include "dtDirector.h"
#include "platform/dtFileUtils.h"

NS_DT_BEGIN

SpriteFrame* SpriteFrame::create()
{
    SpriteFrame *spriteFrame = new SpriteFrame();
    if(spriteFrame)
    {
        spriteFrame->autorelease();
        return spriteFrame;
    }
    DT_SAFE_DELETE(spriteFrame);
    return nullptr;
}

SpriteFrame* SpriteFrame::create(const std::string& path)
{
    uint32_t size = 0;
    uint8_t* bitmap = FileUtils::getInstance()->readDataFromPath(path.c_str(),&size);
    SpriteFrame *frame = SpriteFrame::create(bitmap,size);
    free(bitmap);
    return frame;
}

SpriteFrame* SpriteFrame::create(const uint8_t* bitmap,uint32_t size)
{
    SpriteFrame *spriteFrame = new SpriteFrame();
    if(spriteFrame && spriteFrame->init(bitmap,size))
    {
        spriteFrame->autorelease();
        return spriteFrame;
    }
    DT_SAFE_DELETE(spriteFrame);
    return nullptr;
}

SpriteFrame::SpriteFrame()
{

}

SpriteFrame::~SpriteFrame()
{

}

DTRGB SpriteFrame::colorWithXY(uint16_t x,uint16_t y) const
{
    return DTRGB(getColor(x,y));
}

void SpriteFrame::setSaturationRate(float rate)
{
    for (uint8_t i = 0; i < this->colorCount(); i++)
    {
        DTRGB color = this->getColorByIndex(i);
        DTHSV chsv = rgb2hsv_approximate(color);
        chsv.s += (255 - chsv.s) * rate;
        DTRGB crgb = chsv;
        this->setColor(i,crgb.r,crgb.g,crgb.b);
    }
}

bool SpriteFrame::init(const uint8_t* bitmap,uint32_t size)
{
    return decode(bitmap,size);
}

NS_DT_END
