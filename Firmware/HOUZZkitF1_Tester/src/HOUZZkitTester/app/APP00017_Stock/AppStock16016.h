#ifndef __APP_Stock_16016_H__
#define __APP_Stock_16016_H__


#include "AppStock.h"

NS_DT_BEGIN


class AppStockLayer16016 : public AppStockLayer
{
public:

    bool initLayer() override;

    APP_LAYER_CREATE_WITH_APP(AppStockLayer16016,AppStock);

};


NS_DT_END


#endif //__APP_Stock_16016_H__