#ifndef __APP_Btc_808_H__
#define __APP_Btc_808_H__


#include "AppBtc.h"

NS_DT_BEGIN


class AppBtcLayer808 : public AppBtcLayer
{
public:

    TextSprite *_priceText;

    bool initLayer() override;
    
    APP_LAYER_CREATE_WITH_APP(AppBtcLayer808,AppBtc);

    void customUpdate(float dt);

    void textScroll(float dt);
};


NS_DT_END


#endif //__APP_Btc_808_H__
