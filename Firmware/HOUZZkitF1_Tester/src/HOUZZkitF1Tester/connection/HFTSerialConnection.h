#ifndef __HFT_SERIAL_CONNECTION_H__
#define __HFT_SERIAL_CONNECTION_H__

#include <Arduino.h>
#include "HFTConnectionBase.h"


class SerialConnection : public ConnectionBase
{

private:

    enum SerialType
    {
        ST_HARD = 1,
        ST_SOFT
    };

    SerialType _serialType;

    ConnectionBaseDelegate* _delegate = nullptr;

public:

    SerialConnection(ConnectionBaseDelegate *delegate);

    ~SerialConnection();

    bool init(uint8_t txPin,uint8_t rxPin); //初始化软串口

    bool init(HardwareSerial* serial);      //初始化硬串口

    void loop();

};

#endif //__HFT_SERIAL_CONNECTION_H__