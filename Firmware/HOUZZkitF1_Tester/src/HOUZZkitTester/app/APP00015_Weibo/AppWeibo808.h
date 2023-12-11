#ifndef __APP_Weibo_808_H__
#define __APP_Weibo_808_H__


#include "AppWeibo.h"

NS_DT_BEGIN


class AppWeiboLayer808 : public AppWeiboLayer
{
public:

    bool initLayer() override;

    APP_LAYER_CREATE_WITH_APP(AppWeiboLayer808,AppWeibo);

};


NS_DT_END


#endif //__APP_Weibo_808_H__