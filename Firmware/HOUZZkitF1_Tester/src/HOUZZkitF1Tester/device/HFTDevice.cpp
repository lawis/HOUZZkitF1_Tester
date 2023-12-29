#include "HFTDevice.h"

HFTDevice::HFTDevice()
{

};

HFTDevice::~HFTDevice()
{

};

void HFTDevice::init()
{
    this->relayOff();
}


void HFTDevice::relayOn()
{
    pinMode(RELAY_PIN,OUTPUT);
    digitalWrite(RELAY_PIN,LOW);
}

void HFTDevice::relayOff()
{
    pinMode(RELAY_PIN,OUTPUT);
    digitalWrite(RELAY_PIN,HIGH);
}