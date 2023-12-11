#ifndef __APP_Bilibili_808_H__
#define __APP_Bilibili_808_H__


#include "AppBilibili.h"

NS_DT_BEGIN


class AppBilibiliLayer808 : public AppBilibiliLayer
{
public:

    bool initLayer() override;

    APP_LAYER_CREATE_WITH_APP(AppBilibiliLayer808,AppBilibili);

};


NS_DT_END


#endif //__APP_Bilibili_808_H__