#include "dtRenderer.h"
#include "../dtDirector.h"

NS_DT_BEGIN

void Renderer::initDotCanvas(uint16_t width,uint16_t height)
{
    _canvasSize.width = width;
    _canvasSize.height = height;
    _dotNums = width * height;
    if(_dotCanvas)
    {
        delete _dotCanvas;
    }
    _dotCanvas = (DTRGB *)malloc(sizeof(DTRGB) * _dotNums);
    memset(_dotCanvas, 0, sizeof(DTRGB) * _dotNums);
    if (_matrixIndex)
    {
        delete _matrixIndex;
    }
    _matrixIndex = (uint16_t* )malloc(sizeof(uint16_t) * _dotNums);
    for (int32_t i = 0; i < _dotNums; i++)
    {
        _matrixIndex[i] = i;
    }
}

DTRGB* Renderer::getDotCanvas()
{
    return _dotCanvas;
}

DTRGB* Renderer::getDotCanvas(uint16_t* width,uint16_t* height)
{
    *width = _canvasSize.width;
    *height = _canvasSize.height;
    return _dotCanvas;
}

void Renderer::clearDotCanvas(const DTRGB& color)
{
    for (uint16_t i = 0; i < _dotNums; i++)
    {
        _dotCanvas[i] = DTRGB(color.r,color.g,color.b);
    }
}

const Size& Renderer::getCanvasSize() const
{
    return _canvasSize;
}

uint16_t Renderer::getDotCount()
{
    return _canvasSize.width * _canvasSize.height;
}

void Renderer::setMatrixOrder(uint16_t x, uint16_t y,uint16_t order)
{
    if(x >= _canvasSize.width || y >= _canvasSize.height || order >= _canvasSize.width * _canvasSize.height)
    {
        return;
    }
    _matrixIndex[y * _canvasSize.width + x] = order;
}

uint16_t Renderer::XY(uint16_t x, uint16_t y)
{
    return _matrixIndex[y * _canvasSize.width + x];
}

Renderer::Renderer()
: _canvasSize(0,0)
{

}

Renderer::~Renderer()
{
    if(_dotCanvas)
    {
        delete _dotCanvas;
        _dotCanvas = nullptr;
    }
    if(_matrixIndex)
    {
        delete _matrixIndex;
        _matrixIndex = nullptr;
    }
}

void Renderer::drawDot(const Transform& transform,int32_t x,int32_t y,const DTRGB& c)
{
    //矩阵变换
    transform.transform(&x,&y);
    if(x>=0 && x<_canvasSize.width && y>=0 && y<_canvasSize.height)
    {
        _dotCanvas[XY(x,y)] = c;
    }
}

void Renderer::clear()
{
    this->clearDotCanvas(DTRGB(0,0,0));
}

void Renderer::render()
{
    Director::getInstance()->render();
}




NS_DT_END
