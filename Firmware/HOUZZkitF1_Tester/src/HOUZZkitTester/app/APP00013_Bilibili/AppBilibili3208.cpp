#include "AppBilibili3208.h"
#include "HOUZZkitTester/SDTIconLib.h"

NS_DT_BEGIN

bool AppBilibiliLayer3208::initLayer()
{
    _layerStyle = LayerStyleIconNum3208::create();
    _layerStyle->initWithData(icon_bilibili+2,GetUint16(icon_bilibili),this->getData(),this);
    this->addChild(_layerStyle);
    return true;
}

void AppBilibiliLayer3208::printAction(SpriteCanvas* canvas)
{
    AppDataBilibili* data = this->getData();
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
