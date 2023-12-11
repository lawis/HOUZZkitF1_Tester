#ifndef __Data_Style_Icon_Num_H__
#define __Data_Style_Icon_Num_H__


#include "Dot2D/dot2d.h"
#include "HOUZZkitTester/app/SDTAppManager.h"

NS_DT_BEGIN

class StyleIconNumTheme : public AppTheme
{
public:
    

};


class StyleIconNumData
{
public:
    StyleIconNumData();
    
    StyleIconNumTheme appTheme;

    bool isCountFormat;

    bool hideIcon;

    uint8_t scrollType;

    uint8_t numberSize;
    
    uint8_t theme;
    
};

NS_DT_END


#endif //__Data_Style_Icon_Num_H__
