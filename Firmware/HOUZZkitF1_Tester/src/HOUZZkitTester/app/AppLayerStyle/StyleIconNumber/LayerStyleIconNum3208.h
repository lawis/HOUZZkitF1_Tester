#ifndef __Layer_Style_Icon_Num_H__
#define __Layer_Style_Icon_Num_H__


#include "Dot2D/dot2d.h"
#include "HOUZZkitTester/app/SDTAppManager.h"
#include "DataStyleIconNum.h"

NS_DT_BEGIN

class LayerStyleIconNum3208Delegate
{
public:
  
    virtual void printAction(SpriteCanvas* canvas) = 0;
    
};

class LayerStyleIconNum3208: public Layer
{
private:
    

    StyleIconNumData* _data = nullptr;
    
    LayerStyleIconNum3208Delegate* _delegate = nullptr;

    bool _preHideIcon = false;

    int8_t _textPositonY = 2;

protected:
    
    FrameSprite *_icon = nullptr;
    
    TextSprite *_numberText = nullptr;

    

public:

    void numberUpdate(float dt);

    STATIC_CREATE(LayerStyleIconNum3208);

    bool initWithData(const uint8_t* buffer,uint32_t size,StyleIconNumData* data,LayerStyleIconNum3208Delegate* delegate = nullptr,uint32_t textWidth = 32);
    
    void setIcon(const uint8_t* buffer,uint32_t size);
    
    void setContentOffsetX(int16_t x);

    void setTextPositionY(int32_t y);

    void resetLayout();
};


NS_DT_END


#endif //__Layer_Style_Icon_Num_H__
