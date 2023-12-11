#ifndef __APP_Youtube_32016_H__
#define __APP_Youtube_32016_H__


#include "AppYoutube.h"

NS_DT_BEGIN


class AppYoutubeLayer32016 : public AppYoutubeLayer
{
public:

    bool initLayer() override;

    APP_LAYER_CREATE_WITH_APP(AppYoutubeLayer32016,AppYoutube);

};


NS_DT_END


#endif //__APP_Youtube_32016_H__