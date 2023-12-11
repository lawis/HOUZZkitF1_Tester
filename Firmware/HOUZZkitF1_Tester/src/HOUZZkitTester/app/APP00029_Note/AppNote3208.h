#ifndef __APP_Note_3208_H__
#define __APP_Note_3208_H__


#include "AppNote.h"
#include "Dot2D/extended/U8g2_For_Adafruit_GFX/U8g2_for_Adafruit_GFX.h"

NS_DT_BEGIN


class AppNoteLayer3208 : public AppNoteLayer
{
protected:

    U8G2_FOR_ADAFRUIT_GFX u8g2_for_adafruit_gfx;

    TextSprite *_timeText;

public:

    bool initLayer() override;

    int16_t alignmentCal(uint16_t width);

    APP_LAYER_CREATE_WITH_APP(AppNoteLayer3208,AppNote);

    void showUpdate(float dt);
};


NS_DT_END


#endif //__APP_Note_3208_H__