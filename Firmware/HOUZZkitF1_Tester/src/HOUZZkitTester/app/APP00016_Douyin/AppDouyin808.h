#ifndef __APP_Douyin_808_H__
#define __APP_Douyin_808_H__


#include "AppDouyin.h"

NS_DT_BEGIN


class AppDouyinLayer808 : public AppDouyinLayer
{
public:

    bool initLayer() override;

    APP_LAYER_CREATE_WITH_APP(AppDouyinLayer808,AppDouyin);

};


NS_DT_END


#endif //__APP_Douyin_808_H__