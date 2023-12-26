#ifndef __CORE_APP_INIT_808_H__
#define __CORE_APP_INIT_808_H__


#include "CoreAppSystemState.h"

NS_DT_BEGIN


class CoreAppSystemStateLayer808 : public CoreAppSystemStateLayer
{
public:
    
    FrameSprite *_icon;

    TextSprite* _text;
    
public:

    bool initLayer() override;

    void showBluetoothIcon() override;

    void showBluetoothCloseIcon() override;

    void showWifiIcon()  override;

    void showReset()  override;

    void showSetting() override;

    void showUpdate() override;

    void updateUpdate(float dt);
    
    APP_LAYER_CREATE_WITH_APP(CoreAppSystemStateLayer808,CoreAppSystemState);

};


NS_DT_END


#endif //__CORE_APP_INIT_808_H__
