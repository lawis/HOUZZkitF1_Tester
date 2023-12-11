#ifndef __APP_TimeOfLife_3208_H__
#define __APP_TimeOfLife_3208_H__


#include "AppTimeOfLife.h"

NS_DT_BEGIN


class AppTimeOfLifeLayer3208 : public AppTimeOfLifeLayer
{
protected:

    TextSprite *_timeText;

    CanvasSprite *_lineCanvas;

public:

    bool initLayer() override;

    void customUpdate(float dt);

    APP_LAYER_CREATE_WITH_APP(AppTimeOfLifeLayer3208,AppTimeOfLife);

};


NS_DT_END


#endif //__APP_TimeOfLife_3208_H__