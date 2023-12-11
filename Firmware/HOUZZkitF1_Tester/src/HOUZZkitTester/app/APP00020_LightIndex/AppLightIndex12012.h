#ifndef __APP_LightIndex_12012_H__
#define __APP_LightIndex_12012_H__


#include "AppLightIndex.h"

NS_DT_BEGIN


class AppLightIndexLayer12012 : public AppLightIndexLayer
{
public:

    bool initLayer() override;

    APP_LAYER_CREATE_WITH_APP(AppLightIndexLayer12012,AppLightIndex);

};


NS_DT_END


#endif //__APP_LightIndex_12012_H__