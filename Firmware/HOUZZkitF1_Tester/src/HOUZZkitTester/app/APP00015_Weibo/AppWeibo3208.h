#ifndef __APP_Weibo_3208_H__
#define __APP_Weibo_3208_H__


#include "AppWeibo.h"
#include "HOUZZkitTester/app/AppLayerStyle/StyleIconNumber/LayerStyleIconNum3208.h"

NS_DT_BEGIN


class AppWeiboLayer3208 : public AppWeiboLayer,public LayerStyleIconNum3208Delegate
{
protected:

    LayerStyleIconNum3208 *_layerStyle = nullptr;
public:

    bool initLayer() override;

    APP_LAYER_CREATE_WITH_APP(AppWeiboLayer3208,AppWeibo);

    void printAction(SpriteCanvas* canvas) override;

};


NS_DT_END


#endif //__APP_Weibo_3208_H__