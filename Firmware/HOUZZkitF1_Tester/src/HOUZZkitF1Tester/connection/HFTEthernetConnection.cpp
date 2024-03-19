#include "HFTEthernetConnection.h"
#include <Ethernet2.h>
#include <EthernetHTTPClient.h>
#include <ArduinoJson.h>

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xFF};
IPAddress defaultIp(192, 168, 13, 177);
IPAddress defaultDns(114, 114, 114, 114);
// EthernetClient client;

EthernetConnection::EthernetConnection(ConnectionBaseDelegate *delegate) : ConnectionBase(delegate)
{
}

EthernetConnection::~EthernetConnection()
{
}

bool EthernetConnection::init()
{
  Ethernet.init(45);
  int beginRes = Ethernet.begin(mac);
  if (beginRes == 0)
  {
    delay(100);
    Serial.println("Failed to configure Ethernet using DHCP");
    _status = EC_DHCP_FAILED;
    Ethernet.begin(mac, defaultIp, defaultDns);
  }
  else if (beginRes == 1)
  {
    _status = EC_CONNECTED;
  }
  delay(100);
  Serial.printf("My IP address: %s\n", Ethernet.localIP().toString().c_str());
  return true;
}
unsigned long lastConnectionTime = 0;
const unsigned long postingInterval = 10L * 1000L;

void EthernetConnection::loop()
{
  // vTaskDelay(10);
}

bool EthernetConnection::serverConnected()
{
  int responseCode = -1;
  std::map<String, String> payloadMap;
  std::map<String, String> headerMap;
  payloadMap["token"] = String("b0613a4c72a0d7053ffa42bedb98ddea67d14f9a9b3b408edac8b101c21bc75c");
  String result = this->httpRequest(String("http://") + VERIFICATION_SERVER_HOST + String("/api/json/sys/v2/CheckToken"), &responseCode, payloadMap, headerMap);

  uint32_t size = result.length();
  DynamicJsonDocument json(size + 256);
  DeserializationError error = deserializeJson(json, result);

  if (error)
  {
    return false;
  }
  if (json["msg"].as<String>() != "success")
  {
    return false;
  }
  return true;
}

bool EthernetConnection::activateDevice(const String& firmwareVersion, const String &wanMac, const String &lanMac, const String &snCode, const String &verify,String& checkCode,const String& burnStartTimestamp,const String& burnEndTimestamp)
{
  // Serial.println("100:"+verify);
  // Serial.println("100:"+wanMac);
  // Serial.println("100:"+lanMac);
  // Serial.println("100:"+snCode);
  // Serial.println("100:"+burnInTest);
  int responseCode = -1;
  std::map<String, String> payloadMap;
  std::map<String, String> headerMap;
  payloadMap["version"] = firmwareVersion;
  payloadMap["token"] = verify;
  payloadMap["wan"] = wanMac;
  payloadMap["lan"] = lanMac;
  payloadMap["sn"] = snCode;
  payloadMap["burnStartTimestamp"] = burnStartTimestamp;
  payloadMap["burnEndTimestamp"] = burnEndTimestamp;
  String result = this->httpRequest(String("http://") + VERIFICATION_SERVER_HOST + String("/api/json/sys/v2/CheckSn"), &responseCode, payloadMap, headerMap);

  uint32_t size = result.length();
  DynamicJsonDocument json(size + 256);
  DeserializationError error = deserializeJson(json, result);


  if (error)
  {
    return false;
  }
  if (json["msg"].as<String>() != "success")
  {
    return false;
  }
  checkCode = json["data"]["check_code"].as<String>();
  return true;

}

bool EthernetConnection::ethernetReady()
{
  return _status == EC_CONNECTED;
}

String EthernetConnection::payloadCreator(const std::map<String, String> &payloadMap)
{
  StaticJsonDocument<500> doc;
  for (auto iter = payloadMap.begin(); iter != payloadMap.end(); ++iter)
  {
    doc[iter->first] = iter->second;
  }
  String payload;
  serializeJson(doc, payload);
  return payload;
}

String EthernetConnection::httpRequest(const String &url, int *errCode, const std::map<String, String> &payloadMap, const std::map<String, String> &headerMap)
{
  String res;
  EthernetClient client;
  EthernetHTTPClient httpClient;
  httpClient.begin(client,VERIFICATION_SERVER_HOST,VERIFICATION_SERVER_PORT,url);
  for (auto iter = headerMap.begin(); iter != headerMap.end(); ++iter)
  {
    httpClient.addHeader(iter->first.c_str(), iter->second.c_str());
  }
  int httpCode = 0;
  if (payloadMap.size() > 0)
  {
    httpCode = httpClient.POST(payloadCreator(payloadMap));
  }
  else
  {
    httpCode = httpClient.GET();
  }
  if (errCode)
  {
    *errCode = httpCode;
  }
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
