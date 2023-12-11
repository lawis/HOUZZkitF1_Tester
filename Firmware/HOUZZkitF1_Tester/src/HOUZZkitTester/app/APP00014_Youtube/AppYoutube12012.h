#ifndef __APP_Youtube_12012_H__
#define __APP_Youtube_12012_H__


#include "AppYoutube.h"

NS_DT_BEGIN


class AppYoutubeLayer12012 : public AppYoutubeLayer
{
public:

    bool initLayer() override;

    APP_LAYER_CREATE_WITH_APP(AppYoutubeLayer12012,AppYoutube);

};


NS_DT_END


#endif //__APP_Youtube_12012_H__