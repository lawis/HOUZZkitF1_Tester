#ifndef __APP_Douyin_32016_H__
#define __APP_Douyin_32016_H__


#include "AppDouyin.h"

NS_DT_BEGIN


class AppDouyinLayer32016 : public AppDouyinLayer
{
public:

    bool initLayer() override;

    APP_LAYER_CREATE_WITH_APP(AppDouyinLayer32016,AppDouyin);

};


NS_DT_END


#endif //__APP_Douyin_32016_H__