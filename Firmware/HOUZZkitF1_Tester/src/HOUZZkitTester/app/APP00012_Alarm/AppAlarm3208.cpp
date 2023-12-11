#include "AppAlarm3208.h"

NS_DT_BEGIN

const uint8_t icon_alarm[] PROGMEM =
{
    0x00,0x2d,
    0x44,0x47,0x46,0x11,0x08,0x08,0x43,0x00,
    0x00,0x00,0xff,0xff,0xff,0xf1,0xe1,0x04,
    0xff,0x27,0x00,0x2c,0x00,0x3c,0x00,0x00,
    0x08,0x08,0x80,0x00,0x01,0x40,0x06,0x90,
    0x06,0x90,0x06,0x90,0x1a,0xa4,0x6a,0xa9,
    0x15,0x54,0x02,0x80,0x96
};

const uint8_t icon_alarm_ringing[] PROGMEM = 
{
    0x00,0x51,
    0x44,0x47,0x46,0x11,0x08,0x08,0x43,0x00,
    0x00,0x00,0xff,0xff,0xff,0xf1,0xe1,0x04,
    0xff,0x27,0x00,0x2c,0x00,0x3c,0x00,0x00,
    0x08,0x08,0x80,0x00,0x01,0x40,0x06,0x90,
    0x06,0x90,0x06,0x90,0x1a,0xa4,0x6a,0xa9,
    0x15,0x54,0x02,0x80,0x2c,0x00,0x3c,0x02,
    0x07,0x03,0x01,0x40,0x20,0x00,0x00,0xa0,
    0x2c,0x00,0x3c,0x02,0x07,0x03,0x01,0x80,
    0x20,0x00,0x00,0x28,0x2c,0x00,0x3c,0x03,
    0x07,0x03,0x01,0x40,0x20,0x00,0x00,0x28,
    0x96
};


bool AppAlarmLayer3208::initLayer()
{
    _layerStyle = LayerStyleIconNum3208::create();
    _layerStyle->initWithData(icon_alarm+2,GetUint16(icon_alarm),this->getData(),this);
    this->addChild(_layerStyle);
    return true;
}

void AppAlarmLayer3208::printAction(SpriteCanvas* canvas)
{
    AppDataAlarm* data = this->getData();
    if (data->alarming) {
        canvas->setTextColor(DTRGB(255,0,0));
    }
    canvas->printf("%02d:%02d",data->hour,data->minute);
    // if (data->isCountFormat)
    // {
    //     String countStr = NumberFormat(data->fansCount,data->numberSize);
    //     canvas->printf("%s",countStr.c_str());
    // }else
    // {
    //     canvas->printf("%d",data->fansCount);
    // }
}


NS_DT_END
