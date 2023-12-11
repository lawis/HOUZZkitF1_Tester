#ifndef __APP_Btc_12012_H__
#define __APP_Btc_12012_H__


#include "AppBtc.h"

NS_DT_BEGIN

class AppBtcLayer12012 : public AppBtcLayer
{
public:

    bool initLayer() override;
    
    APP_LAYER_CREATE_WITH_APP(AppBtcLayer12012,AppBtc);

};


NS_DT_END


#endif //__APP_Btc_12012_H__
