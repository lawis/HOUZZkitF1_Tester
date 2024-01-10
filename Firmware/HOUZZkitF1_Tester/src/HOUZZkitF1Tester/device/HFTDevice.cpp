#include "HFTDevice.h"
#include "CyMCP23016.h"

HFTDevice::HFTDevice()
{

};

HFTDevice::~HFTDevice()
{

};

void HFTDevice::init()
{
    this->relayOff();
    mcpU6.begin(17,18,0x26);
    mcpU7.begin(17,18,0x27);
    for (size_t i = 0; i < 16; i++)
    {
        mcpU6.pinMode(i,INPUT);
        mcpU7.pinMode(i,INPUT);
    }
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

uint8_t HFTDevice::mcpU6Read(uint8_t pin)
{
    return mcpU6.digitalRead(pin);
}

uint8_t HFTDevice::mcpU7Read(uint8_t pin)
{
    return mcpU7.digitalRead(pin);
}