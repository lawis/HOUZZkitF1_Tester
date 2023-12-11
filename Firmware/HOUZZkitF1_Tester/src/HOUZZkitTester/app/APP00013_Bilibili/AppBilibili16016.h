#ifndef __APP_Bilibili_16016_H__
#define __APP_Bilibili_16016_H__


#include "AppBilibili.h"

NS_DT_BEGIN


class AppBilibiliLayer16016 : public AppBilibiliLayer
{
public:

    bool initLayer() override;

    APP_LAYER_CREATE_WITH_APP(AppBilibiliLayer16016,AppBilibili);

};


NS_DT_END


#endif //__APP_Bilibili_16016_H__