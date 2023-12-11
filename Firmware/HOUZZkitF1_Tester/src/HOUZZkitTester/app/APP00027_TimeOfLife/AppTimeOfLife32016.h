#ifndef __APP_TimeOfLife_32016_H__
#define __APP_TimeOfLife_32016_H__


#include "AppTimeOfLife.h"

NS_DT_BEGIN


class AppTimeOfLifeLayer32016 : public AppTimeOfLifeLayer
{
public:

    bool initLayer() override;

    APP_LAYER_CREATE_WITH_APP(AppTimeOfLifeLayer32016,AppTimeOfLife);

};


NS_DT_END


#endif //__APP_TimeOfLife_32016_H__