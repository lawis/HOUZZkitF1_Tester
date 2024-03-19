#ifndef __HFT_CONNECTION_MANAGER_H__
#define __HFT_CONNECTION_MANAGER_H__

#include "HFTEthernetConnection.h"
#include "HFTSerialConnection.h"
#include "SDTProtocol.h"

class ConnectionManager : public Protocols , ConnectionBaseDelegate
{

private:

    bool _device_connected = false;

public:

    EthernetConnection* ethernetConn = nullptr;
    SerialConnection* serialConn = nullptr;
    SerialConnection* serialRS485 = nullptr;
    String firmwareVersion = "";
    String eno0Mac = "";
    String eno1Mac = "";
    String verify = "";
    String snCode = "";
    String burnStartTimestamp = "";
    String burnEndTimestamp = "";
    
protected:

    void recvDataCallback(byte cmd, byte *payload, uint16_t length, bool isTimeout,void* net);

    void sendData(byte cmd, byte *payload, uint16_t length, bool isTimeout,void* net);

public:
    ConnectionManager();

    ~ConnectionManager();

    static ConnectionManager* getInstance();

    bool init();

    bool initRS485();

    void loop();

    void reset();

    void dataParse(const String& data,ConnectionBase* connection);

    void dataParse(uint8_t* data, size_t length,ConnectionBase* connection);

    void dataParse(uint16_t pid, const String &data);

    bool deviceConnected(){return _device_connected;};
    

};

#endif //__HFT_CONNECTION_MANAGER_H__