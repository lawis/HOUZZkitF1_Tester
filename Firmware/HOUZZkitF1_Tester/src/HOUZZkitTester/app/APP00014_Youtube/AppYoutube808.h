#ifndef __APP_Youtube_808_H__
#define __APP_Youtube_808_H__


#include "AppYoutube.h"

NS_DT_BEGIN


class AppYoutubeLayer808 : public AppYoutubeLayer
{
public:

    bool initLayer() override;

    APP_LAYER_CREATE_WITH_APP(AppYoutubeLayer808,AppYoutube);

};


NS_DT_END


#endif //__APP_Youtube_808_H__