#include "HFTEncoder.h"

HFTEncoder::HFTEncoder()
{

}

HFTEncoder::~HFTEncoder()
{
}

void HFTEncoder::init(uint8_t pinA, uint8_t pinB, uint8_t pinButton)
{
    _encoder = new Encoder(pinA, pinB);
    _button = new OneButton(pinButton,true,true);
}

void HFTEncoder::attachClick(callbackFunction newFunction)
{
    _button->attachClick(newFunction);
}

void HFTEncoder::attachRotation(RotationCallbackFunction newFunction)
{
    _rotationCallbackFunc = newFunction;
}

void HFTEncoder::tick()
{
    _button->tick();
    long newPos = _encoder->read() / 4;
    if (newPos != _prePosition)
    {
        if (_prePosition < newPos)
        {
            _rotationCallbackFunc(RT_RIGHT);
        }else if (_prePosition > newPos)
        {
            _rotationCallbackFunc(RT_LEFT);
        }
        _prePosition = newPos;
    }
}
