#ifndef __APP_Stock_3208_H__
#define __APP_Stock_3208_H__


#include "AppStock.h"
#include "HOUZZkitTester/app/AppLayerStyle/StyleIconNumber/LayerStyleIconNum3208.h"

NS_DT_BEGIN


class AppStockLayer3208 : public AppStockLayer ,public LayerStyleIconNum3208Delegate
{
    protected:
    
    LayerStyleIconNum3208 *_layerStyle = nullptr;
    
public:

    bool initLayer() override;

    APP_LAYER_CREATE_WITH_APP(AppStockLayer3208,AppStock);

    void printAction(SpriteCanvas* canvas) override;
};


NS_DT_END


#endif //__APP_Stock_3208_H__