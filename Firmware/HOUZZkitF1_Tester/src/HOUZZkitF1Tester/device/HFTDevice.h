#ifndef __HFT_DEVICE_H__
#define __HFT_DEVICE_H__

#include <Arduino.h>
#include "avdweb_Switch.h"

#define RELAY_PIN  8

class HFTDevice{

private:

    Switch toggleSwitch = Switch(RELAY_PIN);

public:

    HFTDevice();

    ~HFTDevice();

    void init();

    void relayOn();

    void relayOff();

};

#endif //__HFT_DEVICE_H__