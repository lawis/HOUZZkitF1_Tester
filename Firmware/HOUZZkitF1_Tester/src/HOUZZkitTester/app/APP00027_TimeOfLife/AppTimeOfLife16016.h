#ifndef __APP_TimeOfLife_16016_H__
#define __APP_TimeOfLife_16016_H__


#include "AppTimeOfLife.h"

NS_DT_BEGIN


class AppTimeOfLifeLayer16016 : public AppTimeOfLifeLayer
{
public:

    bool initLayer() override;

    APP_LAYER_CREATE_WITH_APP(AppTimeOfLifeLayer16016,AppTimeOfLife);

};


NS_DT_END


#endif //__APP_TimeOfLife_16016_H__