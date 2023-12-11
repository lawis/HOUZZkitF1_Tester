#include "SDTAppManager.h"
#include "HOUZZkitTester/SDTConfig.h"
#include "HOUZZkitTester/SDTMaroc.h"
#include "HOUZZkitTester/app/COREAPP_SystemState/CoreAppSystemState.h"
#include "HOUZZkitTester/app/AppDataList.h"
#include "HOUZZkitTester/network/SDTNetManager.h"
#include "HOUZZkitTester/SDTSystem.h"
#include "SDTApp.h"
#if APP_DEBUG_DATA
#include "HOUZZkitTester/app/APP00001_Clock/AppClock.h"
#include "HOUZZkitTester/app/APP00002_BoardCheck/AppBoardCheck.h"
#include "HOUZZkitTester/app/APP00003_Animation/AppAnimation.h"
#include "HOUZZkitTester/app/APP00004_Btc/AppBtc.h"
#include "HOUZZkitTester/app/APP00005_Matrix/AppMatrix.h"
#include "HOUZZkitTester/app/APP00007_AwtrixClient/AppAwtrixClient.h"
#include "HOUZZkitTester/app/APP00008_Weather/AppWeather.h"
#include "HOUZZkitTester/app/APP00013_Bilibili/AppBilibili.h"
#include "HOUZZkitTester/app/APP00014_Youtube/AppYoutube.h"
#include "HOUZZkitTester/app/APP00015_Weibo/AppWeibo.h"
#include "HOUZZkitTester/app/APP00016_Douyin/AppDouyin.h"
#include "HOUZZkitTester/app/APP00009_AmbientLight/AppAmbientLight.h"
#include "HOUZZkitTester/app/APP00010_Spectrum/AppSpectrum.h"
#include "HOUZZkitTester/app/APP00012_Alarm/AppAlarm.h"
#include "HOUZZkitTester/app/APP00017_Stock/AppStock.h"
#include "HOUZZkitTester/app/APP00018_Thermometer/AppThermometer.h"
#include "HOUZZkitTester/app/APP00019_DayCountdown/AppDayCountdown.h"
#include "HOUZZkitTester/app/APP00020_LightIndex/AppLightIndex.h"
#include "HOUZZkitTester/app/APP00021_DayNumOfYear/AppDayNumOfYear.h"
#include "HOUZZkitTester/app/APP00023_ScoreBoard/AppScoreBoard.h"
#include "HOUZZkitTester/app/APP00027_TimeOfLife/AppTimeOfLife.h"
#include "HOUZZkitTester/app/APP00028_Chronograph/AppChronograph.h"
#include "HOUZZkitTester/app/APP00029_Note/AppNote.h"
#include "HOUZZkitTester/SDTIconLib.h"

#endif


static AppManager* s_AppManager = nullptr;

bool AppManager::playAppByData(AppDataBase* appData,bool autoSwith)
{
    if (appData)
    {
        AppBase* currentApp = (AppBase*)dot2d::Director::getInstance()->getRunningScene();
        if (currentApp && currentApp->getData()->id == appData->id) {
            if (autoSwith && appData->showDuration>0)
            {
                currentApp->setAutoSwitch();
            }else
            {
                currentApp->removeAutoSwitch();
            }
            return true;
        }
        AppBase *app = AppFactory::getInstance().createAppWithData(appData);
        if (app)
        {
            dot2d::Director::getInstance()->replaceScene(app);
            if (autoSwith && appData->showDuration>0)
            {
                app->setAutoSwitch();
            }
            return true;
        }
    }else{
        this->showSetting();
    }
    return false;
}

void AppManager::playAppByIndex(uint8_t index)
{
    AppDataBase* appData = _appDataList->getAppDataWithIndex(index);
    this->playAppByData(appData,true);
    _appPlayType = AppPlayType::LIST;
}

void AppManager::playAppById(uint8_t id)
{
    AppDataBase* appData = _appDataList->getAppDataWithId(id);
    this->playAppByData(appData,false);
    _appPlayType = AppPlayType::SINGLE;
    _appPlayId = id;
}

bool AppManager::findNextPlayableApp()
{
    uint8_t findtimes = 0;
    uint8_t appDataListSize = _appDataList->getAppDataListSize();
    while (findtimes<appDataListSize) {
        findtimes++;
        _appListPlayPointer++;
        if (_appListPlayPointer >= _appDataList->getAppDataListSize())
        {
            _appListPlayPointer = 0;
        }
        AppDataBase* appData = _appDataList->getAppDataWithIndex(_appListPlayPointer);
        if (appData->canPlay && appData->openPlay && appData->isActive) {
            return true;
        }
    }
    return false;
}

bool AppManager::findPreviousPlayableApp()
{
    uint8_t findtimes = 0;
    uint8_t appDataListSize = _appDataList->getAppDataListSize();
    while (findtimes<appDataListSize) {
        findtimes++;
        _appListPlayPointer--;
        if (_appListPlayPointer == UINT8_MAX)
        {
            _appListPlayPointer = _appDataList->getAppDataListSize() - 1;
        }
        AppDataBase* appData = _appDataList->getAppDataWithIndex(_appListPlayPointer);
        if (appData->canPlay && appData->openPlay && appData->isActive) {
            return true;
        }
    }
    return false;
}


#if APP_DEBUG_DATA
void AppManager::createDebugAppDataList()
{
//    AppDataList::removeAppDataListFile();
    AppDataList* appDataList = AppDataList::createWithDefaultFile();
    appDataList->clear();
    
//    dot2d::AppDataClock* appDataClock = dot2d::AppDataClock::create();
//    appDataClock->appAlias = "Clock";
//    appDataClock->showDuration = 15;
//    appDataClock->isShowDate = false;
//    appDataClock->isShowSecond = true;
//  //    appDataClock->minuteOffset = 5;
//    appDataClock->isSundayFirstday = false;
//    appDataClock->isShowWeek = true;
//    appDataClock->is24HourView = true;
//    appDataClock->appSizeType = AppDataBase::AppSizeType::S3208;
//    appDataClock->appAlignType = AppDataBase::AppAlignType::CENTER;
//    appDataClock->setTheme(0);
//    appDataList->addNewAppData(appDataClock);
// //
//     dot2d::AppDataBoardCheck* appDataBoardCheck = dot2d::AppDataBoardCheck::create();
//     appDataBoardCheck->boardCheckType = dot2d::AppDataBoardCheck::BoardCheckType::DOT_SHOW;
//     appDataList->addNewAppData(appDataBoardCheck);
//
//    dot2d::AppDataAnimation * appDataAnimation = dot2d::AppDataAnimation::create();
//    appDataList->addNewAppData(appDataAnimation);
//   appDataAnimation->saveImageBuffer(img_test+2, GetUint16(img_test));
//
//    dot2d::AppDataBtc* appDataBtc = dot2d::AppDataBtc::create();
//    appDataBtc->appSizeType = AppDataBase::AppSizeType::S3208;
//    appDataBtc->coinName = String("bitcoin");
//    appDataList->addNewAppData(appDataBtc);
//
      dot2d::AppDataMatrix * appDataMatrix = dot2d::AppDataMatrix::create();
      appDataList->addNewAppData(appDataMatrix);
//

//
//    // //
//    dot2d::AppDataBilibili * appDataBilibili = dot2d::AppDataBilibili::create();
//    appDataBilibili->appSizeType = AppDataBase::AppSizeType::S3208;
//    appDataBilibili->uid = "298146460";
//    appDataList->addNewAppData(appDataBilibili);
//
//    dot2d::AppDataYoutube * appDataYoutube = dot2d::AppDataYoutube::create();
//    appDataYoutube->appSizeType = AppDataBase::AppSizeType::S3208;
//    appDataYoutube->channelID = "UCVP3cwbysoohuvQbSWN8RgA";
//    appDataYoutube->apiKey = "AIzaSyCJVGd3mN2h2o4Koj8dOtGkCrQervigUpk";
//    appDataList->addNewAppData(appDataYoutube);
//
//    dot2d::AppDataWeibo * appDataWeibo = dot2d::AppDataWeibo::create();
//    appDataWeibo->appSizeType = AppDataBase::AppSizeType::S3208;
//    appDataWeibo->uid = "1841479001";
//    appDataWeibo->isCountFormat = true;
//    appDataList->addNewAppData(appDataWeibo);
//
//    dot2d::AppDataDouyin * appDataDouyin = dot2d::AppDataDouyin::create();
//    appDataDouyin->appSizeType = AppDataBase::AppSizeType::S3208;
//    appDataDouyin->uid = "MS4wLjABAAAA4N4OrZzTSmCPp8vVAqCeyU215Kav2JgFv2Lfy4DNWRs";
//    appDataDouyin->isCountFormat = true;
//    appDataList->addNewAppData(appDataDouyin);
//

//      dot2d::AppDataAmbientLight* appDataAmbientLight = dot2d::AppDataAmbientLight::create();
//      appDataAmbientLight->ambientEffectType = dot2d::AppDataAmbientLight::AmbientEffectType::RANDOM;
//      appDataList->addNewAppData(appDataAmbientLight);


//      dot2d::AppDataSpectrum* appDataSpectrum = dot2d::AppDataSpectrum::create();
//      appDataSpectrum->spectrumEffectType = dot2d::AppDataSpectrum::SpectrumEffectType::LINE_CHROMATOGRAM;
//      appDataList->addNewAppData(appDataSpectrum);
    
//     dot2d::AppDataWeather * appDataWeather = dot2d::AppDataWeather::create();
// //    appDataWeather->cityCode = "101010300";
//     appDataList->addNewAppData(appDataWeather);
    
//    dot2d::AppDataAlarm* appDataAlarm = dot2d::AppDataAlarm::create();
//    appDataAlarm->enable = true;
//    appDataAlarm->hour = 17;
//    appDataAlarm->minute = 4;
//    appDataList->addNewAppData(appDataAlarm);

//     dot2d::AppDataStock* appDataStock = dot2d::AppDataStock::create();
// //    appDataStock->stockCode = String("sh000001");
//     appDataList->addNewAppData(appDataStock);
    
    // dot2d::AppDataThermometer* appDataThermometer = dot2d::AppDataThermometer::create();
    // appDataList->addNewAppData(appDataThermometer);

    // dot2d::AppDataDayCountdown* appDataDayCountdown = dot2d::AppDataDayCountdown::create();
    // appDataDayCountdown->year = 2021;
    // appDataDayCountdown->month = 9;
    // appDataDayCountdown->day = 30;
    // appDataDayCountdown->enable = true;
    // appDataList->addNewAppData(appDataDayCountdown);

    // dot2d::AppDataLightIndex *appDataLightIndex = dot2d::AppDataLightIndex::create();
    // appDataList->addNewAppData(appDataLightIndex);

//    dot2d::AppDataDayNumOfYear *appDataDayNumOfYear = dot2d::AppDataDayNumOfYear::create();
//    appDataDayNumOfYear->showAsPercent = true;
//    appDataList->addNewAppData(appDataDayNumOfYear);
    
//    dot2d::AppDataScoreBoard *appDataScoreBoard = dot2d::AppDataScoreBoard::create();
//    appDataList->addNewAppData(appDataScoreBoard);

    // dot2d::AppDataTimeOfLife *appDataTimeOfLife = dot2d::AppDataTimeOfLife::create();
    // appDataTimeOfLife->year = 1988;
    // appDataTimeOfLife->month = 12;
    // appDataTimeOfLife->day = 11;
    // appDataList->addNewAppData(appDataTimeOfLife);

    // dot2d::AppDataChronograph *appDataChronograph = dot2d::AppDataChronograph::create();
    // appDataList->addNewAppData(appDataChronograph);

    dot2d::AppDataNote *appDataNote = dot2d::AppDataNote::create();
    appDataNote->message = String("AaBb123长文字");
    appDataNote->alignment = 2;
    appDataNote->scrollType = 0;
    appDataList->addNewAppData(appDataNote);

    // dot2d::AppDataAwtrixClient *appDataAwtrixClient = dot2d::AppDataAwtrixClient::create();
    // appDataAwtrixClient->serverIp = "10.10.10.100";
    // appDataAwtrixClient->serverPort = 7001;
    // appDataList->addNewAppData(appDataAwtrixClient);

    appDataList->saveData();
}
#endif

AppManager* AppManager::getInstance()
{
    if (s_AppManager == nullptr)
    {
        s_AppManager = new AppManager();
        s_AppManager->init();
    }
    return s_AppManager;
}

AppManager::AppManager()
{
    
}

AppManager::~AppManager()
{
    DT_SAFE_DELETE(globalAppTheme);
    DT_SAFE_RELEASE(_appDataList);
}

bool AppManager::init()
{
    globalAppTheme = new AppTheme();
    return true;
}

void AppManager::setGlobalTheme(GlobalAppThemeType type)
{
    this->setTheme(type,globalAppTheme);
}

void AppManager::setTheme(GlobalAppThemeType type,AppTheme* theme)
{
    switch (type)
    {
        case GlobalAppThemeType::CLASSICAL:
            theme->bgColor = dot2d::DTRGB(30,30,30);
            theme->fgColor = dot2d::DTRGB(255, 255, 255);
            theme->textColor = dot2d::DTRGB(255, 255, 255);
            break;
        case GlobalAppThemeType::FIRE:
            theme->bgColor = dot2d::DTHSV( 0, 255, 100);
            theme->fgColor = dot2d::DTHSV( 0, 255, 255);
            theme->textColor = dot2d::DTHSV( 32, 255, 255);
            break;
        case GlobalAppThemeType::PURPLEDREAM:
            theme->bgColor = dot2d::DTHSV( 96, 255, 100);
            theme->fgColor = dot2d::DTHSV( 96, 255, 255);
            theme->textColor = dot2d::DTHSV( 192, 255, 255);
            break;
        case GlobalAppThemeType::FOREST:
            theme->bgColor = dot2d::DTHSV( 110, 255, 100);
            theme->fgColor = dot2d::DTHSV( 110, 255, 255);
            theme->textColor = dot2d::DTHSV( 133, 255, 255);
            break;
        case GlobalAppThemeType::CLOUD:
            theme->bgColor = dot2d::DTHSV( 180, 255, 100);
            theme->fgColor = dot2d::DTHSV( 180, 255, 255);
            theme->textColor = dot2d::DTHSV( 255, 255, 255);
            break;
        case GlobalAppThemeType::LAVA:
            theme->bgColor = dot2d::DTHSV( 0, 255, 100);
            theme->fgColor = dot2d::DTHSV( 0, 255, 255);
            theme->textColor = dot2d::DTHSV( 11, 255, 255);
            break;
        case GlobalAppThemeType::OCEAN:
            theme->bgColor = dot2d::DTHSV( 170, 255, 100);
            theme->fgColor = dot2d::DTHSV( 170, 255, 255);
            theme->textColor = dot2d::DTHSV( 155, 255, 255);
            break;
        default:
            theme->bgColor = dot2d::DTRGB(30,30,30);
            theme->fgColor = dot2d::DTRGB(255, 255, 255);
            theme->textColor = dot2d::DTRGB(255, 255, 255);
            break;
    }
}

bool AppManager::initAppData()
{
#if APP_DEBUG_DATA
    createDebugAppDataList();
#endif    
    _appDataList = AppDataList::createWithDefaultFile();
    _appDataList->retain();
    return true;
}

bool AppManager::removeAppData()
{
    _appDataList->removeAppDataListFile();
    return true;
}

bool AppManager::mtfSystemNormalAction(const uint8_t* buffer,const uint32_t size)
{
    this->systemAppStartSchedule();
    this->initRequestList();
    this->playNextPlayableApp();
    return true;
}

bool AppManager::mtfSystemInitAction(const uint8_t* buffer,const uint32_t size)
{
    this->showBluetoothIcon();
    return true;
}

bool AppManager::mtfShowPreviewApp(const uint8_t* buffer,const uint32_t size)
{
    AppDataBase* appData = _appDataList->getAppDataWithId(buffer[0]);
    if (!appData)
    {
        return false;
    }
    appData->deBegin(buffer,size);
    if(!appData->decode(SDTData::DataSourceType::Net))
    {
        return false;
    }
    if(_previewApp && _previewApp->getData() == appData)
    {
        //do nothing
    }else
    {
        AppBase* app = AppFactory::getInstance().createAppWithData(appData);
        if (app)
        {
            dot2d::Director::getInstance()->replaceScene(app);
            _previewApp = app;
        }else
        {
            return false;
        }
    }
    _previewApp->resetLayer();
    return true;
}

bool AppManager::mtfExitPreviewApp(const uint8_t* buffer,const uint32_t size)
{
    _previewApp = nullptr;
    this->recoverPlayApp();
    return true;
}

bool AppManager::mtfPlayAppById(const uint8_t* buffer,const uint32_t size)
{
    uint8_t id = buffer[0];
    if (id == 0)
    {
        this->playNextPlayableApp(true);
        ((dot2d::CoreAppSystemState*)_systemApp)->exitAppControll();
    }else
    {
        this->playAppById(id);
        ((dot2d::CoreAppSystemState*)_systemApp)->enterAppControll();
    }
    return true;
}

bool AppManager::mtfAddNewApp(const uint8_t* buffer,const uint32_t size)
{
    uint8_t id = buffer[0];
    AppDataBase* appData = _appDataList->getAppDataWithId(id);
    appData->appAddInit();
    this->addAppSchduler(id);
    if (this->currentAppId() == 0)
    {
        this->playNextPlayableApp();
    }
    return true;
}

bool AppManager::mtfDeleteApp(const uint8_t* buffer,const uint32_t size)
{
    uint8_t id = buffer[0];
    this->deleteAppSchduler(id);
    if (this->currentAppId() == id)
    {
        this->playNextPlayableApp(true);
    }
    return true;
}

bool AppManager::mtfSaveConfigData(const uint8_t* buffer,const uint32_t size)
{
    SDTSystem::getInstance()->saveConfigData();
    return true;
}

bool AppManager::mtfPlayNextPlayableApp(const uint8_t* buffer,const uint32_t size)
{
    this->playNextPlayableApp();
    ((dot2d::CoreAppSystemState*)_systemApp)->exitAppControll();
    return true;
}

bool AppManager::mtfPlayPreviousPlayableApp(const uint8_t* buffer,const uint32_t size)
{
    this->playPreviousPlayableApp();
    ((dot2d::CoreAppSystemState*)_systemApp)->exitAppControll();
    return true;
}

bool AppManager::mtfUpdateFirmware(const uint8_t* buffer,const uint32_t size)
{
    uint16_t offset = 0;

    uint16_t length = GetUint8(buffer);
    offset++;
    std::string url = "";
    url.append((char *)(buffer+offset), length);
    offset+=length;

    std::string certificate = "";
    length = GetUint16(buffer+offset);
    offset+=2;
    certificate.append((char *)(buffer+offset), length);

#ifndef TARGET_OS_MAC
    delay(100);//等待mtfUpdateFirmware返回网络数据
#endif
    SDTSystem::getInstance()->updateFirmware(url.c_str(),certificate.c_str());
    return true;
}

bool AppManager::mtfReorderAppList(const uint8_t* buffer,const uint32_t size)
{
    // uint8_t id = buffer[1];
    uint8_t fromIndex = buffer[0];
    uint8_t toIndex = buffer[2];

    bool res = _appDataList->reorderAppData(fromIndex,toIndex);
    if (res)
    {
        _appDataList->saveData();
    }
    return res;
}


bool AppManager::wtfShowPreviewApp(const uint8_t* buffer,const uint32_t size,Protocols& proto)
{
    AppDataBase* appData = _appDataList->getAppDataWithId(buffer[0]);
    if (!appData)
    {
        return false;
    }
#ifndef TARGET_OS_MAC
    xMESSAGE msg(&AppManager::mtfShowPreviewApp,buffer,size);
    NET_QUEUE_SEND(&msg);
#endif
    return true;
}

bool AppManager::wtfExitPreviewApp(const uint8_t* buffer,const uint32_t size,Protocols& proto)
{
#ifndef TARGET_OS_MAC
    xMESSAGE msg(&AppManager::mtfExitPreviewApp);
    NET_QUEUE_SEND(&msg);
#endif
    return true;
}

bool AppManager::wtfPlayAppById(const uint8_t* buffer,const uint32_t size,Protocols& proto)
{
#ifndef TARGET_OS_MAC
    xMESSAGE msg(&AppManager::mtfPlayAppById,buffer,size);
    NET_QUEUE_SEND(&msg);
#endif
    uint8_t id = buffer[0];
    if (id == 0)
    {
        proto.pushUint8(0);
        return true;
    }
    AppDataBase* appData = _appDataList->getAppDataWithId(id);
    if (!appData)
    {
        proto.pushUint8(1);
        return false;
    }
    proto.pushUint8(0);
    return true;
}

bool AppManager::wtfAddNewApp(const uint8_t* buffer,const uint32_t size,Protocols& proto)
{
    uint16_t appId = GetUint16(buffer);
    AppDataBase* appDataBase = AppFactory::getInstance().createInitAppData(appId);
    if (!appDataBase)
    {
        proto.pushUint8(1);
        return false;
    }
    bool res = _appDataList->addNewAppData(appDataBase);
    if (res)
    {
        proto.pushUint8(0);
        proto.pushUint8(_appDataList->getAppDataListSize()-1);
        appDataBase->enBegin();
        appDataBase->encodeSummary();
        proto.pushProtocol(*appDataBase);
#ifndef TARGET_OS_MAC
        uint8_t tempBuffer[] = {appDataBase->id};
        xMESSAGE msg(&AppManager::mtfAddNewApp,tempBuffer,1);
        NET_QUEUE_SEND(&msg);
#endif
    }else
    {
        proto.pushUint8(1);
    }
#ifndef TARGET_OS_MAC
    NET_MUTEX_LOCK();
#endif
    _appDataList->saveData();
#ifndef TARGET_OS_MAC
    NET_MUTEX_UNLOCK();
#endif
    return res;
}

bool AppManager::wtfDeleteApp(const uint8_t* buffer,const uint32_t size,Protocols& proto)
{
    uint8_t id = buffer[0];
    AppDataBase* appData = _appDataList->getAppDataWithId(id);
    if (!appData)
    {
        proto.pushUint8(1);
        return false;
    }

#ifndef TARGET_OS_MAC
    NET_MUTEX_LOCK();
#endif
    bool res = _appDataList->removeAppData(id);
    if (res)
    {
        proto.pushUint8(0);
    }else
    {
        proto.pushUint8(1);
    }
    _appDataList->saveData();

#ifndef TARGET_OS_MAC
    uint8_t tempBuffer[] = {appData->id};
    xMESSAGE msg(&AppManager::mtfDeleteApp,tempBuffer,1);
    NET_QUEUE_SEND(&msg);
#endif

#ifndef TARGET_OS_MAC
    NET_MUTEX_UNLOCK();
#endif
    
    return res;
}

bool AppManager::wtfPlayNextPlayableApp(const uint8_t* buffer,const uint32_t size,Protocols& proto)
{

#ifndef TARGET_OS_MAC
    xMESSAGE msg(&AppManager::mtfPlayNextPlayableApp);
    NET_QUEUE_SEND(&msg);
#endif
    proto.pushUint8(0);
    return true;
}

bool AppManager::wtfPlayPreviousPlayableApp(const uint8_t* buffer,const uint32_t size,Protocols& proto)
{
#ifndef TARGET_OS_MAC
    xMESSAGE msg(&AppManager::mtfPlayPreviousPlayableApp);
    NET_QUEUE_SEND(&msg);
#endif
    proto.pushUint8(0);
    return true;
}

bool AppManager::wtfUpdateFirmware(const uint8_t* buffer,const uint32_t size,Protocols& proto)
{
#ifndef TARGET_OS_MAC
    xMESSAGE msg(&AppManager::mtfUpdateFirmware,buffer,size);
    NET_QUEUE_SEND(&msg);
#endif
    proto.pushUint8(0);
    return true;
}

bool AppManager::wtfReorderAppList(const uint8_t* buffer,const uint32_t size,Protocols& proto)
{
    uint8_t id = buffer[1];
    uint8_t fromIndex = buffer[0];
    uint8_t toIndex = buffer[2];
    AppDataBase* appData = _appDataList->getAppDataWithIndex(fromIndex);
    if (!appData || appData->id != id)
    {
        proto.pushUint8(1);
        return false;
    }
    if (toIndex>_appDataList->getAppDataListSize())
    {
        proto.pushUint8(1);
        return false;
    }
#ifndef TARGET_OS_MAC
    xMESSAGE msg(&AppManager::mtfReorderAppList,buffer,size);
    NET_QUEUE_SEND(&msg);
#endif
    proto.pushUint8(0);
    return true;
}

bool AppManager::mqtfPlayAppByName(const String& name)
{
    AppDataBase* appData = _appDataList->getAppDataWithAlias(name);
    if (!appData)
    {
        return false;
    }
#ifndef TARGET_OS_MAC
    uint8_t tempBuffer[] = {appData->id};
    xMESSAGE msg(&AppManager::mtfPlayAppById,tempBuffer,1);
    NET_QUEUE_SEND(&msg);
#endif
    return true;
}


bool AppManager::mqtfPlayAppByList()
{
#ifndef TARGET_OS_MAC
    uint8_t tempBuffer[] = {0};
    xMESSAGE msg(&AppManager::mtfPlayAppById,tempBuffer,1);
    NET_QUEUE_SEND(&msg);
#endif
    return true;
}

AppBase* AppManager::getCurrentApp()
{
    return (AppBase*)dot2d::Director::getInstance()->getRunningScene();
}

void AppManager::pauseCurrentAppEventDispatcher()
{
    AppBase* app = this->getCurrentApp();
    if (app != _systemApp)
    {
        // app->scheduleOnce(DT_SCHEDULE_SELECTOR(AppBase::pauseEventDispatcherDt),0.01);
        app->pauseEventDispatcher();
        app->setAutoSwitch();
        app->buttonEventDisable();
        _appPlayType = AppPlayType::LIST;
    }
}

void AppManager::resumeCurrentAppEventDispatcher()
{
    AppBase* app = this->getCurrentApp();
    if (app != _systemApp)
    {
        //延迟调用，保证至少在下一帧才进入应用内操作状态
        app->scheduleOnce(DT_SCHEDULE_SELECTOR(AppBase::resumeEventDispatcherDt),0.01);
        app->removeAutoSwitch();
        app->buttonEventEnable();
        _appPlayType = AppPlayType::SINGLE;
        _appPlayId = app->getData()->id;
    }
}

bool AppManager::systemShow()
{
    //开始启动画面
    if(!_systemApp)
    {
        AppDataBase* appData = AppFactory::getInstance().createInitAppData(65535);
        _systemApp = (dot2d::CoreAppSystemState*)AppFactory::getInstance().createAppWithData(appData);
        if(_systemApp)
        {
            ((dot2d::CoreAppSystemState*)_systemApp)->retain();
        }else
        {
            return false;
        }
    }
    ((dot2d::CoreAppSystemState*)_systemApp)->onEnter();
    dot2d::Director::getInstance()->replaceScene(((dot2d::CoreAppSystemState*)_systemApp));
    return true;
}

bool AppManager::systemAppStartSchedule()
{
    if (_systemApp)
    {
        ((dot2d::CoreAppSystemState*)_systemApp)->startSchedule();
        return true;
    }
    return false;
    
}

void AppManager::drawTempStatus()
{
    if (_systemApp)
    {
        ((dot2d::CoreAppSystemState*)_systemApp)->drawStatus();
    }
}

void AppManager::playNextPlayableApp(bool findFromHead)
{
    if (findFromHead)
    {
        _appListPlayPointer = UINT8_MAX;
    }
    if (findNextPlayableApp()) {
        this->playAppByIndex(_appListPlayPointer);
    }else
    {
        this->playAppByData(nullptr);
    }

    // dot2d::SDTApp *app = dot2d::SDTApp::create();
    // dot2d::Director::getInstance()->replaceScene(app);

}

void AppManager::playPreviousPlayableApp()
{
    if (findPreviousPlayableApp()) {
        this->playAppByIndex(_appListPlayPointer);
    }else
    {
        this->playAppByData(nullptr);
    }
}

void AppManager::recoverPlayApp()
{
    switch (_appPlayType)
    {
    case AppPlayType::NONE:
    {
        this->playAppByData(nullptr);
    }
        break;
    case AppPlayType::LIST:
    {
        this->playAppByIndex(_appListPlayPointer);
    }
        break;
    case AppPlayType::SINGLE:
    {
        this->playAppById(_appPlayId);
    }
        break;
    default:
        break;
    }
}

uint8_t AppManager::currentAppId()
{
    AppBase* currentApp = (AppBase*)dot2d::Director::getInstance()->getRunningScene();
    if (currentApp) {
        return currentApp->getData()->id;
    }
    return 0;
}

bool AppManager::addSchedulerToList(AppScheduleBase *request)
{
    if(!_appScheduleList.contains(request))
    {
        request->onEnter();
        _appScheduleList.pushBack(request);
        request->startSchedule(true);
        return true;
    }
    return false;
}

bool AppManager::executeAppSchduleTask(uint8_t id)
{
    for (uint8_t i = 0; i < _appScheduleList.size(); i++)
    {
        AppScheduleBase* scheduler = _appScheduleList.at(i);
        AppDataBase* appData = scheduler->getData();
        if (appData->id == id)
        {
            scheduler->startScheduleAction(0);
            return true;
        }
    }
    return false;
}

bool AppManager::deleteAppSchduler(uint8_t id)
{
    for (uint8_t i = 0; i < _appScheduleList.size(); i++)
    {
        AppScheduleBase* appSchdule = _appScheduleList.at(i);
        AppDataBase* appData = appSchdule->getData();
        if (appData->id == id)
        {
            NetManager::getInstance()->dirtyRequestTask(appSchdule);
            _appScheduleList.erase(i);
            return true;
        }
    }
    return false;
}

bool AppManager::addAppSchduler(uint8_t id)
{
    AppDataBase* appData = _appDataList->getAppDataWithId(id);
    if (appData->scheduleType == AppDataBase::ScheduleType::SCHEDULE) 
    {
        AppScheduleBase* appSchdule = AppFactory::getInstance().createAppScheduleWithData(appData);
        if (appSchdule) {
            if (!this->addSchedulerToList(appSchdule)) {
                LOG("ERROR APP:%d AppScheduleClass has been Add",appData->appId());
                return false;
            }
        }else
        {
            LOG("ERROR APP:%d Has NO AppScheduleClass",appData->appId());
            return false;
        }
    }
    return true;
}

bool AppManager::initRequestList()
{
    if (!_appDataList)
    {
        return false;
    }
    uint8_t appDataListSize = _appDataList->getAppDataListSize();
    for (uint16_t i = 0; i<appDataListSize; i++) {
        AppDataBase* appData = _appDataList->getAppDataWithIndex(i);
        if (!addAppSchduler(appData->id))
        {
            return false;
        }
    }
    return true;
}


void AppManager::showBluetoothIcon()
{
    if (dot2d::Director::getInstance()->getRunningScene()!=_systemApp)
    {
        dot2d::Director::getInstance()->replaceScene(((dot2d::CoreAppSystemState*)_systemApp));
    }
    ((dot2d::CoreAppSystemState*)_systemApp)->showBluetoothIcon();
}

void AppManager::showWifiIcon()
{
    if (dot2d::Director::getInstance()->getRunningScene()!=_systemApp)
    {
        dot2d::Director::getInstance()->replaceScene(((dot2d::CoreAppSystemState*)_systemApp));
    }
    ((dot2d::CoreAppSystemState*)_systemApp)->showWifiIcon();
}
void AppManager::showReset()
{
    if (dot2d::Director::getInstance()->getRunningScene()!=_systemApp)
    {
        dot2d::Director::getInstance()->replaceScene(((dot2d::CoreAppSystemState*)_systemApp));
    }
    ((dot2d::CoreAppSystemState*)_systemApp)->showReset();
}

void AppManager::showSetting()
{
    if (dot2d::Director::getInstance()->getRunningScene()!=_systemApp)
    {
        dot2d::Director::getInstance()->replaceScene(((dot2d::CoreAppSystemState*)_systemApp));
    }
    ((dot2d::CoreAppSystemState*)_systemApp)->showSetting();
}

void AppManager::showUpdate()
{
    if (dot2d::Director::getInstance()->getRunningScene()!=_systemApp)
    {
        dot2d::Director::getInstance()->replaceScene(((dot2d::CoreAppSystemState*)_systemApp));
    }
    ((dot2d::CoreAppSystemState*)_systemApp)->showUpdate();
}


bool AppManager::getAppListData(Protocols& proto)
{
    uint8_t appCount = _appDataList->getAppDataListSize();
    proto.pushUint8(appCount);
    for (uint8_t i = 0; i < appCount; i++)
    {
        AppDataBase* appData = _appDataList->getAppDataWithIndex(i);
        appData->enBegin();
        appData->encodeSummary();
        proto.pushProtocol(*appData);
    }
    return true;
}

bool AppManager::getAppData(uint8_t id,Protocols& proto)
{
    AppDataBase* appData = _appDataList->getAppDataWithId(id);
    if (!appData)
    {
        return false;
    }
    appData->enBegin();
    appData->encode(SDTData::DataSourceType::Net);
    proto.pushProtocol(*appData);
    return true;
}

bool AppManager::setAppData(uint8_t id,uint8_t* buffer,uint32_t size,Protocols& proto)
{
    AppDataBase* appData = _appDataList->getAppDataWithId(id);
    if (!appData)
    {
        proto.pushUint8(1);
        return false;
    }
#ifndef TARGET_OS_MAC
    NET_MUTEX_LOCK();
#endif
    appData->deBegin(buffer,size);
    bool res = appData->decode(SDTData::DataSourceType::Net);
    this->executeAppSchduleTask(id);
    if (res)
    {
        proto.pushUint8(0);
        appData->enBegin();
        appData->encodeSummary();
        proto.pushProtocol(*appData);
    }else
    {
        proto.pushUint8(1);
    }
    _appDataList->saveData();

#ifndef TARGET_OS_MAC
    xMESSAGE msg(&AppManager::mtfSaveConfigData);
    NET_QUEUE_SEND(&msg);
#endif

#ifndef TARGET_OS_MAC
    NET_MUTEX_UNLOCK();
#endif
    return res;
}    
