#ifndef __SDT_SYSTEM_H__
#define __SDT_SYSTEM_H__

#include <Arduino.h>
#include "Dot2D/dot2d.h"
#include "device/SDTDevice.h"

#include "data/GlobalDeviceData.h"
#include "data/GlobalConfigData.h"
#include "data/GlobalUserData.h"

#include "app/SDTAppManager.h"
#include "SDTConfig.h"

// #include "app/APP00001_Clock/AppClock.h"
#include "app/COREAPP_SystemState/CoreAppSystemState.h"

#ifndef TARGET_OS_MAC
#include "network/SDTNetManager.h"
#endif




class SDTSystem
{
public:

    enum class SystemState
    {
        BOOTING = -1,
        INIT = 0,
        NORMAL = 1
    };


    

private:

    GlobalDeviceData* _deviceData = nullptr;

    GlobalConfigData* _configData = nullptr;

    GlobalUserData* _userData = nullptr;

    AppManager* _appManager = nullptr;

    SystemState _systemState = SystemState::BOOTING;

    uint32_t _firmware_totoal_size = 0;

    uint32_t _firmware_download_size = 0;

    String _updateUrl;

    String _updateCertificate;

    uint8_t _updateBeginCount;

#ifndef TARGET_OS_MAC
    NetManager *_netManager = nullptr;
#endif

private:

    bool _isUpdating = false;

    void updateBlock()
    {
        while (_isUpdating)
        {
#ifndef TARGET_OS_MAC
            vTaskDelay(1000);
#endif
        }
    };
    
protected:

    void setDeviceName(const String& deviceName);

    void setUninitDevice(bool e);

    void setWifiSSID(const String& wifiSsid);

    void setWifiPWD(const String& wifiPwd);

    void setToken(const String& token);

    void saveUserData();

    void saveDeviceData();

    void setIsAutoZone(bool e);

    void setManualZone(int8_t v);

    void setAutoZone(int8_t v);

    void setMinuteOffset(int8_t v);

    void setNewDevice(bool e);

    void setBtSpeakerEnable(bool e);

    void setAutoBrighten(bool e);

    void setBrightness(uint8_t v);

    void setVolume(uint8_t v);

    void setTheme(uint8_t v);

    void setMqttServerIp(const String& ip);

    void setMqttServerPort(uint16_t port);

    void xMsgLoop();

    friend class dot2d::CoreAppSystemState;

    friend class NetManager;

    friend void setup();

public:
        
    SDTSystem();

    ~SDTSystem();

    static SDTSystem* getInstance();

    bool initSystemData();

    bool initFileSystem();

    bool initGlobalDeviceData();

    bool initGlobalConfigData();

    bool initGlobalUserData();

    const String& UDID(){return _deviceData->UDID;}

    const String& deviceName(){return _deviceData->deviceName;}

    const String& deviceModel(){return _deviceData->deviceModel;}

    const bool uninitDevice(){return _userData->uninitDevice;}

    const String& wifiSSID(){return _userData->wifiSsid;}

    const String& wifiPWD(){return _userData->wifiPwd;}

    const String& token(){return _userData->token;}

    DateTime now();

    bool isAutoZone(){return _configData->isAutoZone;}

    int8_t manualZone(){return _configData->manualZone;}

    int8_t autoZone(){return _configData->autoZone;}

    int8_t minuteOffset(){return _configData->minuteOffset;}

    bool newDevice(){return _deviceData->newDevice;}

    bool btSpeakerEnable(){return _configData->btSpeakerEnable;}

    bool autoBrighten(){return _configData->autoBrighten;}

    uint8_t brightness(){return _configData->brightness;}

    uint8_t volume(){return _configData->volume;}

    uint8_t theme(){return _configData->theme;}

    String& mqttServerIp(){return _configData->mqttServerIp;}

    uint16_t mqttServerPort(){return _configData->mqttServerPort;}

    
    bool initAppManager();

    bool initDevice();

    void resetSystemToFactoryState();

    void resetSystem();

    SystemState getSystemState(){return _systemState;}


    void initNetwork();

    SystemState resetState();

    bool initWifiSystem();

    bool initBleSystem();

    bool initMqttSystem();

    bool initWebSocket();

    void startHeartbeat();
    
    void loop();

    void networkLoop();

    void saveConfigData();

    void ESPReset();

    void setFirmwareTotoalSize(uint32_t s);

    void setFirmwareDownloadSize(uint32_t s);

    float getUpdatePercent();

    void updateFirmware(const char* url,const char* certificate);

    bool updateBegin();

};

#endif //__SDT_SYSTEM_H__
