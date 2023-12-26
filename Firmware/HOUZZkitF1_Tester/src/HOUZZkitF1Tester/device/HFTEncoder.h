#ifndef __HFT_ENCODER_H__
#define __HFT_ENCODER_H__

#include <Arduino.h>
#define ENCODER_DO_NOT_USE_INTERRUPTS
#include <Encoder.h> 
#include <OneButton.h>

enum RotationType
{
    RT_LEFT = 1,
    RT_RIGHT
};

extern "C" {
typedef void (*RotationCallbackFunction)(RotationType);
}


class HFTEncoder{

private:

    Encoder *_encoder = nullptr;

    OneButton *_button = nullptr;

    long _prePosition = 0;

    RotationCallbackFunction _rotationCallbackFunc = nullptr;

public:

    HFTEncoder();

    ~HFTEncoder();

    void init(uint8_t pinA, uint8_t pinB, uint8_t pinButton);

    void attachClick(callbackFunction newFunction);

    void attachRotation(RotationCallbackFunction newFunction);

    void tick();

};

#endif //__HFT_ENCODER_H__