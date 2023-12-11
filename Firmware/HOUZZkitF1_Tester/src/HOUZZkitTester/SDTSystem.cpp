#include "SDTSystem.h"
#include "device/SDTScreen.h"
#ifndef TARGET_OS_MAC
#include "Ticker.h"
#include "HttpsOTAUpdateT0.h"
#endif

static SDTSystem *s_sdtSystem = nullptr;

SDTSystem::SDTSystem()
{
    
}

SDTSystem::~SDTSystem()
{
    DT_SAFE_RELEASE(_configData);
    DT_SAFE_RELEASE(_userData);
    DT_SAFE_RELEASE(_deviceData);
}

void SDTSystem::setDeviceName(const String& deviceName)
{
    if (deviceName.length()<=0)
    {
        return;
    }
    _deviceData->deviceName = deviceName;
#ifndef TARGET_OS_MAC
    DEVICE.bt201.setBTName(deviceName.c_str());
    delay(200);
#endif
}

void SDTSystem::setUninitDevice(bool e)
{
    _userData->uninitDevice = e;
}

void SDTSystem::setWifiSSID(const String& wifiSsid)
{
    _userData->wifiSsid = wifiSsid;
}

void SDTSystem::setWifiPWD(const String& wifiPwd)
{
    _userData->wifiPwd = wifiPwd;
}

void SDTSystem::setToken(const String& token)
{
     _userData->token = token;
}

void SDTSystem::saveUserData()
{
    _userData->writeToKey(PREFERENCES_USER_DATA);
}

void SDTSystem::saveDeviceData()
{
    _deviceData->writeToKey(PREFERENCES_DEVICE_DATA);
}

void SDTSystem::saveConfigData()
{
    _configData->writeToKey(PREFERENCES_CONFIG_DATA);
}

void SDTSystem::setIsAutoZone(bool e)
{
    _configData->isAutoZone = e;
}

void SDTSystem::setManualZone(int8_t v)
{
    _configData->manualZone = v;
}

void SDTSystem::setAutoZone(int8_t v)
{
    _configData->autoZone = v;
}

void SDTSystem::setMinuteOffset(int8_t v)
{
    _configData->minuteOffset = v;
}

void SDTSystem::setNewDevice(bool e)
{
    _deviceData->newDevice = e;
}

void SDTSystem::setBtSpeakerEnable(bool e)
{
    _configData->btSpeakerEnable = e;
}

void SDTSystem::setAutoBrighten(bool e)
{
    _configData->autoBrighten = e;
}

void SDTSystem::setBrightness(uint8_t v)
{
    _configData->brightness = v>100?100:v;
    LedScreenManager::getInstance()->setBrightness(_configData->brightness);
}

void SDTSystem::setVolume(uint8_t v)
{
    _configData->volume = v>30?30:v;
}

void SDTSystem::setTheme(uint8_t v)
{
    _configData->theme = v;
    AppManager::getInstance()->setGlobalTheme((AppManager::GlobalAppThemeType)v);
}

void SDTSystem::setMqttServerIp(const String& ip)
{
    _configData->mqttServerIp = ip;
}

void SDTSystem::setMqttServerPort(uint16_t port)
{
    _configData->mqttServerPort = port;
}

void SDTSystem::xMsgLoop()
{
#ifndef TARGET_OS_MAC
    xMESSAGE msg;
    if (NET_QUEUE_RECEIVE(&msg))
    {
        return;
    }
    (_appManager->*msg.handle)(msg.buffer,msg.size);
    msg.freeBuffer();
#endif
}

SDTSystem* SDTSystem::getInstance()
{
    if (s_sdtSystem == nullptr)
    {
        s_sdtSystem = new SDTSystem();
    }
    return s_sdtSystem;
}

bool SDTSystem::initSystemData()
{
    //----------------开启 SPIFFS 文件系统----------------
    if(this->initFileSystem())
    {
#ifndef TARGET_OS_MAC
        Serial.printf("------SPIFFS File System init succeed.\n");
#endif
    }else
    {
#ifndef TARGET_OS_MAC
        Serial.printf("---ERROR!!!---SPIFFS File System init failed.\n");
#endif
        return false;
    }

//----------------初始化全局设备数据----------------
    if(this->initGlobalDeviceData())
    {
#ifndef TARGET_OS_MAC
        Serial.printf("------Global Device Data init succeed.\n");
#endif
    }else
    {
#ifndef TARGET_OS_MAC
        Serial.printf("---ERROR!!!---Global Device Data init failed.\n");
#endif
        return false;
    }

    //----------------初始化全局配置数据----------------
    if(this->initGlobalConfigData())
    {
#ifndef TARGET_OS_MAC
        Serial.printf("------Global Config Data init succeed.\n");
#endif
    }else
    {
#ifndef TARGET_OS_MAC
        Serial.printf("---ERROR!!!---Global Config Data init failed.\n");
#endif
        return false;
    }

    //----------------初始化全局用户数据----------------
    if(this->initGlobalUserData())
    {
#ifndef TARGET_OS_MAC
        Serial.printf("------Global User Data init succeed.\n");
#endif
    }else
    {
#ifndef TARGET_OS_MAC
        Serial.printf("---ERROR!!!---Global User Data init failed.\n");
#endif
        return false;
    }
    return true;
}

bool SDTSystem::initFileSystem()
{
    dot2d::FileUtils::getInstance()->setup(dot2d::FSS_SPIFFS);
    if(dot2d::FileUtils::getInstance()->getSpiffsState() == dot2d::FileState::FS_READY)
    {
        return true;
    }
    return false;
}

bool SDTSystem::initGlobalDeviceData()
{
#ifndef TARGET_OS_MAC
    _deviceData = GlobalDeviceData::createWithKey(PREFERENCES_DEVICE_DATA);
    if(_deviceData)
    {
        _deviceData->retain();
        return true;
    }
#else
    _deviceData = GlobalDeviceData::createWithPath(PREFERENCES_DEVICE_DATA);
    if(_deviceData)
    {
        _deviceData->retain();
        return true;
    }
#endif
    return false;
}

bool SDTSystem::initGlobalConfigData()
{
#ifndef TARGET_OS_MAC
    _configData = GlobalConfigData::createWithKey(PREFERENCES_CONFIG_DATA);
    if(_configData)
    {
        _configData->retain();
        return true;
    }
#else
    _configData = GlobalConfigData::createWithPath(PREFERENCES_CONFIG_DATA);
    if(_configData)
    {
        _configData->retain();
        return true;
    }
#endif
    return false;
}

bool SDTSystem::initGlobalUserData()
{
#ifndef TARGET_OS_MAC
    _userData = GlobalUserData::createWithKey(PREFERENCES_USER_DATA);
    if(_userData)
    {
        _userData->retain();
        return true;
    }
#else
//    _userData = GlobalUserData::createWithPath(PREFERENCES_USER_DATA);
//    if(_userData)
//    {
//        _userData->retain();
//        return true;
//    }
#endif
    return false;
}

 DateTime SDTSystem::now()
 {
    DateTime now = DEVICE.rtc.now();
    if (_configData->isAutoZone) {
        now = now + _configData->autoZone * 60 * 60 ;
    }
    else
    {
        now = now + _configData->manualZone * 60 * 60 ;
    }
    if (_configData->minuteOffset != 0) {
        now = now + _configData->minuteOffset * 60 ;
    }
    return now;
 }

bool SDTSystem::initAppManager()
{
    _appManager = AppManager::getInstance();
    _appManager->setGlobalTheme((AppManager::GlobalAppThemeType)_configData->theme);
    _appManager->initAppData();
    return _appManager->systemShow();
}

bool SDTSystem::initDevice()
{
    if(DEVICE.begin())
    {
        return true;
    }
    return false;
}

void delayReset_C()
{
    SDTSystem::getInstance()->ESPReset();
}

void SDTSystem::resetSystemToFactoryState()
{
#ifndef TARGET_OS_MAC
    _netManager->shutdownNetwork(true);
#endif
    _appManager->showReset();

#ifndef TARGET_OS_MAC
    SDTData::remvoeWithKey(PREFERENCES_DEVICE_DATA);
    SDTData::remvoeWithKey(PREFERENCES_USER_DATA);
    SDTData::remvoeWithKey(PREFERENCES_CONFIG_DATA);
    AppManager::getInstance()->removeAppData();

    Ticker *ticker = new Ticker();
    ticker->once(1,delayReset_C);
#endif
}

void SDTSystem::resetSystem()
{
#ifndef TARGET_OS_MAC
    _netManager->shutdownNetwork(true);
#endif
    _appManager->showReset();

    _deviceData->deviceName = _deviceData->getDefaultName();
    this->saveDeviceData();

#ifndef TARGET_OS_MAC
    DEVICE.resetBtChipName();
#endif
#ifndef TARGET_OS_MAC
    SDTData::remvoeWithKey(PREFERENCES_USER_DATA);
    SDTData::remvoeWithKey(PREFERENCES_CONFIG_DATA);
    AppManager::getInstance()->removeAppData();

    Ticker *ticker = new Ticker();
    ticker->once(1,delayReset_C);
#endif
}

void SDTSystem::initNetwork()
{
#ifndef TARGET_OS_MAC
    _netManager = NetManager::getInstance();
    _netManager->initMutexAndQueue();
#endif
}

SDTSystem::SystemState SDTSystem::resetState()
{
    // LOG_STR(_deviceData->UDID);
    // LOG_STR(_userData->wifiSsid);
    // LOG_STR(_userData->wifiPwd);

    if (_userData->wifiSsid.length()<=0 || _userData->wifiPwd.length()<=0)
    {
        //未读取到wifi连接信息
        _systemState = SystemState::INIT;
        return _systemState;
    }
    
    _appManager->showWifiIcon();

#ifndef TARGET_OS_MAC
    if(!NetManager::getInstance()->bootWifi(_userData->wifiSsid,_userData->wifiPwd))
    {
        //wifi连接失败
        _systemState = SystemState::INIT;
        return _systemState;
    }
#endif
    _systemState = SystemState::NORMAL;

    return _systemState;
}

bool SDTSystem::initWifiSystem()
{
#ifndef TARGET_OS_MAC
    return _netManager->initWifiSystem();
#else
    return true;
#endif
    
}

bool SDTSystem::initBleSystem()
{
#ifndef TARGET_OS_MAC
    return _netManager->initBleSystem();
#endif
    return false;
}

bool SDTSystem::initMqttSystem()
{
#ifndef TARGET_OS_MAC

#if MQTT_DEBUG_DATA
    this->setMqttServerIp("10.10.10.12");
    this->setMqttServerPort(1883);
#endif

    return _netManager->initMqttSystem();
#else
    return false;
#endif
}

bool SDTSystem::initWebSocket()
{
#ifndef TARGET_OS_MAC
    return _netManager->bootWebSocket();
#else
    return true;
#endif
}

void SDTSystem::startHeartbeat()
{
#ifndef TARGET_OS_MAC
    _netManager->startHeartBeat();
#endif
}

void SDTSystem::loop()
{
    // this->updateBlock();
    while (_isUpdating)
    {
#ifndef TARGET_OS_MAC
        HttpsOTAStatus_t otastatus = HttpsOTA.status();
        if(otastatus == HTTPS_OTA_SUCCESS) { 
            // Serial.println("Firmware written successfully. To reboot device, call API ESP.restart() or PUSH restart button on device");
            this->ESPReset();
        } else if(otastatus == HTTPS_OTA_FAIL) { 
            // Serial.println("Firmware Upgrade Fail");
            if (!this->updateBegin())
            {
                //提示更新失败
                this->ESPReset();
            }
        }else if(otastatus == HTTPS_OTA_ERR)
        {
            // Serial.println("Firmware Upgrade err");
        }
        // LOG_INT(otastatus);
        vTaskDelay(1000);
#endif
    }
    xMsgLoop();
    DEVICE.loop();
}
#ifndef TARGET_OS_MAC
void HttpEvent(HttpEvent_t *event)
{
    switch(event->event_id) {
        case HTTP_EVENT_ERROR:
            // Serial.println("Http Event Error");
            break;
        case HTTP_EVENT_ON_CONNECTED:
            // Serial.println("Http Event On Connected");
            break;
        case HTTP_EVENT_HEADER_SENT:
            // Serial.println("Http Event Header Sent");
            break;
        case HTTP_EVENT_ON_HEADER:
        {
            // Serial.printf("Http Event On Header, key=%s, value=%s\n", event->header_key, event->header_value);
            String ContentLength("Content-Length");
            if (ContentLength.equals(event->header_key))
            {
              String value(event->header_value);
              SDTSystem::getInstance()->setFirmwareTotoalSize(value.toInt());
            }
        }
            break;
        case HTTP_EVENT_ON_DATA:
        {
            SDTSystem::getInstance()->setFirmwareDownloadSize(event->data_len);
        }
            break;
        case HTTP_EVENT_ON_FINISH:
            // Serial.println("Http Event On Finish");
            break;
        case HTTP_EVENT_DISCONNECTED:
        {
            // Serial.println("Http Event Disconnected");
        }
            break;
    }
    vTaskDelay(5);
    dot2d::Director::getInstance()->mainLoop();
}
#endif

void SDTSystem::networkLoop()
{
#ifndef TARGET_OS_MAC
    this->updateBlock();
    _netManager->loop();
#endif
}

void SDTSystem::ESPReset()
{
#ifndef TARGET_OS_MAC
    DEVICE.bt201.end();
    LedScreenManager::getInstance()->closeScreen();
    delay(800);
    ESP.restart();
#endif
}

void SDTSystem::setFirmwareTotoalSize(uint32_t s)
{
    _firmware_totoal_size = s;
}

void SDTSystem::setFirmwareDownloadSize(uint32_t s)
{
    _firmware_download_size += s;
}

float SDTSystem::getUpdatePercent()
{
    if (_firmware_totoal_size == 0)
    {
        return 0;
    }
    return ((float)_firmware_download_size) / _firmware_totoal_size;
}

void SDTSystem::updateFirmware(const char* url,const char* certificate)
{
    _isUpdating = true;
    _updateUrl = url;
    _updateCertificate = certificate;
    _updateBeginCount = 0;
#ifndef TARGET_OS_MAC
    NetManager::getInstance()->shutdownNetwork(false);
    HttpsOTA.onHttpEvent(HttpEvent);
    // Serial.printf("Starting OTA , url:%s\n",_updateUrl.c_str());
    // Serial.printf("Starting OTA , certificate:%s\n",_updateCertificate.c_str());
    this->updateBegin();
#endif
    _appManager->showUpdate();
    dot2d::Director::getInstance()->mainLoop();
}

bool SDTSystem::updateBegin()
{
    if (_updateBeginCount < 5)
    {
#ifndef TARGET_OS_MAC
        HttpsOTA.begin(_updateUrl.c_str(), _updateCertificate.c_str());
#endif
        _updateBeginCount++;
        return true;
    }else
    {
        return false;
    }
}
