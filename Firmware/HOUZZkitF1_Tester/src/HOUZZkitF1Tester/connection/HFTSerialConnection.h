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
        ST_HARD_INNER,
        ST_SOFT
    };

    SerialType _serialType;

    HardwareSerial* _hardSerial = nullptr;

    String test;

    bool _synchronous = false;

public:

    SerialConnection(ConnectionBaseDelegate *delegate);

    ~SerialConnection();

    bool init(uint8_t txPin,uint8_t rxPin); //初始化软串口

    bool initInner(uint8_t rxPin,uint8_t txPin); //初始化内部硬串口

    bool init(HardwareSerial* serial);      //初始化硬串口

    void clear(); //清空串口缓存

    String readString(); //读取串口数据

    void sendString(uint16_t pid, const String& data);

    String sendString(uint16_t pid, const String& data, float timeout); //发送字符串并等待返回

    void sendDebug(const String& data);

    String readString(float timeout);

    void receivedData(const String &data);

    void loop();

};

#endif //__HFT_SERIAL_CONNECTION_H__