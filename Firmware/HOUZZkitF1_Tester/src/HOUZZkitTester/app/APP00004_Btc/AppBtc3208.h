#ifndef __APP_Btc_3208_H__
#define __APP_Btc_3208_H__


#include "AppBtc.h"
#include "HOUZZkitTester/app/AppLayerStyle/StyleIconNumber/LayerStyleIconNum3208.h"

NS_DT_BEGIN

class AppBtcLayer3208 : public AppBtcLayer ,public LayerStyleIconNum3208Delegate
{
protected:
    
    LayerStyleIconNum3208 *_layerStyle = nullptr;
    
public:

    bool initLayer() override;

    APP_LAYER_CREATE_WITH_APP(AppBtcLayer3208,AppBtc);
    
    void printAction(SpriteCanvas* canvas) override;
    
};


NS_DT_END


#endif //__APP_Btc_3208_H__
