#ifndef __APP_Douyin_3208_H__
#define __APP_Douyin_3208_H__


#include "AppDouyin.h"
#include "HOUZZkitTester/app/AppLayerStyle/StyleIconNumber/LayerStyleIconNum3208.h"

NS_DT_BEGIN


class AppDouyinLayer3208 : public AppDouyinLayer,public LayerStyleIconNum3208Delegate
{
protected:

    LayerStyleIconNum3208 *_layerStyle = nullptr;

public:

    bool initLayer() override;

    APP_LAYER_CREATE_WITH_APP(AppDouyinLayer3208,AppDouyin);

    void printAction(SpriteCanvas* canvas) override;
};


NS_DT_END


#endif //__APP_Douyin_3208_H__