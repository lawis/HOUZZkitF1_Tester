#ifndef __APP_Btc_16016_H__
#define __APP_Btc_16016_H__


#include "AppBtc.h"

NS_DT_BEGIN

class AppBtcLayer16016: public AppBtcLayer
{
public:

    bool initLayer() override;
    
    APP_LAYER_CREATE_WITH_APP(AppBtcLayer16016,AppBtc);

};


NS_DT_END


#endif //__APP_Btc_16016_H__
