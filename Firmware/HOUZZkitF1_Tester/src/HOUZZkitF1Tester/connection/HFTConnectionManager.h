#ifndef __HFT_CONNECTION_MANAGER_H__
#define __HFT_CONNECTION_MANAGER_H__

#include "HFTEthernetConnection.h"
#include "SDTProtocol.h"

class ConnectionManager : public Protocols , ConnectionBaseDelegate
{

public:

    EthernetConnection* ethernetConn = nullptr;
    
protected:

    void recvDataCallback(byte cmd, byte *payload, uint16_t length, bool isTimeout,void* net);

    void sendData(byte cmd, byte *payload, uint16_t length, bool isTimeout,void* net);

public:
    ConnectionManager();

    ~ConnectionManager();

    static ConnectionManager* getInstance();

    bool init();

    void loop();

    void dataParse(std::string& data,ConnectionBase* connection);

    void dataParse(uint8_t* data, size_t length,ConnectionBase* connection);

};

#endif //__HFT_CONNECTION_MANAGER_H__