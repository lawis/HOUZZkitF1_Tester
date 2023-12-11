#ifndef __APP_Bilibili_32016_H__
#define __APP_Bilibili_32016_H__


#include "AppBilibili.h"

NS_DT_BEGIN


class AppBilibiliLayer32016 : public AppBilibiliLayer
{
public:

    bool initLayer() override;

    APP_LAYER_CREATE_WITH_APP(AppBilibiliLayer32016,AppBilibili);

};


NS_DT_END


#endif //__APP_Bilibili_32016_H__