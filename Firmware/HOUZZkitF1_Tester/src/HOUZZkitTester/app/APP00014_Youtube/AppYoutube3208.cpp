#include "AppYoutube3208.h"
#include "HOUZZkitTester/SDTIconLib.h"
#include "Fonts/TomThumb.h"

NS_DT_BEGIN

bool AppYoutubeLayer3208::initLayer()
{
    _layerStyle = LayerStyleIconNum3208::create();
    _layerStyle->initWithData(icon_youtube+2,GetUint16(icon_youtube),this->getData(),this);
    this->addChild(_layerStyle);
    return true;
}

void AppYoutubeLayer3208::printAction(SpriteCanvas* canvas)
{
    AppDataYoutube* data = this->getData();
    if (data->isCountFormat)
    {
        String countStr = NumberFormat(data->fansCount,data->numberSize);
        canvas->printf("%s",countStr.c_str());
    }else
    {
        canvas->printf("%d",data->fansCount);
    }
}

NS_DT_END
