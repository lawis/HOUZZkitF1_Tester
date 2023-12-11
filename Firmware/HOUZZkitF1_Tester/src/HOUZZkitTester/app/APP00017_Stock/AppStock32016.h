#ifndef __APP_Stock_32016_H__
#define __APP_Stock_32016_H__


#include "AppStock.h"

NS_DT_BEGIN


class AppStockLayer32016 : public AppStockLayer
{
public:

    bool initLayer() override;

    APP_LAYER_CREATE_WITH_APP(AppStockLayer32016,AppStock);

};


NS_DT_END


#endif //__APP_Stock_32016_H__