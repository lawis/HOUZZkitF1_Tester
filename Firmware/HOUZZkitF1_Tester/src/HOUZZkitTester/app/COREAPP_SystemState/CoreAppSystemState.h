#ifndef __CORE_APP_INIT_H__
#define __CORE_APP_INIT_H__

#include "HOUZZkitTester/app/AppBase.h"
#include "HOUZZkitTester/data/SDTData.h"

#define LONG_PRESSED_BUTTON_CHECK_DURATION  800

NS_DT_BEGIN

class CoreAppDataSystemState : public AppDataTemplate<CoreAppDataSystemState,65535>
{
public:

protected:

    bool subEncode(SDTData::DataSourceType type) override;

    bool subDecode(SDTData::DataSourceType type) override;

public:

};

class CoreAppScheduleSystemState : public AppScheduleTemplate<CoreAppScheduleSystemState,CoreAppDataSystemState,65535>
{

public:

    bool parseRequest(const String& res){return false;};

    void scheduleAction(float dt){};
    
    ~CoreAppScheduleSystemState(){};

};



class CoreAppSystemState : public AppTemplate<CoreAppSystemState,CoreAppDataSystemState,CoreAppScheduleSystemState,65535>
{

private:

    bool btSwitchEnable = true;

    bool _offScreen = false;
    
    uint16_t btSwitchPressDuration = 0;

    FrameSprite *_statusSprite;

    bool _showTempStatus = false;

    bool _underAppControll = false;

private:

    void tempStatusTimer();

    void tempShowStatusEnd(float dt);

    void timeProofreadAction(float dt);

protected:

    bool init() override;

public:

    ~CoreAppSystemState();

    void onExit() override;

    void onEnter() override;

    void cleanup() override;

    void pause(void) override;

    void showBluetoothIcon(bool asTempStatus = false);

    void showBluetoothCloseIcon(bool asTempStatus = false);

    void showWifiIcon(bool asTempStatus = false);

    void showReset(bool asTempStatus = false);

    void showSetting(bool asTempStatus = false);

    void showUpdate();

    void switchBtSpeaker();

    void drawStatus();

    void startSchedule();

    bool timeProofreadParser(const String& res);

    void update(float dt) override;

    bool enterAppControll();

    bool exitAppControll();

};

class CoreAppSystemStateLayer : public AppLayerTemplate<CoreAppSystemStateLayer,CoreAppSystemState,CoreAppDataSystemState>
{
private:

    
protected:

    FrameSprite *_icon;

public:

    
    
protected:
    
    void customUpdate(float dt);

public:

    bool initLayer() override;
    
    virtual void showBluetoothIcon(){};

    virtual void showBluetoothCloseIcon(){};

    virtual void showWifiIcon(){};

    virtual void showReset(){};

    virtual void showSetting(){};

    virtual void showUpdate(){};
};


NS_DT_END


#endif //__CORE_APP_INIT_H__
