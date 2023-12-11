#ifndef __APP_Bilibili_3208_H__
#define __APP_Bilibili_3208_H__


#include "AppBilibili.h"
#include "HOUZZkitTester/app/AppLayerStyle/StyleIconNumber/LayerStyleIconNum3208.h"

NS_DT_BEGIN


class AppBilibiliLayer3208 : public AppBilibiliLayer ,public LayerStyleIconNum3208Delegate
{
protected:

    LayerStyleIconNum3208 *_layerStyle = nullptr;

public:

    bool initLayer() override;

    APP_LAYER_CREATE_WITH_APP(AppBilibiliLayer3208,AppBilibili);
    
    void printAction(SpriteCanvas* canvas) override;
};


NS_DT_END


#endif //__APP_Bilibili_3208_H__
