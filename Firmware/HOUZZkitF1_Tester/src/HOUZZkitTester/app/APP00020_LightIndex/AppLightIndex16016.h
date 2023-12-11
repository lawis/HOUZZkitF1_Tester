#ifndef __APP_LightIndex_16016_H__
#define __APP_LightIndex_16016_H__


#include "AppLightIndex.h"

NS_DT_BEGIN


class AppLightIndexLayer16016 : public AppLightIndexLayer
{
public:

    bool initLayer() override;

    APP_LAYER_CREATE_WITH_APP(AppLightIndexLayer16016,AppLightIndex);

};


NS_DT_END


#endif //__APP_LightIndex_16016_H__