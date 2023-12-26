#include "CoreAppSystemState.h"
#include "CoreAppSystemState808.h"
#ifndef TARGET_OS_MAC
#include "HOUZZkitTester/device/SDTDevice.h"
#endif
#include "HOUZZkitTester/SDTSystem.h"
#include "HOUZZkitTester/app/SDTAppManager.h"
#include "HOUZZkitTester/device/SDTScreen.h"
NS_DT_BEGIN



bool CoreAppDataSystemState::subEncode(SDTData::DataSourceType type)
{
    return true;
}

bool CoreAppDataSystemState::subDecode(SDTData::DataSourceType type)
{
    return true;
}

CoreAppSystemState::~CoreAppSystemState()
{
    DT_SAFE_RELEASE(_statusSprite);
}

void CoreAppSystemState::tempStatusTimer()
{
    _showTempStatus = true;
    this->scheduleOnce(DT_SCHEDULE_SELECTOR(CoreAppSystemState::tempShowStatusEnd),2.0);
}

void CoreAppSystemState::tempShowStatusEnd(float dt)
{
    _showTempStatus = false;
}

void CoreAppSystemState::timeProofreadAction(float dt)
{
    String url = "http://worldtimeapi.org/api/ip";
    this->pushRequestTask(url, false, this, AppBase::HandleRequestCallback(&CoreAppSystemState::timeProofreadParser), 5);
}

bool CoreAppSystemState::timeProofreadParser(const String& res)
{
    uint32_t size = res.length();
    DynamicJsonDocument json(size+256);
    DeserializationError error = deserializeJson(json, res);
    if (error) {
        LOG("ERROR!!! Time Json Parse Failed! ");
        return false; 
    }
    uint32_t secondStamp = (uint32_t)json["unixtime"].as<String>().substring(0, 10).toInt();
    if (secondStamp == 0)
    {
        return false;
    }
#ifndef TARGET_OS_MAC
    int timeZone = (int)json["utc_offset"].as<String>().substring(0, 3).toInt();
    SDTSystem::getInstance()->setAutoZone(timeZone);
    SDTSystem::getInstance()->saveConfigData();
    DEVICE.rtc.adjust(DateTime(secondStamp));
#endif
    return true;
}

void CoreAppSystemState::update(float dt)
{
    SDTSystem* _sdtSystem = SDTSystem::getInstance();
    if (_sdtSystem->autoBrighten())
    {
        uint16_t lux = DEVICE.ldr.getCurrentLux();
        if (_offScreen)
        {
            if (lux>25)
            {
                LedScreenManager::getInstance()->setBrightness(_sdtSystem->brightness());
                _offScreen = false;
            }
        }else
        {
            if (lux<=10)
            {
                LedScreenManager::getInstance()->setBrightness(0);
                _offScreen = true;
            }
        }
    }else
    {
        if (_offScreen)
        {
            LedScreenManager::getInstance()->setBrightness(_sdtSystem->brightness());
            _offScreen = false;
        }
    }
}

bool CoreAppSystemState::enterAppControll()
{
    _underAppControll = true;
    AppManager::getInstance()->resumeCurrentAppEventDispatcher();
    return true;
}

bool CoreAppSystemState::exitAppControll()
{
    _underAppControll = false;
    AppManager::getInstance()->pauseCurrentAppEventDispatcher();
    return true;
}



bool CoreAppSystemState::init()
{
    rootLayer = CoreAppSystemStateLayer808::createWithApp(this,Size(8,8));
    this->addChild(rootLayer);



    _statusSprite = FrameSprite::create();
    _statusSprite->onEnter();
    _statusSprite->retain();

    auto listener = EventListenerButton::create();
    listener ->onBtnClick = [&](int8_t keyCode , Event * event ){
        
        if (_underAppControll)
        {
            if (keyCode == 5)
            {
                this->exitAppControll();
            }
        }else
        {
            // LOG("CoreAppSystemState onBtnClick keyCode:%d",keyCode);
            switch (keyCode)
            {
            case 0:
            {
                // DEVICE.tfCardPlay("/ALARM/daycd.mp3");
                // LOG("0006.mp3");
    //            this->showBluetoothIcon(true);
            }
                break;
            case 1:
            {
                AppManager::getInstance()->playNextPlayableApp();
            }
                break;
            case 2:
            {
                // DEVICE.tfCardPlay("/01/0007*.mp3");
                // LOG("0007.mp3");
                
            }
                break;
            case 3:
            {
                AppManager::getInstance()->playPreviousPlayableApp();
                // DEVICE.tfCardPlay("/01/0009*.wav");
                // LOG("0009.mp3");
            }
                break;
            case 4:
            {
                this->enterAppControll();
                // LOG_INT(DEVICE.bt201.readVolume());
                // DEVICE.bt201.play_pause();
            }
                break;
            case 5:
            {
                
    // #ifndef TARGET_OS_MAC
    //             DEVICE.tfCardPlay("/MP3/0003*.mp3");
    // #endif
            }
                break;
            default:
                break;
            }
        }
        // LOG_BOOL(_underAppControll);
        // LOG_INT(keyCode);
        
    };
    listener -> onBtnDoubleClick = [&](int8_t keyCode , Event * event ){
        switch (keyCode)
        {
        case 5:
        {
// #ifndef TARGET_OS_MAC
//             DEVICE.tfCardPlay("/MP3/0001*.mp3");
// #endif
        }
            break;
        default:
            break;
        }
    };
    listener -> onBtnLongPressStart = [&](int8_t keyCode , Event * event ){
        switch (keyCode)
        {
        case 5:
        {
#ifndef TARGET_OS_MAC
            btSwitchPressDuration = millis();
#endif
        }
            break;
        default:
            break;
        }
    };
    listener -> onBtnLongPressStop = [&](int8_t keyCode , Event * event ){
        switch (keyCode)
        {
        case 5:
        {
            btSwitchEnable = true;
        }
            break;
        default:
            break;
        }
    };
    listener -> onBtnDuringLongPress = [&](int8_t keyCode , Event * event ){
        
        switch (keyCode)
        {
        case 5:
        {
#ifndef TARGET_OS_MAC
            if (btSwitchEnable && millis() - btSwitchPressDuration > LONG_PRESSED_BUTTON_CHECK_DURATION)
            {
                switchBtSpeaker();
                btSwitchEnable = false;
            }
#endif
        }
            break;
        default:
            break;
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority ( listener, this );

    return true;
}

void CoreAppSystemState::onExit()
{
    if (_running)
    {
        --__attachedNodeCount;
    }
    
    this->pause();
    
    _running = true;
    
    for( const auto &child: _children)
        child->onExit();
    
}

void CoreAppSystemState::onEnter()
{
    if (!_running)
    {
        ++__attachedNodeCount;
    }

    for( const auto &child: _children)
        child->onEnter();
    
    this->resume();
    
    _running = true;
}

void CoreAppSystemState::cleanup()
{
    this->stopAllActions();
    // this->unscheduleAllCallbacks();
    // for( const auto &child: _children)
    //     child->cleanup();
}

void CoreAppSystemState::pause(void)
{
    // _scheduler->pauseTarget(this);
    _actionManager->pauseTarget(this);
    // _eventDispatcher->pauseEventListenersForTarget(this);
}

void CoreAppSystemState::showBluetoothIcon(bool asTempStatus)
{
    ((CoreAppSystemStateLayer808 *)rootLayer)->showBluetoothIcon();
    if (asTempStatus){tempStatusTimer();}
}

void CoreAppSystemState::showBluetoothCloseIcon(bool asTempStatus)
{
    ((CoreAppSystemStateLayer808 *)rootLayer)->showBluetoothCloseIcon();
    if (asTempStatus){tempStatusTimer();}
}

void CoreAppSystemState::showWifiIcon(bool asTempStatus)
{   
    ((CoreAppSystemStateLayer808 *)rootLayer)->showWifiIcon();
    if (asTempStatus){tempStatusTimer();}
}

void CoreAppSystemState::showReset(bool asTempStatus)
{   
    ((CoreAppSystemStateLayer808 *)rootLayer)->showReset();
    if (asTempStatus){tempStatusTimer();}
}

void CoreAppSystemState::showSetting(bool asTempStatus)
{
    ((CoreAppSystemStateLayer808 *)rootLayer)->showSetting();
    if (asTempStatus){tempStatusTimer();}
}

void CoreAppSystemState::showUpdate()
{
    ((CoreAppSystemStateLayer808 *)rootLayer)->showUpdate();
}

void speakerEnable(void *pvParameters)
{
#ifndef TARGET_OS_MAC
    DEVICE.tfCardPlay("/SYSTEM/001*.wav");
    delay(1000);
    DEVICE.btSpeakerEnable(true);
    vTaskDelete(NULL);
#endif
}

void speakerDisable(void *pvParameters)
{
#ifndef TARGET_OS_MAC
    DEVICE.tfCardPlay("/SYSTEM/002*.wav");
    delay(1000);
    DEVICE.btSpeakerEnable(false);
    vTaskDelete(NULL);
#endif
}

void CoreAppSystemState::switchBtSpeaker()
{
#ifndef TARGET_OS_MAC
    if (SDTSystem::getInstance()->btSpeakerEnable())
    {
        if (xTaskCreatePinnedToCore(speakerDisable, "speakerDisable", 8192, NULL, 0, NULL , 0) != pdPASS) {
            log_e("Couldn't create speakerDisable task\n"); 
        }
        delay(400);
        SDTSystem::getInstance()->setBtSpeakerEnable(false);
        SDTSystem::getInstance()->saveConfigData();
        this->showBluetoothCloseIcon(true);
    }
    else
    {
        if (xTaskCreatePinnedToCore(speakerEnable, "speakerEnable", 8192, NULL, 0, NULL , 0) != pdPASS) {
            log_e("Couldn't create speakerEnable task\n"); 
        }
        delay(400);
        SDTSystem::getInstance()->setBtSpeakerEnable(true);
        SDTSystem::getInstance()->saveConfigData();
        this->showBluetoothIcon(true);
    }
#endif
}

void CoreAppSystemState::drawStatus()
{
    if (!_showTempStatus)
    {
        return;
    }
    rootLayer->visit(Director::getInstance()->getRenderer(),Transform::IDENTITY);
}

void CoreAppSystemState::startSchedule()
{
    this->timeProofreadAction(0);
    this->schedule(DT_SCHEDULE_SELECTOR(CoreAppSystemState::timeProofreadAction),1*60*60);
    this->scheduleUpdate();
}

bool CoreAppSystemStateLayer::initLayer()
{
    
    return true;
}

NS_DT_END
