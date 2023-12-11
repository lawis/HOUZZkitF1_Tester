#ifndef __APP_Stock_12012_H__
#define __APP_Stock_12012_H__


#include "AppStock.h"

NS_DT_BEGIN


class AppStockLayer12012 : public AppStockLayer
{
public:

    bool initLayer() override;

    APP_LAYER_CREATE_WITH_APP(AppStockLayer12012,AppStock);

};


NS_DT_END


#endif //__APP_Stock_12012_H__