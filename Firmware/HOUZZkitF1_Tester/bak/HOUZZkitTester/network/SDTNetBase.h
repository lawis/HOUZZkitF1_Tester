#ifndef __SDT_NETBASE_H__
#define __SDT_NETBASE_H__

#include <Arduino.h>
#include "Dot2D/dot2d.h"
#include "../device/SDTDevice.h"

class NetBase;

class NetBaseDelegate
{
public:

    NetBaseDelegate(){};

    ~NetBaseDelegate(){};

    virtual void dataParse(std::string& data,NetBase* net) = 0;

    virtual void dataParse(uint8_t* data, size_t length,NetBase* net) = 0;

};

class NetBase
{
private:


protected:

    NetBaseDelegate* _delegate = nullptr;

public:

    NetBase(NetBaseDelegate *delegate)
    :_delegate(delegate)
    {};

    virtual ~NetBase(){};

    virtual void sendData(std::string& data){};

    virtual void sendData(uint8_t* data, size_t length){};

    virtual void receivedData(std::string& data){};

    virtual void receivedData(uint8_t* data, size_t length){};
};





#endif //__SDT_NETBASE_H__