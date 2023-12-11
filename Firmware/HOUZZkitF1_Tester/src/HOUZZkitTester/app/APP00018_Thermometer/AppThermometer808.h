#ifndef __APP_Thermometer_808_H__
#define __APP_Thermometer_808_H__


#include "AppThermometer.h"

NS_DT_BEGIN


class AppThermometerLayer808 : public AppThermometerLayer
{
public:

    bool initLayer() override;

    APP_LAYER_CREATE_WITH_APP(AppThermometerLayer808,AppThermometer);

};


NS_DT_END


#endif //__APP_Thermometer_808_H__