#ifndef __HFT_ETHERNET_CONNECTION_H__
#define __HFT_ETHERNET_CONNECTION_H__

#include <Arduino.h>
#include "HFTConnectionBase.h"
#include <map>

#define VERIFICATION_SERVER_HOST "cloud.yinbaiyuan.com"
#define VERIFICATION_SERVER_PORT 5080

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
        EC_DISCONNECTED = 6,
        EC_DHCP_FAILED = 7
    } ec_status_t;

    ec_status_t _status = EC_IDLE_STATUS;

    String payloadCreator(const std::map<String,String>& payloadMap);

    String httpRequest(const String &url, int *errCode,const std::map<String,String>& payloadMap,const std::map<String,String>& headerMap);

public:
    EthernetConnection(ConnectionBaseDelegate *delegate);

    ~EthernetConnection();

    bool init();

    void loop();

    bool serverConnected();

    bool activateDevice(const String& wanMac,const String& lanMac,const String& snCode,const String& verify,String& checkCode);

    bool ethernetReady();

    
};

#endif //__HFT_ETHERNET_CONNECTION_H__