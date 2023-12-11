#include "SDTWiFiManager.h"
#include "../SDTConfig.h"
#include "SDTNetManager.h"
#include "HOUZZkitTester/SDTSystem.h"


#include <ESPmDNS.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>



void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len)
{
  if(type == WS_EVT_CONNECT){
#if PRINT_DEBUG_INFO
    Serial.println("Websocket client connection received");
#endif
    NetManager::getInstance()->wifiManager()->setClient(client,false);
  } else if(type == WS_EVT_DISCONNECT){
#if PRINT_DEBUG_INFO
    Serial.println("Websocket client disconnected");
#endif
    NetManager::getInstance()->wifiManager()->setClient(nullptr,true);
  } else if(type == WS_EVT_DATA){
    NetManager::getInstance()->wifiManager()->receivedData(data,len);
  }
}     

WifiManager::WifiManager(NetBaseDelegate *delegate)
:NetBase(delegate)
{
    
}

WifiManager::~WifiManager()
{
  delete _server;
  delete _ws;
}

bool WifiManager::init()
{
  WiFi.setSleep(false);
  _server = new AsyncWebServer(80);
  _ws = new AsyncWebSocket("/ws"); 
  return true;
}

bool WifiManager::bootWifi(const String& ssid,const String& password)
{
  Serial.print("------Connecting Wifi");
  WiFi.begin(ssid.c_str(), password.c_str());

  for (int8_t i = 0; i < MAX_WIFI_CONNECT_TIMES; i++)
  {
      if(WiFi.status() != WL_CONNECTED)
      {
        Serial.print(".");
        if (i == MAX_WIFI_CONNECT_TIMES-1)
        {
          Serial.println();
          Serial.println("---ERROR!!!---Wifi connected timeout");
          WiFi.disconnect();
          return false;
        }
        delay(500);
      }else
      {
        break;
      }
  }
  
  Serial.println();
  Serial.printf("------Connect successed , Local IP:%s\n",WiFi.localIP().toString().c_str());
  
  return true;
}

bool WifiManager::bootWebSocket()
{
  if(WiFi.status() == WL_CONNECTED)
  {
    _ws->onEvent(onWsEvent);
    _server->addHandler(_ws);
    _server->begin();
  }
  return true;
}

void WifiManager::loop()
{

}

void WifiManager::stop(bool shutdownWifi)
{
    this->setClient(nullptr,false);
    if (_server)
    {
      _server->end();
    }
    if (shutdownWifi)
    {
      WiFi.disconnect();  
    }
}

void WifiManager::sendData(std::string& data)
{
  if (_client)
  {
    _ws->text(_client->id(),data.c_str());
  }
}

void WifiManager::sendData(uint8_t* data, size_t length) 
{ 
  if (_client)
  {
    _ws->binary(_client->id(),data,length);
  }
}

void WifiManager::receivedData(uint8_t* data, size_t length)
{
  if(_delegate)
  {
      _delegate->dataParse(data,length,this);
  }
}

void WifiManager::setClient(AsyncWebSocketClient* client,bool setOnly)
{
  if (!setOnly && _client)
  {
    _client->close();
  }
  _client = client;
}

String WifiManager::localIP()
{
  return WiFi.localIP().toString();
}

String WifiManager::payloadCreator(const std::map<String,String>& payloadMap)
{
  StaticJsonDocument<200> doc;
  for(auto iter = payloadMap.begin(); iter != payloadMap.end(); ++iter){
    doc[iter->first] = iter->second;
  }
  String payload;
  serializeJson(doc,payload);
  return payload;
}


String WifiManager::httpRequest(const String &url, int *errCode,const std::map<String,String>& payloadMap,const std::map<String,String>& headerMap)
{
  String res;
  if(WiFi.status() != WL_CONNECTED)
  {
    *errCode = -1;
    return res;
  }
  HTTPClient httpClient;
  httpClient.begin(url);
  for(auto iter = headerMap.begin(); iter != headerMap.end(); ++iter){
    httpClient.addHeader(iter->first.c_str(),iter->second.c_str());
  }
  int httpCode = 0;
  if (payloadMap.size()>0)
  {
    httpCode = httpClient.POST(payloadCreator(payloadMap));
  }else
  {
    httpCode = httpClient.GET();
  }
  *errCode = httpCode;
  if (httpCode > 0)
  {
    if (httpCode == HTTP_CODE_OK)
    {
      res = httpClient.getString();
    }
  }
  httpClient.end();
  return res;
}

String WifiManager::httpsRequest(const String &url, int *errCode,const std::map<String,String>& payloadMap,const std::map<String,String>& headerMap,const char* certificate)
{
  String res;
  if(WiFi.status() != WL_CONNECTED)
  {
    *errCode = -1;
    return res;
  }
  String urlTemp = url;
  urlTemp.replace("https://", "");
  int splitIndex = urlTemp.indexOf('/');
  const String &httpsServer = urlTemp.substring(0, splitIndex);
  const String &api = urlTemp.substring(splitIndex);
  WiFiClientSecure sslClient;
  if (certificate)
  {
    sslClient.setCACert(certificate);
  }else{
    sslClient.setInsecure();
  }
  sslClient.connect(httpsServer.c_str(), 443);
  HTTPClient httpClient;
  httpClient.begin(sslClient, url);
  for(auto iter = headerMap.begin(); iter != headerMap.end(); ++iter){
    httpClient.addHeader(iter->first.c_str(),iter->second.c_str());
  }
  int httpCode = 0;
  if (payloadMap.size()>0)
  {
    httpCode = httpClient.POST(payloadCreator(payloadMap));
  }else
  {
    httpCode = httpClient.GET();
  }
  *errCode = httpCode;
  if (httpCode > 0)
  {
    if (httpCode == HTTP_CODE_OK)
    {
      res = httpClient.getString();
    }
  }
  httpClient.end();
  return res;
}