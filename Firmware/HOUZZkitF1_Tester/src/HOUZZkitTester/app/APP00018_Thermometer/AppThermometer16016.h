#ifndef __APP_Thermometer_16016_H__
#define __APP_Thermometer_16016_H__


#include "AppThermometer.h"

NS_DT_BEGIN


class AppThermometerLayer16016 : public AppThermometerLayer
{
public:

    bool initLayer() override;

    APP_LAYER_CREATE_WITH_APP(AppThermometerLayer16016,AppThermometer);

};


NS_DT_END


#endif //__APP_Thermometer_16016_H__