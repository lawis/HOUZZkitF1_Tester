#include "CoreAppSystemState808.h"
#include "HOUZZkitTester/SDTIconLib.h"
#include "HOUZZkitTester/SDTMaroc.h"
#include "HOUZZkitTester/SDTSystem.h"
#ifndef TARGET_OS_MAC
#include "HttpsOTAUpdateT0.h"
#endif

NS_DT_BEGIN

bool CoreAppSystemStateLayer808::initLayer()
{
    _text = nullptr;
    _icon = FrameSprite::create();
    // _icon->setTransparent(true);
    this->addChild(_icon);
    return true;
}

void CoreAppSystemStateLayer808::showBluetoothIcon()
{
    uint16_t size = GetUint16(icon_bluetooth);
    _icon->updateFrameData(icon_bluetooth+2,size);
    _icon->setAutoSwitch(false);
}

void CoreAppSystemStateLayer808::showBluetoothCloseIcon()
{
    uint16_t size = GetUint16(icon_bluetooth_close);
    _icon->updateFrameData(icon_bluetooth_close+2,size);
    _icon->setAutoSwitch(false);
}

void CoreAppSystemStateLayer808::showWifiIcon()
{
    uint16_t size = GetUint16(icon_wifi);
    _icon->updateFrameData(icon_wifi+2,size);
    _icon->setAutoSwitch(true);
}

void CoreAppSystemStateLayer808::showReset()
{
    uint16_t size = GetUint16(icon_reset);
    _icon->updateFrameData(icon_reset+2,size);
    _icon->setAutoSwitch(true);
}

void CoreAppSystemStateLayer808::showSetting()
{
    uint16_t size = GetUint16(icon_setting);
    _icon->updateFrameData(icon_setting+2,size);
    _icon->setAutoSwitch(true);
}

void CoreAppSystemStateLayer808::showUpdate()
{
    if (!_text)
    {
        _text = TextSprite::create(Size(8,5),Size(8,5),"",TextSprite::TextAlign::TextAlignLeft,&TomThumb);
        _text->setPaddingLeft(0);
        _text->getSpriteCanvas()->setTextHSpace(1);
        _text->setTransparent(true);
        _text->setPosition(0,2);
        this->addChild(_text);
    }
    
    _text->getSpriteCanvas()->setTextColor(DTRGB(255,0,0));
    _text->getSpriteCanvas()->canvasReset();
    _text->getSpriteCanvas()->printf("%02d",0);
    _text->setVisible(true);
    _icon->setVisible(false);

    this->schedule(DT_SCHEDULE_SELECTOR(CoreAppSystemStateLayer808::updateUpdate),0.1);
}

void CoreAppSystemStateLayer808::updateUpdate(float dt)
{
    uint16_t percent = SDTSystem::getInstance()->getUpdatePercent() * 100;
    SpriteCanvas* canvas = _text->getSpriteCanvas();
    canvas->canvasReset();
    canvas->printf("%02d",percent);

    if (percent >= 100)
    {
        _icon->setVisible(true);
        _text->setVisible(false);
        this->showReset();
    }
}

NS_DT_END
