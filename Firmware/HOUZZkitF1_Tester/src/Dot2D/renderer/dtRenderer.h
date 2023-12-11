#ifndef __DT_RENDERER_H_
#define __DT_RENDERER_H_

#include <vector>

#include "../base/dtMacros.h"
#include "../base/dtTypes.h"
#include "../math/dtGeometry.h"
#include "../math/dtTransform.h"

/**
 * @addtogroup renderer
 * @{
 */

NS_DT_BEGIN

class Renderer
{
private:

    DTRGB* _dotCanvas = nullptr;
    
    Size _canvasSize;

    uint16_t _dotNums = 0;

    uint16_t* _matrixIndex = nullptr;

protected:

    void initDotCanvas(uint16_t width,uint16_t height);

    DTRGB* getDotCanvas();

    DTRGB* getDotCanvas(uint16_t* width,uint16_t* height);

    void clearDotCanvas(const DTRGB& color);

    const Size& getCanvasSize() const;

    uint16_t getDotCount();

    uint16_t XY(uint16_t x, uint16_t y);

    friend class Director;

public:

    Renderer();

    ~Renderer();

    void setMatrixOrder(uint16_t x, uint16_t y,uint16_t order);

    void drawDot(const Transform& transform,int32_t x,int32_t y,const DTRGB& c);

    void clear();

    void render();

    

};

NS_DT_END

/**
 end of support group
 @}
 */

#endif //__DT_RENDERER_H_