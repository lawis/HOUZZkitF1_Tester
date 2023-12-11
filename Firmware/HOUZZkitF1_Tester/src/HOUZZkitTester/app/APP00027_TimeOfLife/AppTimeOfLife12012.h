#ifndef __APP_TimeOfLife_12012_H__
#define __APP_TimeOfLife_12012_H__


#include "AppTimeOfLife.h"

NS_DT_BEGIN


class AppTimeOfLifeLayer12012 : public AppTimeOfLifeLayer
{
public:

    bool initLayer() override;

    APP_LAYER_CREATE_WITH_APP(AppTimeOfLifeLayer12012,AppTimeOfLife);

};


NS_DT_END


#endif //__APP_TimeOfLife_12012_H__