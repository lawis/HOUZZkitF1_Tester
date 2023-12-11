#include "AppBoardCheck.h"
#include "HOUZZkitTester/device/SDTDevice.h"
NS_DT_BEGIN

AppDataBoardCheck::AppDataBoardCheck()
:boardCheckType(BoardCheckType::RGB_FILL)
{
    appAlias = "面板测试";
    canPlay = true;
    openPlay = true;
    isActive = true;
}


bool AppDataBoardCheck::subEncode(SDTData::DataSourceType type)
{
    pushUint8((uint8_t)boardCheckType);
    return true;
}

bool AppDataBoardCheck::subDecode(SDTData::DataSourceType type)
{
    boardCheckType = (BoardCheckType)popUint8();
    return true;
}

void AppScheduleBoardCheck::scheduleAction(float dt)
{
    
}

bool AppBoardCheck::init()
{
    Size size = AppBoardCheckLayer::getSizeByAppSizeType(this->getData()->appSizeType);
    rootLayer = AppBoardCheckLayer::createWithApp(this,size);
    this->addChild(rootLayer);

//     auto listener = EventListenerButton::create();
//     listener ->onBtnClick = [](int8_t keyCode , Event * event ){
//         LOG("onBtnClick keyCode:%d",keyCode);
//         switch (keyCode)
//         {
//         case 0:
//         {
//             // DEVICE.bt201.volumeUp();
//         }
//             break;
//         case 1:
//         {
//             // DEVICE.bt201.prev();
//         }
//             break;
//         case 2:
//         {
//             // DEVICE.bt201.volumeDown();
//         }
//             break;
//         case 3:
//         {
//             // DEVICE.bt201.next();
//         }
//             break;
//         case 4:
//         {
//             // DEVICE.bt201.play_pause();
//         }
//             break;
//         case 5:
//         {
//
//         }
//             break;
//         default:
//             break;
//         }
//     };
//     listener -> onBtnDoubleClick = [](int8_t keyCode , Event * event ){
//         LOG ("onBtnDoubleClick:%d" , keyCode );
//     };
//     listener -> onBtnLongPressStart = [](int8_t keyCode , Event * event ){
//         LOG ("onBtnLongPressStart:%d" , keyCode );
//     };
//     listener -> onBtnLongPressStop = [](int8_t keyCode , Event * event ){
//         LOG ("onBtnLongPressStop:%d" , keyCode );
//     };
//     listener -> onBtnDuringLongPress = [](int8_t keyCode , Event * event ){
//         LOG ("onBtnDuringLongPress:%d" , keyCode );
//     };
//     _eventDispatcher->addEventListenerWithSceneGraphPriority ( listener, this );
//
//     DEVICE.ps2Begin();
//    //  DEVICE.ps2JoystickEnable(true);
//     auto ps2Listener = EventListenerPS2::create();
//     ps2Listener->onPs2Click = [](uint16_t btnCode, uint8_t analog, Event* event)
//     {
//         LOG("onPs2Click Btn:%4x %03d",btnCode,analog);
//     };
//    ps2Listener->onPs2Joystick = [](uint16_t btnCode, uint8_t analog, Event* event)
//    {
//        LOG("onPs2Joystick Btn:%4x %03d",btnCode,analog);
//    };
//     _eventDispatcher->addEventListenerWithSceneGraphPriority ( ps2Listener, this );

    return true;
}

void AppBoardCheckLayer::customUpdate(float dt)
{
    Size size = this->getContentSize();

    switch (this->getData()->boardCheckType)
    {
    case AppDataBoardCheck::BoardCheckType::RGB_FILL:
    {
        DTRGB c;
        if (color < 85) {
            c = DTRGB(255,0,0);
        }else if(color < 170)
        {
            c = DTRGB(0,255,0);
        }else
        {
            c = DTRGB(0,0,255);
        }
        canvas->fillScreen(c);
        color+=2;
        if (color>255)
        {
            color=0;
        }
    }
        break;
    case AppDataBoardCheck::BoardCheckType::DOT_SHOW :
    {
        DTRGB c;
        if (color == 0)
        {
            c = DTRGB(255,0,0);
        }else if (color == 1)
        {
            c = DTRGB(0,255,0);
        }else if (color == 2)
        {
            c = DTRGB(0,0,255);
        }
        canvas->drawPixel(index % size.width,index/size.width,c);
        index++;
        if (index >= size.width*size.height)
        {
            index = 0;
            color++;
            if (color>=3)
            {
                color = 0;
            }
        }
    }
        break;
    case AppDataBoardCheck::BoardCheckType::RAINBOW_FILL:
    {
        DTHSV c(color,255,255);
        canvas->fillScreen(DTRGB(c));
        color+=3;
        if (color>255)
        {
            color=0;
        }
    }
        break;
    default:
        LOG("ERROR!!! AppDataBoardCheck::BoardCheckType");
        break;
    }

}

bool AppBoardCheckLayer::initLayer()
{
    canvas = SpriteCanvas::create(this->getContentSize());
    dot2d::CanvasSprite *sprite = dot2d::CanvasSprite::create(canvas);
    this->addChild(sprite);
    this->schedule(DT_SCHEDULE_SELECTOR(AppBoardCheckLayer::customUpdate),0.05);
    return true;
}

NS_DT_END
