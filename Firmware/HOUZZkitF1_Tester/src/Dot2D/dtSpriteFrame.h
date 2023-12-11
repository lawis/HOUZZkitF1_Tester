#ifndef __DT_SPRITE_FRAME_H__
#define __DT_SPRITE_FRAME_H__

#include <string>
#include "base/dtMacros.h"
#include "base/dtRef.h"
#include "base/dtTypes.h"
#include "extended/DgfParser.h"

NS_DT_BEGIN

class SpriteFrame : public Ref , public DgfDecoder
{
private:



public:



public:
    static SpriteFrame* create();

    static SpriteFrame* create(const std::string& path);

    static SpriteFrame* create(const uint8_t* bitmap,uint32_t size);

    DTRGB colorWithXY(uint16_t x,uint16_t y) const;

    void setSaturationRate(float rate);
    
    bool init(const uint8_t* bitmap,uint32_t size);

protected:

    SpriteFrame();

    virtual ~SpriteFrame();

    

};

NS_DT_END



#endif //__DT_SPRITE_FRAME_H__
