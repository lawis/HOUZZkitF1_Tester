#include "HFTEthernetConnection.h"
#include <Ethernet2.h>
#include <EthernetHTTPClient.h>
#include <ArduinoJson.h>


byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xFD};
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
    if (Ethernet.begin(mac) == 0)
    {
        delay(100);
        Serial.println("Failed to configure Ethernet using DHCP");
        Ethernet.begin(mac, defaultIp, defaultDns);
    }
    delay(100);
    Serial.print("My IP address: ");
    Serial.println(Ethernet.localIP());
    Serial.println("connecting...");

    String result = this->httpRequest(VERIFICATION_SERVER_HOST,VERIFICATION_SERVER_PORT,"/api/json/sys/v2/CheckToken",nullptr,"{\"token\":\"97e2c1838d96d253d65fb75872e5945b73ebd4a268b487086fe6898260384a03\"}");

    // res = "{\"code\":500,\"msg\":\"EOF\",\"data\":{\"token\":\"\",\"wan\":\"\",\"lan\":\"\",\"sn\":\"\",\"check_code\":\"\"},\"trace_id\":\"trace_id_1703504184850\"}";
    result.trim();
    Serial.println(result);
    uint32_t size = result.length();
    DynamicJsonDocument json(size+256);
    DeserializationError error = deserializeJson(json, result);
    Serial.println(error.c_str());
    if (error) {
        return false; 
    }
    if (json["msg"].as<String>() == "EOF")
    {
        _status = EC_CONNECTED;
    }
    Serial.println(json["msg"].as<String>());
    Serial.println("success");
    return true;
}
unsigned long lastConnectionTime = 0;
const unsigned long postingInterval = 10L * 1000L;

void EthernetConnection::loop()
{
    // Serial.print("666 ")

    // if (client.available())
    // {
    //     char c = client.read();
    //     Serial.print(c);
    // }

    // if the server's disconnected, stop the client:
    // if (!client.connected())
    // {
    //     Serial.println();
    //     Serial.println("disconnecting.");
    //     client.stop();

    //     delay(1000);
    //     // do nothing forevermore:
    //     // while (true);
    // }

    vTaskDelay(10);
}
String EthernetConnection::httpRequest(const String &host, const uint16_t port, const String &api, int *errCode,const String& body)
{
    String res;
    EthernetClient client;
    // if (client.connect(host.c_str(), port))
    // {
    //     Serial.println("connected...");
    //     Serial.println(body);
    //     // send the HTTP PUT request:
    //     // client.println("GET /search?q=arduino HTTP/1.1");
    //     client.printf("POST %s HTTP/1.1\n", api.c_str());
    //     client.printf("Host: %s\n", host.c_str());
    //     client.println("Content-Type: application/json");
    //     client.println("Accept: */*");
    //     client.println("Connection: close");
    //     client.printf("\r\n");
    //     client.println(body);
    //     client.println();

    //     // note the time that the connection was made:
    //     delay(100);
    //     while (client.available())
    //     {
    //         char thisChar = client.read();
    //         res += thisChar;
    //     }
    //     client.stop();
    // }
    // else
    // {
    //     // if you couldn't make a connection:
    //     Serial.println("connection failed");
    // }

    EthernetHTTPClient httpClient;
    httpClient.begin(client, host+api);
    // // for (auto iter = headerMap.begin(); iter != headerMap.end(); ++iter)
    // // {
    // //     httpClient.addHeader(iter->first.c_str(), iter->second.c_str());
    // // }
    int httpCode = 0;
    httpCode = httpClient.POST("{}");
    *errCode = httpCode;
    // if (httpCode > 0)
    // {
    //     if (httpCode == HTTP_CODE_OK)
    //     {
    //         res = httpClient.getString();
    //     }
    // }
    // httpClient.end();

    return res;
}


bool EthernetConnection::connected()
{
    return _status == EC_CONNECTED;
}