#ifndef __SDT_BUTTON_H__
#define __SDT_BUTTON_H__

#include "OneButton.h"

enum BaseButtonType
{
    BBT_NONE    = -1,
    BBT_UP      = 0,
    BBT_RIGHT   = 1,
    BBT_DOWN    = 2,
    BBT_LEFT    = 3,
    BBT_ENTER   = 4,
    BBT_BACK    = 5
};


class SDTButton : public OneButton
{
private:
    
    BaseButtonType _btnType = BBT_NONE;

public:
    
    SDTButton(const int pin, const boolean activeLow = true, const bool pullupActive = true);
    
    ~SDTButton();

    void setBtnType(BaseButtonType _type){_btnType = _type;}

    BaseButtonType type(){return _btnType;}

};




#endif //__SDT_BUTTON_H__