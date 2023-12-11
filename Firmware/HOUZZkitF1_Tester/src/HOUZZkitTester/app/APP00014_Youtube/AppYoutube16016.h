#ifndef __APP_Youtube_16016_H__
#define __APP_Youtube_16016_H__


#include "AppYoutube.h"

NS_DT_BEGIN


class AppYoutubeLayer16016 : public AppYoutubeLayer
{
public:

    bool initLayer() override;

    APP_LAYER_CREATE_WITH_APP(AppYoutubeLayer16016,AppYoutube);

};


NS_DT_END


#endif //__APP_Youtube_16016_H__