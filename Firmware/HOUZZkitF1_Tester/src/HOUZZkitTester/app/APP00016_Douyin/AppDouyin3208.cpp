#include "AppDouyin3208.h"
#include "HOUZZkitTester/SDTIconLib.h"

NS_DT_BEGIN


const uint8_t icon_douyin[] PROGMEM =
{
    0x00,0x2d,
0x44,0x47,0x46,0x11,0x08,0x08,0x43,0x00,
0x00,0x00,0xff,0xff,0xff,0x01,0xff,0xff,
0xfe,0x00,0x54,0x2c,0x01,0x2c,0x00,0x00,
0x08,0x08,0x40,0x00,0x02,0x70,0x02,0x5c,
0x02,0x77,0x02,0x7c,0x26,0x70,0x92,0x70,
0x92,0x70,0x25,0xc0,0x96
};

bool AppDouyinLayer3208::initLayer()
{
    _layerStyle = LayerStyleIconNum3208::create();
    _layerStyle->initWithData(icon_douyin+2,GetUint16(icon_douyin),this->getData(),this);
    this->addChild(_layerStyle);
    return true;
}

void AppDouyinLayer3208::printAction(SpriteCanvas* canvas)
{
    AppDataDouyin* data = this->getData();
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
