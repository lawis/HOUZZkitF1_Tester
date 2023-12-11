#ifndef __APP_LightIndex_3208_H__
#define __APP_LightIndex_3208_H__


#include "AppLightIndex.h"
#include "HOUZZkitTester/app/AppLayerStyle/StyleIconNumber/LayerStyleIconNum3208.h"

NS_DT_BEGIN


class AppLightIndexLayer3208 : public AppLightIndexLayer,public LayerStyleIconNum3208Delegate
{
protected:
    
    LayerStyleIconNum3208 *_layerStyle = nullptr;

public:

    bool initLayer() override;

    APP_LAYER_CREATE_WITH_APP(AppLightIndexLayer3208,AppLightIndex);

    void printAction(SpriteCanvas* canvas) override;

};


NS_DT_END


#endif //__APP_LightIndex_3208_H__