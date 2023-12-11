#ifndef __APP_Thermometer_12012_H__
#define __APP_Thermometer_12012_H__


#include "AppThermometer.h"

NS_DT_BEGIN


class AppThermometerLayer12012 : public AppThermometerLayer
{
public:

    bool initLayer() override;

    APP_LAYER_CREATE_WITH_APP(AppThermometerLayer12012,AppThermometer);

};


NS_DT_END


#endif //__APP_Thermometer_12012_H__