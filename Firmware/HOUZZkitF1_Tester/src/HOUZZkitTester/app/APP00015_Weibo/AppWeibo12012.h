#ifndef __APP_Weibo_12012_H__
#define __APP_Weibo_12012_H__


#include "AppWeibo.h"

NS_DT_BEGIN


class AppWeiboLayer12012 : public AppWeiboLayer
{
public:

    bool initLayer() override;

    APP_LAYER_CREATE_WITH_APP(AppWeiboLayer12012,AppWeibo);

};


NS_DT_END


#endif //__APP_Weibo_12012_H__