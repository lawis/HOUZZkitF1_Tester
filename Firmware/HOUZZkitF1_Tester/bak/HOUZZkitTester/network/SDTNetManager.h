#ifndef __SDT_CONNECTING_H__
#define __SDT_CONNECTING_H__


#include <Arduino.h>
#include "Dot2D/dot2d.h"
#include "SDTNetBase.h"
#include "SDTBLEManager.h"
#include "SDTWiFiManager.h"
#include "SDTProtocol.h"
#include <functional>
#include "Ticker.h"
#include <WiFiClient.h>
#include <PubSubClient.h>
#include "freertos/semphr.h"
#include <map>

extern const char *server_certificate;

extern QueueHandle_t g_xQueue;
extern xSemaphoreHandle g_xMutex;

class AppManager;

typedef bool (AppManager::*xMessageCallback)(const uint8_t* buffer,const uint32_t size);

typedef struct xMESSAGE
{
    xMessageCallback handle;
    uint8_t *buffer;
    uint32_t size;
    xMESSAGE()
    :handle(nullptr)
    ,buffer(nullptr)
    ,size(0)
    {

    };

    xMESSAGE(xMessageCallback h)
    :handle(h)
    ,buffer(nullptr)
    ,size(0)
    {

    };

    xMESSAGE(xMessageCallback h,const uint8_t *b,const uint32_t s)
    :handle(h)
    ,buffer(nullptr)
    ,size(s)
    {
        if(b)
        {
            buffer = (uint8_t *)malloc(size);
            memcpy(buffer,b,size);
        }
    };

    void freeBuffer()
    {
        if(buffer)
        {
            free(buffer);
            buffer = nullptr;
        }
    };
}xMESSAGE;

#define NET_MUTEX_LOCK()    do {} while (xSemaphoreTake(g_xMutex, portMAX_DELAY) != pdPASS)
#define NET_MUTEX_UNLOCK()  xSemaphoreGive(g_xMutex)

#define NET_QUEUE_SEND(msg)     xQueueSend(g_xQueue,msg,portMAX_DELAY);
#define NET_QUEUE_RECEIVE(msg)  (pdPASS != xQueueReceive( g_xQueue, msg, 0))

#define PROTOCOL_CALLBACK(n)    protocolCallback_##n(byte*,uint16_t,bool,NetBase*)
#define PROTOCOL_REGISTER(n)    _protoCallbackMap[n] = &NetManager::protocolCallback_##n;


class WebRequestTask : public dot2d::Ref
{
public:

    typedef bool(dot2d::Ref::*RequestCallback)(const String&);

private:

    dot2d::Ref* _target = nullptr;

    RequestCallback _callback = nullptr;

    bool _dirty = false;

public:

    String url;

    std::map<String,String> payloadMap;
    
    std::map<String,String> headerMap;

    uint8_t retryTimes = 0;

    bool ssl = false;

    WebRequestTask(){};

    ~WebRequestTask(){};

    static WebRequestTask* create(const String& url,bool ssl,dot2d::Ref* target,RequestCallback callback,uint8_t times = 1);

    bool parse(const String& res){return (_target->*_callback)(res);}

    dot2d::Ref* getTarget(){return _target;}

    bool isDirty(){return _dirty;}

    void dirtyTask(){_dirty = true;}
};



class NetManager : public Protocols , NetBaseDelegate 
{
private:

    BleManager* _bleManager = nullptr;

    WifiManager* _wifiManager = nullptr;

    Ticker* delayBleEndTicker = nullptr;

    WiFiClient* espClient = nullptr;
    
    PubSubClient* mqttClient = nullptr;
    
    dot2d::Vector<WebRequestTask *> requestTaskList;

    bool _heartBeatTag = false;

    bool _mqttReinit = false;

    String _mqttConnectedServer;

    uint16_t _mqttConnectedPort;

    bool _verifySuccessfully = true;

private:

    typedef bool (NetManager::*protocolCallback)(byte*,uint16_t,bool,NetBase*);

    std::map<uint8_t,protocolCallback> _protoCallbackMap;

    bool PROTOCOL_CALLBACK(1);  
    bool PROTOCOL_CALLBACK(2);
    bool PROTOCOL_CALLBACK(5);
    bool PROTOCOL_CALLBACK(6);
    bool PROTOCOL_CALLBACK(7);
    bool PROTOCOL_CALLBACK(11);
    bool PROTOCOL_CALLBACK(12);
    bool PROTOCOL_CALLBACK(13);
    bool PROTOCOL_CALLBACK(14);
    bool PROTOCOL_CALLBACK(15);
    bool PROTOCOL_CALLBACK(16);
    bool PROTOCOL_CALLBACK(17);
    bool PROTOCOL_CALLBACK(18);
    bool PROTOCOL_CALLBACK(21);
    bool PROTOCOL_CALLBACK(22);
    bool PROTOCOL_CALLBACK(23);
    bool PROTOCOL_CALLBACK(24);
    bool PROTOCOL_CALLBACK(25);
    bool PROTOCOL_CALLBACK(26);
    bool PROTOCOL_CALLBACK(27);

private:

    void delayBleEnd(float seconds);

protected:

    void recvDataCallback(byte cmd, byte *payload, uint16_t length, bool isTimeout,void* net);

    void sendData(byte cmd, byte *payload, uint16_t length, bool isTimeout,void* net);

public:
    NetManager();

    ~NetManager();

    static NetManager* getInstance();

    void initMutexAndQueue();

    WifiManager* wifiManager(){return _wifiManager;}

    bool shutdownNetwork(bool shutdownWifi);

    bool initWifiSystem();

    bool initBleSystem();

    bool initMqttSystem();

    bool bootWifi(const String& ssid,const String& password);

    bool bootWebSocket();

    void loop();

    void mqttReinit(){_mqttReinit = true;}

    void mqttLoop();

    void dataParse(std::string& data,NetBase* net);

    void dataParse(uint8_t* data, size_t length,NetBase* net);

    void bleEnd();

    WebRequestTask* pushRequestTask(const String& url,bool ssl,dot2d::Ref* target,WebRequestTask::RequestCallback callback,uint8_t retryTimes = 5);

    void dirtyRequestTask(dot2d::Ref* target);

    void startHeartBeat();

    void executeHeartBeat();

    void executeReqeustTask();

    void heartBeat(){_heartBeatTag = true;}
};


#endif //__SDT_CONNECTING_H__