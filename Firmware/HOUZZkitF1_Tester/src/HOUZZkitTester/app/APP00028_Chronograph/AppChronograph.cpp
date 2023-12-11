#include "AppChronograph.h"
#include "AppChronograph3208.h"
#include "HOUZZkitTester/SDTSystem.h"

NS_DT_BEGIN

AppDataChronograph::AppDataChronograph()
:_theme(0)
,startMillisecond(0)
,tempMillisecond(0)
,recordNum(0)
,recordPointer(0)
,isRunning(false)
{
    scheduleType = AppDataBase::ScheduleType::NONE;
    scheduleInterval = 0;

    for (size_t i = 0; i < recordMaxNum-1; i++)
    {
        recordMillisecond[i] = 0;
    }

    appAlias = "秒表";
    canPlay = true;
    openPlay = true;
    isActive = true;
    this->setTheme(_theme);
}

bool AppDataChronograph::subEncode(SDTData::DataSourceType type)
{
    pushUint8(_theme);
    return true;
}

bool AppDataChronograph::subDecode(SDTData::DataSourceType type)
{
    this->setTheme(popUint8());
    return true;
}


void AppDataChronograph::setTheme(uint8_t t)
{
    _theme = t;
    switch (t) {
        case 0:
            appTheme.copyTheme(*(AppManager::getInstance()->globalAppTheme));
            appTheme.recordBgColor = appTheme.bgColor;
            appTheme.recordFgColor = appTheme.fgColor;
            appTheme.curRecordBgColor = DTRGB(30,0,0);
            appTheme.curRecordFgColor = DTRGB(255,0,0);
            break;
        default:
            AppManager::getInstance()->setTheme((AppManager::GlobalAppThemeType)t,&appTheme);
            appTheme.recordBgColor = appTheme.bgColor;
            appTheme.recordFgColor = appTheme.fgColor;
            appTheme.curRecordBgColor = DTRGB(30,0,0);
            appTheme.curRecordFgColor = DTRGB(255,0,0);
            break;
    }
}

void AppDataChronograph::reset()
{
    startMillisecond = 0;
    tempMillisecond = 0;
    recordNum = 0;
    recordPointer = 0;
    for (size_t i = 0; i < recordMaxNum-1; i++)
    {
        recordMillisecond[i] = 0;
    }
}

void AppDataChronograph::start()
{
    startMillisecond = millis();
    recordPointer = recordNum;
    isRunning = true;
}

void AppDataChronograph::stop()
{
    tempMillisecond += millis() - startMillisecond;
    isRunning = false;
}

void AppDataChronograph::record()
{
    if (recordNum < recordMaxNum)
    {
        recordMillisecond[recordNum++] = tempMillisecond + millis() - startMillisecond;
        recordPointer = recordNum;
    }
}

void AppDataChronograph::next()
{
    if (isRunning)
    {
        return;
    }
    recordPointer++;
    if (recordPointer > recordNum)
    {
        recordPointer = 0;
    }
}

void AppDataChronograph::previous()
{
    if (isRunning)
    {
        return;
    }
    recordPointer--;
    if (recordPointer < 0)
    {
        recordPointer = recordNum;
    }
}

bool AppScheduleChronograph::parseRequest(const String& res)
{
    return true;
}

void AppScheduleChronograph::scheduleAction(float dt)
{
    
}


bool AppChronograph::init()
{
    AppDataBase::AppSizeType type = this->getData()->appSizeType;
    if (type == AppDataBase::AppSizeType::AUTO)
    {
        type = this->getAppSizeTypeByScreen();
    }
    switch (type)
    {
    case AppDataBase::AppSizeType::S3208:
    {
        rootLayer = AppChronographLayer3208::createWithApp(this,Size(32,8));
    }
        break;
    default:
    {
        rootLayer = AppChronographLayer::createWithApp(this,Size::ZERO);
    }
        break;
    }
    this->addChild(rootLayer);

    auto listener = EventListenerButton::create();
    listener ->onBtnClick = [&](int8_t keyCode , Event * event ){
        switch (keyCode)
        {
            case 0:
            {
                
            }
                break;
            case 1:
            {
                this->getData()->next();
            }
                break;
            case 2:
            {
                if (this->getData()->isRunning)
                {
                    //计次
                    this->getData()->record();
                }else
                {
                    //复位
                    this->getData()->reset();
                }
            }
                break;
            case 3:
            {
                this->getData()->previous();
            }
                break;
            case 4:
            {
                if (this->getData()->isRunning)
                {
                    this->getData()->stop();
                }else
                {
                    this->getData()->start();
                }
            }
                break;
            case 5:
            {

            }
                break;
            default:
                break;
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority ( listener, this );

    return true;
}


void AppChronograph::buttonEventEnable()
{
    DEVICE.checkButtonClickOnly(true);
    rootLayer->resumeEventDispatcher();
}

void AppChronograph::buttonEventDisable()
{
    DEVICE.checkButtonClickOnly(false);
    rootLayer->pauseEventDispatcher();
}

bool AppChronographLayer::initLayer()
{
    return true;
}

NS_DT_END