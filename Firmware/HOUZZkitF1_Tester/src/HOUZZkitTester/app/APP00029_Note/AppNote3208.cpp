#include "AppNote3208.h"
#include "Fonts/TomThumb.h"

NS_DT_BEGIN

bool AppNoteLayer3208::initLayer()
{
    _timeText = TextSprite::create(Size(32,8),Size(32,8),"",TextSprite::TextAlign::TextAlignLeft,&TomThumb);
    _timeText->setTransparent(true);
    _timeText->setPosition(0,0);
    this->addChild(_timeText);

    u8g2_for_adafruit_gfx.begin((*_timeText->getSpriteCanvas()));

    u8g2_for_adafruit_gfx.setFontMode(0);                 // use u8g2 none transparent mode
    u8g2_for_adafruit_gfx.setFontDirection(0);            // left to right (this is default)
    u8g2_for_adafruit_gfx.setForegroundColor(65535);      // apply Adafruit GFX color
    u8g2_for_adafruit_gfx.setFont(u8g2_font_guanzhi8_t_gb2312);  // select u8g2 font from here: https://github.com/olikraus/u8g2/wiki/fntlistall

    this->schedule(DT_SCHEDULE_SELECTOR(AppNoteLayer3208::showUpdate), 0.2);
    return true;
}

int16_t AppNoteLayer3208::alignmentCal(uint16_t width)
{
    switch (this->getData()->alignment)
    {
    case 0:
        return 0;
        break;
    case 1:
        return (32 - width) / 2;
        break;
    case 2:
        return (32 - width);
        break;
    default:
        break;
    }
    return 0;
}

void AppNoteLayer3208::showUpdate(float dt)
{
    _timeText->getSpriteCanvas()->canvasReset();
    AppDataNote *data = this->getData();
    // _timeText->getSpriteCanvas()->setTextColor(data->appTheme.textColor, data->appTheme.bgColor);
    int16_t contentOffsetX = this->getData()->offsetX;
    uint16_t width = u8g2_for_adafruit_gfx.getUTF8Width(this->getData()->message.c_str());
    if (width > 32)
    {
        if (this->getData()->hasRefresh)
        {
            switch (this->getData()->scrollType)
            {
            case 0:
                contentOffsetX = alignmentCal(width);
                break;
            case 1:
                contentOffsetX = 10;
                break;
            case 2:
                contentOffsetX = 0;
                break;
            default:
                break;
            }
            this->getData()->hasRefresh = false;
        }
        switch (this->getData()->scrollType)
        {
        case 0:
            contentOffsetX = alignmentCal(width);
            break;
        case 1:
            contentOffsetX--;
            if (width + contentOffsetX < 32 - 10)
            {
                contentOffsetX = 10;
            }
            break;
        case 2:
            if (this->getData()->scrollForward)
            {
                contentOffsetX--;
                if (width + contentOffsetX < 32 - 5)
                {
                    this->getData()->scrollForward = false;
                }
                
            }else
            {
                contentOffsetX++;
                if (contentOffsetX > 5)
                {
                    this->getData()->scrollForward = true;
                }
            }
            break;
        default:
            break;
        }
        //滚动逻辑
    }else
    {
        contentOffsetX = alignmentCal(width);
    }
    u8g2_for_adafruit_gfx.setCursor(contentOffsetX,7);
    u8g2_for_adafruit_gfx.setForegroundColor(data->appTheme.textColor.u16());
    u8g2_for_adafruit_gfx.print(this->getData()->message.c_str());
    this->getData()->offsetX = contentOffsetX;
}


NS_DT_END