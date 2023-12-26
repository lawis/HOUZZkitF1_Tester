#include "SDTProtocol.h"
#include "HOUZZkitF1Tester/SDTMaroc.h"
void Protocols::invertUint16(uint16_t *dBuf, uint16_t *srcBuf)
{
    uint16_t tmp[4] = {0};
    for (uint8_t i = 0; i < 16; i++)
    {
        if (srcBuf[0] & (1 << i))
        tmp[0] |= 1 << (15 - i);
    }
    dBuf[0] = tmp[0];
}

uint16_t Protocols::CRC16_MODBUS(uint8_t *data, uint16_t datalen)
{
    uint16_t wCRCin = 0xFFFF;
    uint16_t wCPoly = 0x8005;
    invertUint16(&wCPoly, &wCPoly);
    while (datalen--)
    {
        wCRCin ^= *(data++);
        for (uint8_t i = 0; i < 8; i++)
        {
        if (wCRCin & 0x01)
            wCRCin = (wCRCin >> 1) ^ wCPoly;
        else
            wCRCin = wCRCin >> 1;
        }
    }
    uint16_t wCRCinTemp = wCRCin;
    return (wCRCin >> 8 & 0x00FF) | (wCRCinTemp << 8 & 0xFF00);
}

void Protocols::printData(const uint8_t* data,uint16_t size)
{
    Serial.printf("--------------------------- \nLength : %d , Data Array : ",size);
    for (int16_t i = 0; i < size; i++)
    {
        Serial.printf("0x%02X",data[i]);
        if (i != size-1)
        {   
            Serial.print(",");
        }
        if (i % 8 == 7)
        {
            Serial.println();
        }
    }
    Serial.println();
}

Protocols::Protocols()
{

    
}

Protocols::~Protocols()
{

}

Protocols& Protocols::pBegin(byte cmd)
{
    enBegin();
    pushUint8(0);
    pushUint8(0);
    pushUint8(cmd);
    return *this;
}

Protocols& Protocols::pushColor(byte r,byte g,byte b)
{
    pushUint8(r);
    pushUint8(g);
    pushUint8(b);
    return *this;
}

void Protocols::pParse(const std::string& s,void* baseNet)
{
    const uint8_t* data = (const uint8_t*)s.c_str();
    const uint32_t length = s.length();
    this->pParse(data,length,baseNet);
}

void Protocols::pParse(const uint8_t* data, const uint32_t length,void* baseNet)
{
    uint16_t pLength = 0;
    const uint8_t* dataPointer = nullptr;
    if (_parseLength == 0)
    {
        if ( length < 5 )
        {
            LOG("ERROR!!! Net Data Incomplete");
            return;//协议数据不完整抛弃
        }
        pLength = GetUint16(data);
        if (pLength + 2 > length)
        {
            if (_parseBuffer)
            {
                free(_parseBuffer);
            }
            _parseBuffer = (uint8_t *)malloc(pLength + 2);
            memcpy(_parseBuffer,data,length);
            _parseLength = length;
            return;//等待后续数据
        }
        dataPointer = data;
    }else
    {
        pLength = GetUint16(_parseBuffer);
        if (pLength + 2 < _parseLength + length)
        {
            free(_parseBuffer);
            LOG("ERROR!!! Net Data Create Failed");
            return;
        }else
        {
            memcpy(_parseBuffer+_parseLength,data,length);
            _parseLength += length;
            if (pLength + 2 > _parseLength)
            {
                //继续等待数据
                // LOG("pLength : %d _parseLength:%d length:%d",pLength,_parseLength,length);
                return;
            }
        }
        dataPointer = _parseBuffer;
    }

    if(this->CRC16_MODBUS((uint8_t *)dataPointer+2,pLength) != 0)
    {
        LOG("ERROR!!! CRC FAILED!!!");
        return; 
    }
#if PRINT_DEBUG_INFO
    printData(data,length);
#endif
    uint8_t cmd = dataPointer[2];
    this->recvDataCallback(cmd,(byte *)(dataPointer+3),pLength-3,false,baseNet);
    if (_parseBuffer)
    {
        free(_parseBuffer);
        _parseBuffer = nullptr;
    }
    _parseLength = 0;
}

void Protocols::pSend(void* baseNet,bool checkTimeout)
{   
    uint8_t* buffer = nullptr;
    uint32_t size = 0;
    this->pushUint16(0);
    this->enEnd(&buffer,&size);
    uint16_t crc = this->CRC16_MODBUS(buffer+2, size-4);
    buffer[size-2] = (crc >> 8) & 0xFF;
    buffer[size-1] = (crc >> 0) & 0xFF;
    buffer[0] = ((size - 2) >> 8) & 0xFF;
    buffer[1] = ((size - 2) >> 0) & 0xFF;
    this->sendData(buffer[2],buffer,size,checkTimeout,baseNet);
    free(buffer);
}