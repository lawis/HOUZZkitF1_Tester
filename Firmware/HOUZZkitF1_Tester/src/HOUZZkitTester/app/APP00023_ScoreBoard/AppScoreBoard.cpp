#include "AppScoreBoard.h"
#include "AppScoreBoard16016.h"
#include "AppScoreBoard12012.h"
#include "AppScoreBoard808.h"
#include "AppScoreBoard3208.h"
#include "AppScoreBoard32016.h"


NS_DT_BEGIN

AppDataScoreBoard::AppDataScoreBoard()
:redScore(0)
,blueScore(0)
,redWinCount(0)
,blueWinCount(0)
{
    scheduleType = AppDataBase::ScheduleType::NONE;
    scheduleInterval = 0;

    appAlias = "计分板";
    canPlay = true;
    openPlay = true;
    isActive = true;
    this->setTheme(theme);
}

bool AppDataScoreBoard::subEncode(SDTData::DataSourceType type)
{
    pushUint8(theme);
    return true;
}

bool AppDataScoreBoard::subDecode(SDTData::DataSourceType type)
{
    this->setTheme(popUint8());
    return true;
}

void AppDataScoreBoard::redAddScore(int8_t v)
{
    redScore+=v;
    redScore = MIN(redScore,999);
    redScore = MAX(redScore,0);
}

void AppDataScoreBoard::blueAddScore(int8_t v)
{
    blueScore+=v;
    blueScore = MIN(blueScore,999);
    blueScore = MAX(blueScore,0);
}

void AppDataScoreBoard::redAddWinCount(int8_t v)
{
    redWinCount+=v;
    redWinCount = MIN(redWinCount,3);
    redWinCount = MAX(redWinCount,0);
}

void AppDataScoreBoard::blueAddWinCount(int8_t v)
{
    blueWinCount+=v;
    blueWinCount = MIN(blueWinCount,3);
    blueWinCount = MAX(blueWinCount,0);
}

void AppDataScoreBoard::setTheme(uint8_t t)
{
    theme = t;
    switch (t) {
        case 0:
            appTheme.copyTheme(*(AppManager::getInstance()->globalAppTheme));
            break;
        default:
            AppManager::getInstance()->setTheme((AppManager::GlobalAppThemeType)t,&appTheme);
            break;
    }
}

void AppScheduleScoreBoard::scheduleAction(float dt)
{
    
}


bool AppScoreBoard::init()
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
        rootLayer = AppScoreBoardLayer3208::createWithApp(this,Size(32,8));
    }
        break;
    case AppDataBase::AppSizeType::S16016:
    {
        rootLayer = AppScoreBoardLayer16016::createWithApp(this,Size(16,16));
    }
        break;
    case AppDataBase::AppSizeType::S808:
    {
        rootLayer = AppScoreBoardLayer808::createWithApp(this,Size(8,8));
    }
        break;
    case AppDataBase::AppSizeType::S12012:
    {
        rootLayer = AppScoreBoardLayer12012::createWithApp(this,Size(12,12));
    }
        break;
    case AppDataBase::AppSizeType::S32016:
    {
        rootLayer = AppScoreBoardLayer32016::createWithApp(this,Size(32,16));
    }
        break;
    default:
    {
        rootLayer = AppScoreBoardLayer::createWithApp(this,Size::ZERO);
    }
        break;
    }
    this->addChild(rootLayer);

    
    auto listener = EventListenerButton::create();
    listener ->onBtnClick = [&](int8_t keyCode , Event * event ){
        // LOG("AppScoreBoard onBtnClick keyCode:%d",keyCode);
        AppDataScoreBoard *appData = this->getData();
        switch (keyCode)
        {
        case 0:
        {
            appData->blueAddScore(1);
        }
            break;
        case 1:
        {
            appData->blueAddScore(-1);
        }
            break;
        case 2:
        {
            appData->redAddScore(-1);
        }
            break;
        case 3:
        {
            appData->redAddScore(1);
        }
            break;
        case 4:
        {
            if (appData->redScore > appData->blueScore)
            {
                appData->redAddWinCount(1);
                appData->redScore = 0;
                appData->blueScore = 0;
            }else if (appData->redScore < appData->blueScore)
            {
                appData->blueAddWinCount(1);
                appData->redScore = 0;
                appData->blueScore = 0;
            }else
            {
                appData->redAddWinCount(1);
                appData->blueAddWinCount(1);
                appData->redScore = 0;
                appData->blueScore = 0;
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


    listener -> onBtnDoubleClick = [&](int8_t keyCode , Event * event )
    {
        AppDataScoreBoard *appData = this->getData();
        switch (keyCode)
        {
        case 0:
        {
            appData->blueAddScore(2);
        }
            break;
        case 1:
        {
            appData->blueAddScore(-2);
        }
            break;
        case 2:
        {
            appData->redAddScore(-2);
        }
            break;
        case 3:
        {
            appData->redAddScore(2);
        }
            break;
        case 4:
        {
            appData->redScore = 0;
            appData->blueScore = 0;
            appData->redWinCount = 0;
            appData->blueWinCount = 0;
        }
        case 5:
        {

        }
            break;
        default:
            break;
        }
    };

     listener -> onBtnDuringLongPress = [&](int8_t keyCode , Event * event )
     {
         AppDataScoreBoard *appData = this->getData();
         switch (keyCode)
         {
         case 0:
         {
             appData->blueAddScore(1);
         }
             break;
         case 1:
         {
             appData->blueAddScore(-1);
         }
             break;
         case 2:
         {
             appData->redAddScore(-1);
         }
             break;
         case 3:
         {
             appData->redAddScore(1);
         }
             break;
         default:
             break;
         }
#ifndef TARGET_OS_MAC
         vTaskDelay(100);
#endif
     };
    _eventDispatcher->addEventListenerWithSceneGraphPriority ( listener, this );

    return true;
}


bool AppScoreBoardLayer::initLayer()
{
    return true;
}

NS_DT_END
