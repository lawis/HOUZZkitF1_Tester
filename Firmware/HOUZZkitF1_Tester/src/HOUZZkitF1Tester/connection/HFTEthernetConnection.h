#ifndef __HFT_ETHERNET_CONNECTION_H__
#define __HFT_ETHERNET_CONNECTION_H__

#include <Arduino.h>
#include "HFTConnectionBase.h"
#include <map>

#define VERIFICATION_SERVER_HOST "192.168.13.110"
#define VERIFICATION_SERVER_PORT 80

class EthernetConnection : public ConnectionBase
{
private:
    typedef enum
    {
        EC_NO_SHIELD = 255, // for compatibility with WiFi Shield library
        EC_IDLE_STATUS = 0,
        EC_NO_SSID_AVAIL = 1,
        EC_SCAN_COMPLETED = 2,
        EC_CONNECTED = 3,
        EC_CONNECT_FAILED = 4,
        EC_CONNECTION_LOST = 5,
        EC_DISCONNECTED = 6
    } ec_status_t;

    ec_status_t _status = EC_IDLE_STATUS;

    String httpRequest(const String &host, const uint16_t port, const String &api, int *errCode,const String& body);

public:
    EthernetConnection(ConnectionBaseDelegate *delegate);

    ~EthernetConnection();

    bool init();

    void loop();

    bool connected();
    
};

#endif //__HFT_ETHERNET_CONNECTION_H__