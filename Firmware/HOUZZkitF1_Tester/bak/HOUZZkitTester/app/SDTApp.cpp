#include "SDTApp.h"
#include "Dot2D/dtLayer.h"
#include "Dot2D/dtSprite.h"
#include "Dot2D/dtDirector.h"
#include "Fonts/TomThumb.h"
#include "Dot2D/dtActionManager.h"
#include "Dot2D/dtActionInterval.h"
#include "Dot2D/dtActionInstant.h"
#include "Dot2D/base/dtEventListenerButton.h"
#include "Dot2D/base/dtEventListenerPS2.h"
#include "HOUZZkitTester/device/SDTDevice.h"
#include "HOUZZkitTester/network/SDTNetManager.h"

NS_DT_BEGIN

SDTApp::SDTApp()
{

}

SDTApp::~SDTApp()
{

}

bool SDTApp::init()
{
    TextSprite *timeText = TextSprite::create(Size(32,5),Size(32,5),"13:56:48",TextSprite::TextAlign::TextAlignCenter,&TomThumb);
    timeText->setTransparent(true);
    timeText->setPosition(0,1);
    this->addChild(timeText);
    return true;
}







    

SDTApp* SDTApp::create()
{
    SDTApp *ret = new SDTApp();
    if (ret && ret->init())
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        DT_SAFE_DELETE(ret);
        return nullptr;
    }
}

void SDTApp::update(float delta)
{
    
}

NS_DT_END