#ifndef __SDT_PROTOCOL_H__
#define __SDT_PROTOCOL_H__

#include <Arduino.h>
#include "ByteProtocol.h"
#include <map>

class Protocols : public ByteProtocol
{
private:

    uint8_t* _parseBuffer = nullptr;

    uint16_t _parseLength = 0;

protected:

    void invertUint16(uint16_t *dBuf, uint16_t *srcBuf);

    uint16_t CRC16_MODBUS(uint8_t *data, uint16_t datalen);

    virtual void recvDataCallback(byte cmd, byte *payload, uint16_t length, bool isTimeout,void* baseNet) = 0;

    virtual void sendData(byte cmd, byte *payload, uint16_t length, bool isTimeout,void* baseNet) = 0;

    void printData(const uint8_t* data,uint16_t size);
public:

    Protocols();

    ~Protocols();

    Protocols& pBegin(byte cmd);

    Protocols& pushColor(uint8_t r,uint8_t g,uint8_t b);

    void pParse(const std::string& s,void* baseNet);

    void pParse(const uint8_t* data, const uint32_t length,void* baseNet);

    void pSend(void* baseNet,bool checkTimeout = false);

};

#endif //__SDT_PROTOCOL_H__