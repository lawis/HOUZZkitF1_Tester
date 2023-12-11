#ifndef __APP_Youtube_3208_H__
#define __APP_Youtube_3208_H__


#include "AppYoutube.h"
#include "HOUZZkitTester/app/AppLayerStyle/StyleIconNumber/LayerStyleIconNum3208.h"

NS_DT_BEGIN


class AppYoutubeLayer3208 : public AppYoutubeLayer,public LayerStyleIconNum3208Delegate
{
protected:
    
    LayerStyleIconNum3208 *_layerStyle = nullptr;

public:

    bool initLayer() override;

    APP_LAYER_CREATE_WITH_APP(AppYoutubeLayer3208,AppYoutube);

    void printAction(SpriteCanvas* canvas) override;
};


NS_DT_END


#endif //__APP_Youtube_3208_H__
