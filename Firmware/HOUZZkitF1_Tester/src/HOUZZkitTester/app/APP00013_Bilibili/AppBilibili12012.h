#ifndef __APP_Bilibili_12012_H__
#define __APP_Bilibili_12012_H__


#include "AppBilibili.h"

NS_DT_BEGIN


class AppBilibiliLayer12012 : public AppBilibiliLayer
{
public:

    bool initLayer() override;

    APP_LAYER_CREATE_WITH_APP(AppBilibiliLayer12012,AppBilibili);

};


NS_DT_END


#endif //__APP_Bilibili_12012_H__