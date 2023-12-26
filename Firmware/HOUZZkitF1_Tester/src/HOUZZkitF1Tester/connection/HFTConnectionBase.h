#ifndef __HFT_CONNECTIONBASE_H__
#define __HFT_CONNECTIONBASE_H__

#include <Arduino.h>

class ConnectionBase;

class ConnectionBaseDelegate
{
public:

    ConnectionBaseDelegate(){};

    ~ConnectionBaseDelegate(){};

    virtual void dataParse(std::string& data,ConnectionBase* connection) = 0;

    virtual void dataParse(uint8_t* data, size_t length,ConnectionBase* connection) = 0;

};

class ConnectionBase
{
private:


protected:

    ConnectionBaseDelegate* _delegate = nullptr;

public:

    ConnectionBase(ConnectionBaseDelegate *delegate)
    :_delegate(delegate)
    {};

    virtual ~ConnectionBase(){};

    virtual void sendData(std::string& data){};

    virtual void sendData(uint8_t* data, size_t length){};

    virtual void receivedData(std::string& data){};

    virtual void receivedData(uint8_t* data, size_t length){};
};





#endif //__HFT_CONNECTIONBASE_H__