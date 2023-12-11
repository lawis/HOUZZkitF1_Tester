#ifndef __SDT_WIFIMANAGER_H__
#define __SDT_WIFIMANAGER_H__

#include <Arduino.h>
#include "Dot2D/dot2d.h"
#include "SDTNetBase.h"

#include "WiFi.h"
#include "ESPAsyncWebServer.h"

class WifiManager : public NetBase
{
private:

    AsyncWebServer *_server = nullptr;

    AsyncWebSocket *_ws = nullptr;

    uint32_t _wsClientId = 0;

    AsyncWebSocketClient* _client = nullptr;

private:

    String payloadCreator(const std::map<String,String>& payloadMap);

public:

    WifiManager(NetBaseDelegate *delegate);

    ~WifiManager();

    bool init();

    void loop();

    bool bootWifi(const String& ssid,const String& password);

    bool bootWebSocket();

    void startHeartBeat();
    
    void stop(bool shutdownWifi);

    void sendData(std::string& data);

    void sendData(uint8_t* data, size_t length);

    void receivedData(uint8_t* data, size_t length);

    String localIP();

    void setClient(AsyncWebSocketClient* client,bool setOnly);

    String httpRequest(const String &url, int *errCode,const std::map<String,String>& payloadMap,const std::map<String,String>& headerMap);

    String httpsRequest(const String &url, int *errCode,const std::map<String,String>& payloadMap,const std::map<String,String>& headerMap,const char* certificate = nullptr);

};





#endif //__SDT_WIFIMANAGER_H__