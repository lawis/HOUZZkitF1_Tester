#include "AppBase.h"
#include "SDTAppManager.h"


AppDataBase::AppDataBase()
:scheduleType(ScheduleType::NONE)
,scheduleInterval(0)
,id(0)
,appAlias("")
,showDuration(15)
,canPlay(false)
,openPlay(false)
,isActive(false)
,appSizeType(AppSizeType::AUTO)
,appAlignType(AppAlignType::CENTER)
{
    
}

bool AppDataBase::init()
{
    return true;
}

uint16_t AppDataBase::APPID()
{
    return 0;
}

uint16_t AppDataBase::appId()
{
    return 0;
}

bool AppDataBase::appAddInit()
{
    return true;
}

bool AppDataBase::encodeSummary()
{
    pushUint8(id)
    .pushUint16(appId())
    .pushString8(appAlias.c_str())
    .pushUint16(showDuration)
    .enByteBegin()
    .pushBit(canPlay)
    .pushBit(openPlay)
    .pushBit(isActive)
    .pushByte();
    return true;
}

bool AppDataBase::encode(SDTData::DataSourceType type)
{
    encodeSummary();
    pushUint8((uint8_t)appSizeType);
    pushUint8((uint8_t)appAlignType);
    return subEncode(type);

}

bool AppDataBase::decode(SDTData::DataSourceType type)
{
    id = popUint8();
    popUint16();//pop appID
    appAlias = popString8().c_str();
    if (this->decodeError()) {
        return false;
    }
    showDuration = popUint16();
    popByte();
    canPlay = popBit();
    openPlay = popBit();
    isActive = popBit();
    appSizeType = (AppSizeType)popUint8();
    appAlignType = (AppAlignType)popUint8();
    bool res = subDecode(type);
    detectActive();
    return res;
}


uint32_t AppDataBase::fileId()
{
    uint32_t res = this->appId();
    res = res << 8;
    res += this->id;
    return res;
}


//---------------------------AppLayerBase-----------------------
bool AppLayerBase::initWithApp(AppBase* app,const dot2d::Size& size)
{
    if (!app){return false;}
    this->app = app;
    this->initSize(size);
    return initLayer();
}

void AppLayerBase::initSize(const dot2d::Size& size)
{
    if (size != dot2d::Size::ZERO)
    {
        this->setContentSize(size);
    }else
    {
        this->setContentSize(dot2d::Director::getInstance()->getCanvasSize());
    }

    dot2d::Vec2 pos(0,0);
    dot2d::Size screenSize = dot2d::Director::getInstance()->getCanvasSize();
    dot2d::Size contentSize = this->getContentSize();
    switch (this->app->appData->appAlignType)
    {
        case AppDataBase::AppAlignType::CENTER:
        {
            pos.x = (screenSize.width - contentSize.width) / 2;
            pos.y = (screenSize.height - contentSize.height) / 2;
        }
            break;
        case AppDataBase::AppAlignType::TOPLEFT:
        {
            pos.x = 0;
            pos.y = 0;
        }
            break;
        case AppDataBase::AppAlignType::TOPCENTER:
        {
            pos.x = (screenSize.width - contentSize.width) / 2;
            pos.y = 0;
        }
            break;
        case AppDataBase::AppAlignType::TOPRIGHT:
        {
            pos.x = (screenSize.width - contentSize.width);
            pos.y = 0;
        }
            break;
        case AppDataBase::AppAlignType::CENTERLEFT:
        {
            pos.x = 0;
            pos.y = (screenSize.height - contentSize.height) / 2;
        }
            break;
        case AppDataBase::AppAlignType::CENTERRIGHT:
        {
            pos.x = (screenSize.width - contentSize.width);
            pos.y = (screenSize.height - contentSize.height) / 2;
        }
            break;
        case AppDataBase::AppAlignType::BOTTOMLEFT:
        {
            pos.x = 0;
            pos.y = (screenSize.height - contentSize.height);
        }
            break;
        case AppDataBase::AppAlignType::BOTTOMCENTER:
        {
            pos.x = (screenSize.width - contentSize.width) / 2;
            pos.y = (screenSize.height - contentSize.height);
        }
            break;
        case AppDataBase::AppAlignType::BOTTOMRIGHT:
        {
            pos.x = (screenSize.width - contentSize.width);
            pos.y = (screenSize.height - contentSize.height);
        }
            break;
        
        default:
            break;
    }
    this->setPosition(pos);
}

dot2d::Size AppLayerBase::getSizeByAppSizeType(AppDataBase::AppSizeType type)
{
    dot2d::Size size = dot2d::Size::ZERO;
    switch (type)
    {
    case AppDataBase::AppSizeType::S3208:
    {
        size.width = 32;
        size.height = 8;
    }
        break;
    case AppDataBase::AppSizeType::S16016:
    {
        size.width = 16;
        size.height = 16;
    }
        break;
    case AppDataBase::AppSizeType::S808:
    {
        size.width = 8;
        size.height = 8;
    }
        break;
    case AppDataBase::AppSizeType::S12012:
    {
        size.width = 12;
        size.height = 12;
    }
        break;
    case AppDataBase::AppSizeType::S32016:
    {
        size.width = 32;
        size.height = 16;
    }
        break;
    default:
    {
        
    }
        break;
    }
    return size;
}







//---------------------------AppBase-----------------------


bool AppBase::initWithData(AppDataBase* data)
{
    appData = data;
    appData->retain();
    return this->init();
}

void AppBase::autoSwitchUpdate(float dt)
{
    AppManager::getInstance()->playNextPlayableApp();
}

AppBase::AppBase()
:appData(nullptr)
,rootLayer(nullptr)
{
    DT_SAFE_RELEASE(appData);
}

AppBase::~AppBase()
{
    
}

void AppBase::onEnter()
{
    Node::onEnter();
    this->pauseEventDispatcher();
}

uint16_t AppBase::APPID()
{
    return 0;
}

uint16_t AppBase::appId()
{
    return 0;
}

AppDataBase* AppBase::getData()
{
    return appData;
}

bool AppBase::setAutoSwitch()
{
    if (appData->showDuration > 0)
    {
        this->scheduleOnce(DT_SCHEDULE_SELECTOR(AppBase::autoSwitchUpdate),appData->showDuration);
        return true;
    }
    return false;
}

bool AppBase::removeAutoSwitch()
{
    this->unschedule(DT_SCHEDULE_SELECTOR(AppBase::autoSwitchUpdate));
    return true;
}

void AppBase::resumeEventDispatcherDt(float dt)
{
    this->resumeEventDispatcher();
}

void AppBase::pauseEventDispatcherDt(float dt)
{
    this->pauseEventDispatcher();
}

void AppBase::resetLayer()
{
    if(!rootLayer)
    {
        this->init();
        return;
    }else{
        if(this->getData()->appSizeType != AppDataBase::AppSizeType::AUTO &&
           AppLayerBase::getSizeByAppSizeType(this->getData()->appSizeType) != rootLayer->getContentSize())
        {
            rootLayer->removeFromParentAndCleanup(true);
            rootLayer = nullptr;
            this->init();
        }else
        {
            rootLayer->initSize(rootLayer->getContentSize());
        }
    }
}

void AppBase::pushRequestTask(const String& url,bool ssl,dot2d::Ref* target,AppBase::HandleRequestCallback callback,uint8_t retryTimes)
{
    NetManager::getInstance()->pushRequestTask(url,ssl,target,callback,retryTimes);
}

AppDataBase::AppSizeType AppBase::getAppSizeTypeByScreen()
{
    dot2d::Size size = dot2d::Director::getInstance()->getCanvasSize();
    if(size == dot2d::Size(32,8))
    {
        return AppDataBase::AppSizeType::S3208;
    }
    if(size == dot2d::Size(16,16))
    {
        return AppDataBase::AppSizeType::S16016;
    }
    if(size == dot2d::Size(8,8))
    {
        return AppDataBase::AppSizeType::S808;
    }
    if(size == dot2d::Size(12,12))
    {
        return AppDataBase::AppSizeType::S12012;
    }
    if(size == dot2d::Size(32,16))
    {
        return AppDataBase::AppSizeType::S32016;
    }
    return AppDataBase::AppSizeType::AUTO;
}

//---------------------------AppScheduleBase-----------------------

bool AppScheduleBase::init()
{
    return true;
}

bool AppScheduleBase::initWithData(AppDataBase* data)
{
    appData = data;
    appData->retain();
    return this->init();
}

WebRequestTask* AppScheduleBase::pushRequestTask(const String& url,bool ssl,dot2d::Ref* target,AppScheduleBase::HandleRequestCallback callback,uint8_t retryTimes)
{
    return NetManager::getInstance()->pushRequestTask(url,ssl,target,callback,retryTimes);
}

AppScheduleBase::AppScheduleBase()
:appData(nullptr)
{

}

AppScheduleBase::~AppScheduleBase()
{
    DT_SAFE_RELEASE(appData);
}

uint16_t AppScheduleBase::APPID()
{
    return 0;
}

uint16_t AppScheduleBase::appId()
{
    return 0;
}

AppDataBase* AppScheduleBase::getData()
{
    return appData;
}

void AppScheduleBase::startSchedule(bool executeNow)
{
    if (executeNow) {
        this->startScheduleAction(0);
    }
    this->schedule(DT_SCHEDULE_SELECTOR(AppScheduleBase::startScheduleAction),appData->scheduleInterval);
}

void AppScheduleBase::startScheduleAction(float dt)
{
    if (!this->getData()->isActive)
    {
        return;
    }
    this->scheduleAction(dt);
}

void AppScheduleBase::scheduleAction(float dt)
{
    LOG("ERROR!!! ScheduleAction must be overload");
}
    
String AppScheduleBase::substringBetween(const String& str,const String& left,const String& right)
{
    uint32_t beginIndex = str.indexOf(left);
    if (beginIndex == UINT32_MAX)
    {
        return "";
    }
    beginIndex += left.length();
    uint32_t endIndex = str.indexOf(right,beginIndex);
    if (endIndex == UINT32_MAX || endIndex<=beginIndex)
    {
        return "";
    }
    return str.substring(beginIndex, endIndex);
}