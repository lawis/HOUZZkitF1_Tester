#include "HFTConnectionManager.h"

static ConnectionManager *s_connectionManager = nullptr;

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
    return true;
}

void ConnectionManager::loop()
{
    ethernetConn->loop();
}

void ConnectionManager::recvDataCallback(byte cmd, byte *payload, uint16_t length, bool isTimeout,void* baseNet)
{

}

void ConnectionManager::sendData(byte cmd, byte *payload, uint16_t length, bool isTimeout,void* baseNet)
{

}

void ConnectionManager::dataParse(std::string &data, ConnectionBase *connection)
{

}

void ConnectionManager::dataParse(uint8_t *data, size_t length, ConnectionBase *connection)
{

}