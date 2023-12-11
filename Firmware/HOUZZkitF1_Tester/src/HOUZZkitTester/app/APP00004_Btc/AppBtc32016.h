#ifndef __APP_Btc_32016_H__
#define __APP_Btc_32016_H__


#include "AppBtc.h"

NS_DT_BEGIN

class AppBtcLayer32016 : public AppBtcLayer
{
public:

    bool initLayer() override;
    
    APP_LAYER_CREATE_WITH_APP(AppBtcLayer32016,AppBtc);

};


NS_DT_END


#endif //__APP_Btc_32016_H__
