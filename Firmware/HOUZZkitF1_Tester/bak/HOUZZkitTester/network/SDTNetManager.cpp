#include "SDTNetManager.h"
#include "HOUZZkitTester/SDTConfig.h"
#include "HOUZZkitTester/SDTSystem.h"
#include "HOUZZkitTester/SDTMaroc.h"
#include "Ticker.h"

const char *server_certificate = "-----BEGIN CERTIFICATE-----\n" \
"MIIFFjCCAv6gAwIBAgIRAJErCErPDBinU/bWLiWnX1owDQYJKoZIhvcNAQELBQAw\n" \
"TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh\n" \
"cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMjAwOTA0MDAwMDAw\n" \
"WhcNMjUwOTE1MTYwMDAwWjAyMQswCQYDVQQGEwJVUzEWMBQGA1UEChMNTGV0J3Mg\n" \
"RW5jcnlwdDELMAkGA1UEAxMCUjMwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEK\n" \
"AoIBAQC7AhUozPaglNMPEuyNVZLD+ILxmaZ6QoinXSaqtSu5xUyxr45r+XXIo9cP\n" \
"R5QUVTVXjJ6oojkZ9YI8QqlObvU7wy7bjcCwXPNZOOftz2nwWgsbvsCUJCWH+jdx\n" \
"sxPnHKzhm+/b5DtFUkWWqcFTzjTIUu61ru2P3mBw4qVUq7ZtDpelQDRrK9O8Zutm\n" \
"NHz6a4uPVymZ+DAXXbpyb/uBxa3Shlg9F8fnCbvxK/eG3MHacV3URuPMrSXBiLxg\n" \
"Z3Vms/EY96Jc5lP/Ooi2R6X/ExjqmAl3P51T+c8B5fWmcBcUr2Ok/5mzk53cU6cG\n" \
"/kiFHaFpriV1uxPMUgP17VGhi9sVAgMBAAGjggEIMIIBBDAOBgNVHQ8BAf8EBAMC\n" \
"AYYwHQYDVR0lBBYwFAYIKwYBBQUHAwIGCCsGAQUFBwMBMBIGA1UdEwEB/wQIMAYB\n" \
"Af8CAQAwHQYDVR0OBBYEFBQusxe3WFbLrlAJQOYfr52LFMLGMB8GA1UdIwQYMBaA\n" \
"FHm0WeZ7tuXkAXOACIjIGlj26ZtuMDIGCCsGAQUFBwEBBCYwJDAiBggrBgEFBQcw\n" \
"AoYWaHR0cDovL3gxLmkubGVuY3Iub3JnLzAnBgNVHR8EIDAeMBygGqAYhhZodHRw\n" \
"Oi8veDEuYy5sZW5jci5vcmcvMCIGA1UdIAQbMBkwCAYGZ4EMAQIBMA0GCysGAQQB\n" \
"gt8TAQEBMA0GCSqGSIb3DQEBCwUAA4ICAQCFyk5HPqP3hUSFvNVneLKYY611TR6W\n" \
"PTNlclQtgaDqw+34IL9fzLdwALduO/ZelN7kIJ+m74uyA+eitRY8kc607TkC53wl\n" \
"ikfmZW4/RvTZ8M6UK+5UzhK8jCdLuMGYL6KvzXGRSgi3yLgjewQtCPkIVz6D2QQz\n" \
"CkcheAmCJ8MqyJu5zlzyZMjAvnnAT45tRAxekrsu94sQ4egdRCnbWSDtY7kh+BIm\n" \
"lJNXoB1lBMEKIq4QDUOXoRgffuDghje1WrG9ML+Hbisq/yFOGwXD9RiX8F6sw6W4\n" \
"avAuvDszue5L3sz85K+EC4Y/wFVDNvZo4TYXao6Z0f+lQKc0t8DQYzk1OXVu8rp2\n" \
"yJMC6alLbBfODALZvYH7n7do1AZls4I9d1P4jnkDrQoxB3UqQ9hVl3LEKQ73xF1O\n" \
"yK5GhDDX8oVfGKF5u+decIsH4YaTw7mP3GFxJSqv3+0lUFJoi5Lc5da149p90Ids\n" \
"hCExroL1+7mryIkXPeFM5TgO9r0rvZaBFOvV2z0gp35Z0+L4WPlbuEjN/lxPFin+\n" \
"HlUjr8gRsI3qfJOQFy/9rKIJR0Y/8Omwt/8oTWgy1mdeHmmjk7j1nYsvC9JSQ6Zv\n" \
"MldlTTKB3zhThV1+XWYp6rjd5JW1zbVWEkLNxE7GJThEUG3szgBVGP7pSWTUTsqX\n" \
"nLRbwHOoq7hHwg==\n" \
"-----END CERTIFICATE-----";

static NetManager* s_NetManager = nullptr;  

QueueHandle_t g_xQueue = nullptr;

xSemaphoreHandle g_xMutex = nullptr;

Ticker mDNSTicker;

WebRequestTask* WebRequestTask::create(const String& url,bool ssl,dot2d::Ref* target,RequestCallback callback,uint8_t times)
{
    if (!target || !callback)
    {
        return nullptr; 
    }
    
    WebRequestTask* ret = new WebRequestTask();
    if (ret)
    {
        ret->url = String(url);
        ret->ssl = ssl;
        ret->_target = target;
        ret->_callback = callback;
        ret->retryTimes = times;
        ret->autorelease();
        return ret;
    }
    DT_SAFE_DELETE(ret);
	return nullptr; 
}

void delayBleEnd_C()
{
    NetManager::getInstance()->bleEnd();
}

void NetManager::delayBleEnd(float seconds)
{
    if (delayBleEndTicker != nullptr)
    {
        delete delayBleEndTicker;
    }
    delayBleEndTicker = new Ticker();
    delayBleEndTicker->once(seconds,delayBleEnd_C);
}


void NetManager::recvDataCallback(byte cmd, byte *payload, uint16_t length, bool isTimeout,void* net)
{
    if(_protoCallbackMap.find(cmd) != _protoCallbackMap.end())
    {
        if((this->*_protoCallbackMap[cmd])(payload,length,isTimeout,(NetBase*)net))
        {
#if PRINT_DEBUG_INFO
            LOG("CMD: %d execute succeed ! ",cmd);
#endif
        }
#if PRINT_DEBUG_INFO
        else
        {

            LOG("CMD: %d execute failed ! ",cmd);
        }
#endif
    }
#if PRINT_DEBUG_INFO
    else
    {

        LOG("CMD: %d found  failed ！ ",cmd);
    }
#endif
}


void NetManager::sendData(byte cmd, byte *payload, uint16_t length, bool isTimeout,void* net)
{
    //net即为网络指针，谁收到的信息谁发
#if PRINT_DEBUG_INFO

    Serial.printf("--------------------------- \nSend Length : %d , \nData Array : ",length);
    for (int16_t i = 0; i < length; i++)
    {
        Serial.printf("0x%02X",payload[i]);
        if (i != length-1)
        {   
            Serial.print(",");
        }
        if (i % 8 == 7)
        {
            Serial.println();
        }
    }
    Serial.println();

#endif
    ((NetBase*)net)->sendData(payload,length);
}    

NetManager::NetManager()
{

    PROTOCOL_REGISTER(1);
    PROTOCOL_REGISTER(2);
    PROTOCOL_REGISTER(5);
    PROTOCOL_REGISTER(6);
    PROTOCOL_REGISTER(7);
    PROTOCOL_REGISTER(11);
    PROTOCOL_REGISTER(12);
    PROTOCOL_REGISTER(13);
    PROTOCOL_REGISTER(14);
    PROTOCOL_REGISTER(15);
    PROTOCOL_REGISTER(16);
    PROTOCOL_REGISTER(17);
    PROTOCOL_REGISTER(18);
    PROTOCOL_REGISTER(21);
    PROTOCOL_REGISTER(22);
    PROTOCOL_REGISTER(23);
    PROTOCOL_REGISTER(24);
    PROTOCOL_REGISTER(25);
    PROTOCOL_REGISTER(26);
    PROTOCOL_REGISTER(27);
}

NetManager::~NetManager()
{
    delete _bleManager;
    _bleManager = nullptr;
    delete _wifiManager;
    _wifiManager = nullptr;
    delete espClient;
    espClient = nullptr;
    delete mqttClient;
    mqttClient = nullptr;
}

NetManager* NetManager::getInstance()
{
    if (s_NetManager == nullptr)
    {
        s_NetManager = new NetManager();
    }
    return s_NetManager;
}

void NetManager::initMutexAndQueue()
{
    g_xMutex = xSemaphoreCreateRecursiveMutex ();
    g_xQueue = xQueueCreate(5, sizeof(xMESSAGE));
}

bool NetManager::shutdownNetwork(bool shutdownWifi)
{
    if(_wifiManager)
    {
        _wifiManager->stop(shutdownWifi);
    }
    return true;
}

bool NetManager::initWifiSystem()
{
    if (_wifiManager != nullptr)
    {
        return false;
    }
    _wifiManager = new WifiManager(this);
    if(!_wifiManager->init())
    {
        delete _wifiManager;
        _wifiManager = nullptr;
        return false;
    }
    return true;
}

bool NetManager::initBleSystem()
{
    if (_bleManager != nullptr)
    {
        return false;
    }
    _bleManager = new BleManager(this);
    if(!_bleManager->init())
    {
        delete _bleManager;
        _bleManager = nullptr;
        return false;
    }
    _bleManager->start();
    return true;
}

void mqttCallback(char *topic, byte *payload, unsigned int length)
{
    DynamicJsonDocument json(length+32);
    DeserializationError error = deserializeJson(json, String((char *)payload));
    if (error) {
        LOG("ERROR!!! mqttCallback Parse Failed! ");
    }
    uint16_t opCode = json["op"].as<uint16_t>();
    String argument = json["arg"].as<String>();
    uint16_t periodIndex = argument.indexOf("。");
    if (periodIndex>0)
    {
       argument = argument.substring(0,periodIndex);
    }
    LOG_INT(opCode);
    LOG_STR(argument);
    String errResult;
    switch (opCode)
    {
    case 1:
    {
        if (!AppManager::getInstance()->mqtfPlayAppByName(argument))
        {
            errResult = "Can not find app";
        }
        break;
    }
    case 2:
    {
        if (!AppManager::getInstance()->mqtfPlayAppByList())
        {
            errResult = "Play app list error";
        }
        break;
    }
    default:
        break;
    }
    LOG("Mqtt Command Executed %s",errResult.c_str());
}

bool NetManager::initMqttSystem()
{
    SDTSystem* sdtSystem = SDTSystem::getInstance();
    if (sdtSystem->mqttServerIp().length() < 5  
    || sdtSystem->mqttServerPort() == 0)
    {
        return false;
    }
    if (_mqttConnectedPort == sdtSystem->mqttServerPort()
    && _mqttConnectedServer.equals(sdtSystem->mqttServerIp().c_str()))
    {
        return true;
    }
    if (mqttClient)
    {
        mqttClient->disconnect();
        delete mqttClient;
    }
    if (espClient)
    {
        delete espClient;
    }
    espClient = new WiFiClient();
    mqttClient = new PubSubClient(*espClient);
    _mqttConnectedServer = sdtSystem->mqttServerIp();
    _mqttConnectedPort = sdtSystem->mqttServerPort();
    mqttClient->setServer(_mqttConnectedServer.c_str(), _mqttConnectedPort);
    mqttClient->setCallback(mqttCallback);
    String clientId = "";
	clientId += String(random(0x1111111,0xFFFFFFF), HEX);
	if (!mqttClient->connect(clientId.c_str()))
	{
		return false;
	}
    mqttClient->subscribe("PixForceMsg/#");
    mqttClient->publish("PixForce", "connected");
    return true;
}



bool NetManager::bootWifi(const String& ssid,const String& password)
{
    return _wifiManager->bootWifi(ssid,password);
}

bool NetManager::bootWebSocket()
{
    return _wifiManager->bootWebSocket();
}

void NetManager::loop()
{
    this->executeHeartBeat();
    this->executeReqeustTask();
    this->mqttLoop();
}

void NetManager::mqttLoop()
{
    if(_mqttReinit)
    {
        this->initMqttSystem();
        _mqttReinit = false;
    }
    if(mqttClient)
    {
        mqttClient->loop();
    }
}

void NetManager::dataParse(std::string& data,NetBase* net)
{
    this->pParse(data,net);
}

void NetManager::dataParse(uint8_t* data, size_t length,NetBase* net)
{
    this->pParse(data,length,net);
}


void NetManager::bleEnd()
{

    if (delayBleEndTicker)
    {
        delete delayBleEndTicker;
        delayBleEndTicker = nullptr;
    } 
    // this->shutdownNetwork(false);
    // delay(100);
    SDTSystem::getInstance()->ESPReset();
    //BUG 以下代码无法做到彻底释放蓝牙占用内存，通过重启设备解决
    // _bleManager->stop();
    // _bleManager->free();
    // delete _bleManager;
    // _bleManager = nullptr;
           
}

WebRequestTask* NetManager::pushRequestTask(const String& url,bool ssl,dot2d::Ref* target,WebRequestTask::RequestCallback callback,uint8_t retryTimes)
{
    for (int16_t i = 0; i < requestTaskList.size(); i++)
    {
        WebRequestTask* task = requestTaskList.at(i);
        if(task->url == url)
        {
            return task;
        }
    }
    WebRequestTask* task = WebRequestTask::create(url,ssl,target,callback,retryTimes);
    requestTaskList.pushBack(task);
    return task;
}

void NetManager::dirtyRequestTask(dot2d::Ref* target)
{
    for (int16_t i = 0; i < requestTaskList.size(); i++)
    {
        WebRequestTask* task = requestTaskList.at(i);
        if(task->getTarget() == target)
        {
            task->dirtyTask();
            return;
        }
    }
}

void heartBeatTask()
{
  if(WiFi.status() != WL_CONNECTED)
  {
    return;
  }
  NetManager::getInstance()->heartBeat();
  // MDNS.end();
  // MDNS.begin("HOUZZkitTester_plus");
  // MDNS.addService("_http", "_tcp", 9936);
  // MDNS.addServiceTxt("_http", "_tcp", "Hello", "World");
}

void NetManager::startHeartBeat()
{
    heartBeatTask();
    mDNSTicker.attach(HEART_BEAT_INTERVAL,heartBeatTask);
}



void NetManager::executeHeartBeat()
{
    if (!_heartBeatTag){return;}
#if PRINT_DEBUG_INFO
    LOG("executeHeartBeat Begin!!!");
    uint32_t ms = millis();
#endif
    String url(HEARTBEAT_URL);
    std::map<String,String> payloadMap;
    std::map<String,String> headerMap;
    payloadMap["dvid"] = SDTSystem::getInstance()->UDID();
    payloadMap["socketAddress"] = (WiFi.localIP().toString() + String("/ws"));
    headerMap["Authorization"] = SDTSystem::getInstance()->token();
    int responseCode = -1;
    String res = NetManager::getInstance()->wifiManager()->httpsRequest(url,&responseCode,payloadMap,headerMap,server_certificate);
    if (responseCode == 404 && responseCode == 401)
    {
        _verifySuccessfully = false;
#if PRINT_DEBUG_INFO
        LOG("HearBeat verify failed");
#endif
    }
    _heartBeatTag =false;
#if PRINT_DEBUG_INFO
    LOG_STR(res);
    LOG("executeHeartBeat End!!! Takes: %d ms",millis()-ms);
#endif
}

void NetManager::executeReqeustTask()
{
    if (!_verifySuccessfully)
    {
        requestTaskList.clear();
        return;
    }
    while (requestTaskList.size() > 0)
    {
        WebRequestTask* task = requestTaskList.at(0);
        
#if PRINT_DEBUG_INFO
        LOG("RequesetTask Begin. Url:%s",task->url.c_str());
        uint32_t ms = millis();
#endif
        task->retryTimes--;
        String res;
        int errorCode = -1;
        bool parse = false;
        if (task->ssl)
        {
            res = this->_wifiManager->httpsRequest(task->url,&errorCode,task->payloadMap,task->headerMap);
        }else{
            res = this->_wifiManager->httpRequest(task->url,&errorCode,task->payloadMap,task->headerMap);
        }
        NET_MUTEX_LOCK();
        if (errorCode == 200 && res.length() > 0)
        {
            if (!task->isDirty())
            {
                parse = task->parse(res);
            }else
            {
                parse = true;
            }
        }else
        {        
#if PRINT_DEBUG_INFO
            Serial.printf("Error Request Error Code: %d !!! URL:%s",errorCode,task->url.c_str());
#endif
        }
        NET_MUTEX_UNLOCK();

        if (parse || task->retryTimes<=0)
        {
#if PRINT_DEBUG_INFO
            LOG(">>>>>>Request Task Stop. %s  Takes: %d ms",parse?"SUCCESS":"FAILED",millis()-ms);
#endif
            requestTaskList.eraseObject(task);
        }
    }
}



bool NetManager::protocolCallback_1(byte *payload, uint16_t length, bool isTimeout,NetBase* net)
{
    pBegin(1).pushUint24(FIRMWARE_VERSION).pushString8(SDTSystem::getInstance()->UDID().c_str());
    pushString8(SDTSystem::getInstance()->deviceModel().c_str());
    pSend(net);
    return true;
}

bool NetManager::
protocolCallback_2(byte *payload, uint16_t length, bool isTimeout,NetBase* net)
{
    deBegin(payload,length);
    String token = popString8().c_str();
    String ssid = popString8().c_str();
    String pwd  = popString8().c_str();

    LOG_STR(token);
    LOG_STR(ssid);
    LOG_STR(pwd);

    // SDTSystem::getInstance()->setUUID(uuid);
    SDTSystem::getInstance()->setUninitDevice(false);
    SDTSystem::getInstance()->setToken(token);
    SDTSystem::getInstance()->setWifiSSID(ssid);
    SDTSystem::getInstance()->setWifiPWD(pwd);
    SDTSystem::getInstance()->saveUserData();
    SDTSystem::getInstance()->resetState();

    SDTSystem::SystemState ss = SDTSystem::getInstance()->getSystemState();
    switch (ss)
    {
        case SDTSystem::SystemState::INIT:
        {
            pBegin(2).pushUint8(1);
            pSend(net);
        }
            break;
        case SDTSystem::SystemState::NORMAL:
        {
            pBegin(2).pushUint8(0).pushString8((_wifiManager->localIP() + String("/ws")).c_str());
            pSend(net);
            this->delayBleEnd(0.5);
        }
            break;
        default:
            break;
    }
    return true;
}

bool NetManager::protocolCallback_5(byte *payload, uint16_t length, bool isTimeout,NetBase* net)
{
    pBegin(5);
    bool res = AppManager::getInstance()->wtfUpdateFirmware(payload,length,*this);
    pSend(net);
    return res;
}

bool NetManager::protocolCallback_6(byte *payload, uint16_t length, bool isTimeout,NetBase* net)
{
    SDTSystem *_sdtSystem = SDTSystem::getInstance();
    pBegin(6);
    pushString8(_sdtSystem->deviceName().c_str());
    pushUint8(_sdtSystem->theme());
    pushUint8(_sdtSystem->brightness());
    pushUint8(_sdtSystem->volume());
    enByteBegin();
    pushBit(_sdtSystem->autoBrighten());
    pushByte();
    pushString8(_sdtSystem->mqttServerIp().c_str());
    pushUint16(_sdtSystem->mqttServerPort());
    pSend(net);
    return true;
}

bool NetManager::protocolCallback_7(byte *payload, uint16_t length, bool isTimeout,NetBase* net)
{
    SDTSystem *_sdtSystem = SDTSystem::getInstance();
    deBegin(payload,length);
    _sdtSystem->setDeviceName(popString8().c_str());
    _sdtSystem->setTheme(popUint8());
    _sdtSystem->setBrightness(popUint8());
    _sdtSystem->setVolume(popUint8());
    popByte();
    _sdtSystem->setAutoBrighten(popBit());
    _sdtSystem->setMqttServerIp(popString8().c_str());
    _sdtSystem->setMqttServerPort(popUint16());
    pBegin(7);
    pushUint8(0);
    pSend(net);
    NET_MUTEX_LOCK();
    _sdtSystem->saveConfigData();
    _sdtSystem->saveDeviceData();
    this->mqttReinit();
    NET_MUTEX_UNLOCK();
    return true;
} 

bool NetManager::protocolCallback_11(byte *payload, uint16_t length, bool isTimeout,NetBase* net)
{
    pBegin(11).pushUint24(FIRMWARE_VERSION).pushString8(SDTSystem::getInstance()->UDID().c_str());
    pushString8(SDTSystem::getInstance()->deviceModel().c_str());
    pushString8(SDTSystem::getInstance()->deviceName().c_str());
    this->pSend(net);
    return true;
}

bool NetManager::protocolCallback_12(byte *payload, uint16_t length, bool isTimeout,NetBase* net)
{
    pBegin(12).pushUint8(0);
    pSend(net);
    return true;
}

bool NetManager::protocolCallback_13(byte *payload, uint16_t length, bool isTimeout,NetBase* net)
{
    pBegin(13).pushUint8(0);
    pSend(net);
    return true;
}

bool NetManager::protocolCallback_14(byte *payload, uint16_t length, bool isTimeout,NetBase* net)
{
    return AppManager::getInstance()->wtfShowPreviewApp(payload,length,*this);
}

bool NetManager::protocolCallback_15(byte *payload, uint16_t length, bool isTimeout,NetBase* net)
{
    return AppManager::getInstance()->wtfExitPreviewApp(payload,length,*this);
}

bool NetManager::protocolCallback_16(byte *payload, uint16_t length, bool isTimeout,NetBase* net)
{
    pBegin(16);
    bool res = AppManager::getInstance()->wtfPlayAppById(payload,length,*this);
    pSend(net);
    return res;
}

bool NetManager::protocolCallback_17(byte *payload, uint16_t length, bool isTimeout,NetBase* net)
{
    pBegin(17);
    bool res = AppManager::getInstance()->wtfPlayPreviousPlayableApp(payload,length,*this);
    pSend(net);
    return res;
}


bool NetManager::protocolCallback_18(byte *payload, uint16_t length, bool isTimeout,NetBase* net)
{
    pBegin(18);
    bool res = AppManager::getInstance()->wtfPlayNextPlayableApp(payload,length,*this);
    pSend(net);
    return res;
}


bool NetManager::protocolCallback_21(byte *payload, uint16_t length, bool isTimeout,NetBase* net)
{
    pBegin(21);
    bool res = AppManager::getInstance()->wtfAddNewApp(payload,length,*this);
    pSend(net);
    return res;
}

bool NetManager::protocolCallback_22(byte *payload, uint16_t length, bool isTimeout,NetBase* net)
{
    pBegin(22);
    bool res = AppManager::getInstance()->wtfDeleteApp(payload,length,*this);
    pSend(net);
    return res;
}

bool NetManager::protocolCallback_23(byte *payload, uint16_t length, bool isTimeout,NetBase* net)
{
    pBegin(23);
    bool res = AppManager::getInstance()->wtfReorderAppList(payload,length,*this);
    pSend(net);
    return res;
}

bool NetManager::protocolCallback_24(byte *payload, uint16_t length, bool isTimeout,NetBase* net)
{
    pBegin(24);
    bool res = AppManager::getInstance()->getAppListData(*this);
    if (res)
    {
        pSend(net);    
    }
    return res;
}

bool NetManager::protocolCallback_25(byte *payload, uint16_t length, bool isTimeout,NetBase* net)
{
    pBegin(25);
    bool res = AppManager::getInstance()->getAppData(payload[0],*this);
    if (res)
    {
        pSend(net);    
    }
    return res;
}

bool NetManager::protocolCallback_26(byte *payload, uint16_t length, bool isTimeout,NetBase* net)
{
    pBegin(26);
    bool res = AppManager::getInstance()->setAppData(payload[0],payload,length,*this);
    pSend(net);
    if (res)
    {
        SDTSystem::getInstance()->saveConfigData();
    }
    return res;
}

bool NetManager::protocolCallback_27(byte *payload, uint16_t length, bool isTimeout,NetBase* net)
{
    return false;
}