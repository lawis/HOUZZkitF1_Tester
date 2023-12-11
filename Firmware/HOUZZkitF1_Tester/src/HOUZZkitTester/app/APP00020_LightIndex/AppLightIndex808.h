#ifndef __APP_LightIndex_808_H__
#define __APP_LightIndex_808_H__


#include "AppLightIndex.h"

NS_DT_BEGIN


class AppLightIndexLayer808 : public AppLightIndexLayer
{
public:

    bool initLayer() override;

    APP_LAYER_CREATE_WITH_APP(AppLightIndexLayer808,AppLightIndex);

};


NS_DT_END


#endif //__APP_LightIndex_808_H__