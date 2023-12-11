#include "dtLayer.h"
#include "dtDirector.h"

NS_DT_BEGIN

Layer::Layer()
{
    setAnchorPoint(Vec2(0,0));
}

Layer::~Layer()
{

}

bool Layer::init()
{
    Size s = Director::getInstance()->getCanvasSize();
    return init(s.width,s.height);
}

bool Layer::init(uint16_t width,uint16_t height)
{
    setContentSize(Size(width,height));
    return true;
}

Layer *Layer::create()
{
    Layer *ret = new Layer();
    if (ret && ret->init())
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        DT_SAFE_DELETE(ret);
        return nullptr;
    }
}

Layer *Layer::create(uint16_t width,uint16_t height)
{
    Layer *ret = new Layer();
    if (ret && ret->init(width,height))
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        DT_SAFE_DELETE(ret);
        return nullptr;
    }
}

LayerColor::LayerColor()
{
    
}

LayerColor::~LayerColor()
{
    
}

bool LayerColor::init()
{
    Size s = Director::getInstance()->getCanvasSize();
    return initWithColor(DTRGB(0,0,0),s.width,s.height);
}

bool LayerColor::initWithColor(const DTRGB& color, uint16_t width, uint16_t height)
{
    if(Layer::init())
    {
        _displayedColor = color;
        setContentSize(Size(width,height));
        return true;
    }
    return false;
}

bool LayerColor::initWithColor(const DTRGB& color)
{
    Size s = Director::getInstance()->getCanvasSize();
    return initWithColor(color,s.width,s.height);
}

LayerColor* LayerColor::create()
{
    LayerColor* ret = new LayerColor();
    if (ret && ret->init())
    {
        ret->autorelease();
    }
    else
    {
        DT_SAFE_DELETE(ret);
    }
    return ret;
}

LayerColor* LayerColor::create(const DTRGB& color)
{
    LayerColor * layer = new LayerColor();
    if(layer && layer->initWithColor(color))
    {
        layer->autorelease();
        return layer;
    }
    DT_SAFE_DELETE(layer);
    return nullptr;
}

LayerColor* LayerColor::create(const DTRGB& color,uint16_t width,uint16_t height)
{
    LayerColor * layer = new LayerColor();
    if( layer && layer->initWithColor(color,width,height))
    {
        layer->autorelease();
        return layer;
    }
    DT_SAFE_DELETE(layer);
    return nullptr;
}

void LayerColor::draw(Renderer *renderer,const Transform& transform)
{
    for(int32_t x = 0;x<_contentSize.width;x++)
    {
        for(int32_t y = 0;y<_contentSize.height;y++)
        {
            renderer->drawDot(transform,x,y,_displayedColor);
        }
    }
}

NS_DT_END