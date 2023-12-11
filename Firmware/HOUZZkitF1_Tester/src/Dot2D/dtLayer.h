#ifndef __DT_LAYER_H__
#define __DT_LAYER_H__

#include "base/dtMacros.h"
#include "base/dtTypes.h"
#include "dtNode.h"

NS_DT_BEGIN

/**
 * @addtogroup _2d
 * @{
 */

class Layer : public Node
{
protected:

    Layer();

    virtual ~Layer();

    virtual bool init() override;

    virtual bool init(uint16_t width,uint16_t height);

public:

    static Layer *create();

    static Layer *create(uint16_t width,uint16_t height);

    virtual int8_t nodeClassType() override {return 2;}

};


class LayerColor : public Layer
{
protected:

    LayerColor();

    virtual ~LayerColor();
    
    bool init() override;

    bool initWithColor(const DTRGB& color, uint16_t width, uint16_t height);
    
    bool initWithColor(const DTRGB& color);

public:

    static LayerColor *create();

    static LayerColor *create(const DTRGB& color);

    static LayerColor *create(const DTRGB& color,uint16_t width,uint16_t height);

    virtual void draw(Renderer *renderer,const Transform& transform) override;

};

// end of _2d group
/// @}

NS_DT_END

#endif //__DT_LAYER_H__
