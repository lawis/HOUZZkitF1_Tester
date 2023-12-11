#ifndef __APP_Douyin_12012_H__
#define __APP_Douyin_12012_H__


#include "AppDouyin.h"

NS_DT_BEGIN


class AppDouyinLayer12012 : public AppDouyinLayer
{
public:

    bool initLayer() override;

    APP_LAYER_CREATE_WITH_APP(AppDouyinLayer12012,AppDouyin);

};


NS_DT_END


#endif //__APP_Douyin_12012_H__