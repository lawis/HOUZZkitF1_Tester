#ifndef __SDT_APP_MANAGER_H__
#define __SDT_APP_MANAGER_H__

#include "Dot2D/dot2d.h"
#include "HOUZZkitTester/network/SDTProtocol.h"


struct AppTheme
{
public:
    dot2d::DTRGB bgColor;
    dot2d::DTRGB fgColor;
    dot2d::DTRGB textColor;
    
    INLINE void copyTheme(const AppTheme& theme)
    {
        bgColor = theme.bgColor;
        fgColor = theme.fgColor;
        textColor = theme.textColor;
    }

};

class AppDataList;
class AppBase;
class AppDataBase;
class AppScheduleBase;
class AppManager 
{
private:

    enum class AppPlayType
    {
        NONE    = 0,
        LIST    = 1,
        SINGLE  = 2
    };

    uint8_t _appListPlayPointer = UINT8_MAX;

    uint8_t _appPlayId = UINT8_MAX;

    dot2d::Vector<AppScheduleBase *> _appScheduleList;

    AppBase* _previewApp = nullptr;
    
    void* _systemApp = nullptr;
    
    AppDataList *_appDataList = nullptr;

    AppPlayType _appPlayType = AppPlayType::NONE;

protected:

    

public:

    enum class GlobalAppThemeType
    {
        CLASSICAL = 0,
        FIRE,
        PURPLEDREAM,
        FOREST,
        CLOUD,
        LAVA,
        OCEAN
    };
    
    AppTheme* globalAppTheme = nullptr;
    
private:

    bool playAppByData(AppDataBase* appData,bool autoSwith=false);

    bool findNextPlayableApp();

    bool findPreviousPlayableApp();

protected:

    void createDebugAppDataList();

public:

    static AppManager* getInstance();

    AppManager();

    ~AppManager();

    bool init();

    void setGlobalTheme(GlobalAppThemeType type);

    void setTheme(GlobalAppThemeType type,AppTheme* theme);
    
    bool initAppData();

    bool removeAppData();

    //xMessage Task Function

    bool mtfSystemNormalAction(const uint8_t* buffer,const uint32_t size);

    bool mtfSystemInitAction(const uint8_t* buffer,const uint32_t size);

    bool mtfShowPreviewApp(const uint8_t* buffer,const uint32_t size);

    bool mtfExitPreviewApp(const uint8_t* buffer,const uint32_t size);

    bool mtfPlayAppById(const uint8_t* buffer,const uint32_t size);

    bool mtfAddNewApp(const uint8_t* buffer,const uint32_t size);

    bool mtfDeleteApp(const uint8_t* buffer,const uint32_t size);

    bool mtfSaveConfigData(const uint8_t* buffer,const uint32_t size);

    bool mtfPlayNextPlayableApp(const uint8_t* buffer,const uint32_t size);

    bool mtfPlayPreviousPlayableApp(const uint8_t* buffer,const uint32_t size);

    bool mtfUpdateFirmware(const uint8_t* buffer,const uint32_t size);

    bool mtfReorderAppList(const uint8_t* buffer,const uint32_t size);

    //web Task Function
    bool wtfShowPreviewApp(const uint8_t* buffer,const uint32_t size,Protocols& proto);

    bool wtfExitPreviewApp(const uint8_t* buffer,const uint32_t size,Protocols& proto);

    bool wtfPlayAppById(const uint8_t* buffer,const uint32_t size,Protocols& proto);

    bool wtfAddNewApp(const uint8_t* buffer,const uint32_t size,Protocols& proto);

    bool wtfDeleteApp(const uint8_t* buffer,const uint32_t size,Protocols& proto);

    bool wtfPlayNextPlayableApp(const uint8_t* buffer,const uint32_t size,Protocols& proto);

    bool wtfPlayPreviousPlayableApp(const uint8_t* buffer,const uint32_t size,Protocols& proto);

    bool wtfUpdateFirmware(const uint8_t* buffer,const uint32_t size,Protocols& proto);

    bool wtfReorderAppList(const uint8_t* buffer,const uint32_t size,Protocols& proto);
    
    //mqtt Task Function
    bool mqtfPlayAppByName(const String& name);

    bool mqtfPlayAppByList();


    // void pause
    AppBase* getCurrentApp();

    void pauseCurrentAppEventDispatcher();

    void resumeCurrentAppEventDispatcher();



    bool systemShow();

    bool systemAppStartSchedule();

    void drawTempStatus();
    
    void playAppByIndex(uint8_t index);

    void playAppById(uint8_t id);

    void playNextPlayableApp(bool findFromHead = false);

    void playPreviousPlayableApp();

    void recoverPlayApp();

    uint8_t currentAppId();
    
    bool addSchedulerToList(AppScheduleBase *request);

    bool addAppSchduler(uint8_t id);

    bool deleteAppSchduler(uint8_t id);

    bool executeAppSchduleTask(uint8_t id);
    
    bool initRequestList();

    void showBluetoothIcon();

    void showWifiIcon();

    void showReset();

    void showSetting();

    void showUpdate();


    bool getAppListData(Protocols& proto);

    bool getAppData(uint8_t id,Protocols& proto);

    bool setAppData(uint8_t id,uint8_t* buffer,uint32_t size,Protocols& proto);

};

#endif //__SDT_APP_MANAGER_H__
