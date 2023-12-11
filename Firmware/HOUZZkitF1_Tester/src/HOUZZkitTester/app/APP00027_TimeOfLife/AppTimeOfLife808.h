#ifndef __APP_TimeOfLife_808_H__
#define __APP_TimeOfLife_808_H__


#include "AppTimeOfLife.h"

NS_DT_BEGIN


class AppTimeOfLifeLayer808 : public AppTimeOfLifeLayer
{
public:

    bool initLayer() override;

    APP_LAYER_CREATE_WITH_APP(AppTimeOfLifeLayer808,AppTimeOfLife);

};


NS_DT_END


#endif //__APP_TimeOfLife_808_H__