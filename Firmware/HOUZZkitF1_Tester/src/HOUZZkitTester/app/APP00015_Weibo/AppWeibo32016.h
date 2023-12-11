#ifndef __APP_Weibo_32016_H__
#define __APP_Weibo_32016_H__


#include "AppWeibo.h"

NS_DT_BEGIN


class AppWeiboLayer32016 : public AppWeiboLayer
{
public:

    bool initLayer() override;

    APP_LAYER_CREATE_WITH_APP(AppWeiboLayer32016,AppWeibo);

};


NS_DT_END


#endif //__APP_Weibo_32016_H__