#ifndef __APP_Stock_808_H__
#define __APP_Stock_808_H__


#include "AppStock.h"

NS_DT_BEGIN


class AppStockLayer808 : public AppStockLayer
{
public:

    bool initLayer() override;

    APP_LAYER_CREATE_WITH_APP(AppStockLayer808,AppStock);

};


NS_DT_END


#endif //__APP_Stock_808_H__