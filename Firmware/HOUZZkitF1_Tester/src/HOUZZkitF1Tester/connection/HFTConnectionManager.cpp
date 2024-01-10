#include "HFTConnectionManager.h"
#include "HOUZZkitF1Tester/SDTMaroc.h"

static ConnectionManager *s_connectionManager = nullptr;

// HardwareSerial SerialPort(1);

ConnectionManager::ConnectionManager()
{
}

ConnectionManager::~ConnectionManager()
{
}

ConnectionManager *ConnectionManager::getInstance()
{
    if (s_connectionManager == nullptr)
    {
        s_connectionManager = new ConnectionManager();
        s_connectionManager->init();
    }
    return s_connectionManager;
}

bool ConnectionManager::init()
{
    ethernetConn = new EthernetConnection(this);
    ethernetConn->init();
    serialConn = new SerialConnection(this);

    // SerialPort.begin(115200, SERIAL_8N1, 21, 14);
    serialConn->init(&Serial);
    return true;
}

bool ConnectionManager::initRS485()
{
    serialRS485 = new SerialConnection(this);
    serialRS485->init(&Serial1);
    return true;
}

void ConnectionManager::loop()
{
    ethernetConn->loop();
    serialConn->loop();
}

void ConnectionManager::reset()
{
    _device_connected = false;
}

void ConnectionManager::recvDataCallback(byte cmd, byte *payload, uint16_t length, bool isTimeout, void *baseNet)
{
}

void ConnectionManager::sendData(byte cmd, byte *payload, uint16_t length, bool isTimeout, void *baseNet)
{
}

void ConnectionManager::dataParse(const String &data, ConnectionBase *connection)
{
}

void ConnectionManager::dataParse(uint8_t *data, size_t length, ConnectionBase *connection)
{
}

void ConnectionManager::dataParse(uint16_t pid, const String &data)
{
    // Serial.printf("pid:%d length:%ld data:%s\n", pid,data.length(), data.c_str());

    switch (pid)
    {
    case 1:
    {
        if (data == "ping")
        {
            serialConn->sendString(1, "pong");
        }
        break;
    }
    case 2:
    {

        String res = split(data,",",0);
        if (res == "ok")
        {
            _device_connected = true;
            firmwareVersion = split(data,",",1);
            eno0Mac = split(data,",",2);
            eno1Mac = split(data,",",3);
            verify = split(data,",",4);
            snCode = split(data,",",5);
        }
        break;
    }
    default:
        break;
    }
}