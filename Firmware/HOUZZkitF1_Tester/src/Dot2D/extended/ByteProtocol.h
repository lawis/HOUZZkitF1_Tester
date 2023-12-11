//
//  ByteProtocol.hpp
//  HOUZZkitTesterSimulator
//
//  Created by Lawis on 2021/7/11.
//

#ifndef ByteProtocol_h
#define ByteProtocol_h

#include <stdlib.h>
#include <stdint.h>
#include <string>
#include <vector>


class ByteProtocol
{
private:
    
    const uint8_t* _decodeBuffer = nullptr;
    
    uint32_t _decodeBytePointer = 0;
    
    uint32_t _decodeByteSize = 0;
    
    bool _decodeError = false;
    
    uint8_t _decodeByte = 0;
    
    uint8_t _decodeBitPointer = 7;

    
    std::vector<uint8_t> _encodeVector;
    
    uint8_t _encodeByte = 0;
    
    uint8_t _encodeBitPointer = 7;
    
    
private:
    
    void popBuffer(const uint8_t **buffer, uint32_t size);
    
    void setBit(uint8_t* b,uint8_t w);
    
    void clrBit(uint8_t* b,uint8_t w);
    
    void setBit(uint8_t* b,uint8_t w,bool v);
    
    bool getBit(uint8_t b,uint8_t w);
    
public:
    
    ByteProtocol();
    
    ~ByteProtocol();
    
    ByteProtocol& enBegin();
    
    ByteProtocol& enByteBegin();
    
    ByteProtocol& pushBit(bool v);
    
    ByteProtocol& pushByte();
    
    ByteProtocol& pushUint8(uint8_t v);
    
    ByteProtocol& pushUint16(uint16_t v);

    ByteProtocol& pushUint24(uint32_t v);
    
    ByteProtocol& pushUint32(uint32_t v);
    
    ByteProtocol& pushInt8(int8_t v);
    
    ByteProtocol& pushInt16(int16_t v);
    
    ByteProtocol& pushInt32(int32_t v);
    
    ByteProtocol& pushString8(const std::string& v);
    
    ByteProtocol& pushString16(const std::string& v);
    
    ByteProtocol& pushBuffer8(uint8_t *buffer, uint8_t size);
    
    ByteProtocol& pushBuffer16(uint8_t *buffer, uint16_t size);
    
    ByteProtocol& pushBuffer32(uint8_t *buffer, uint32_t size);

    ByteProtocol& pushProtocol(ByteProtocol& proto);
    
    void enEnd(uint8_t** buffer,uint32_t* size);


    uint8_t getUint8(uint32_t index);

    uint32_t getEncodeBufferSize();

    
    void deBegin(const uint8_t* buffer,uint32_t size);
    
    uint8_t popUint8(uint8_t dv = 0);
    
    uint16_t popUint16(uint16_t dv = 0);
    
    uint32_t popUint32(uint32_t dv = 0);
    
    int8_t popInt8(int8_t dv = 0);
    
    int16_t popInt16(int16_t dv = 0);
    
    int32_t popInt32(int32_t dv = 0);
    
    std::string popString8(const std::string& dv = "");
    
    std::string popString16(const std::string& dv = "");
    
    void popBuffer8(const uint8_t **buffer, uint8_t *size, const uint8_t *db = nullptr, uint8_t ds = 0);
    
    void popBuffer16(const uint8_t **buffer, uint16_t *size, const uint8_t *db = nullptr, uint8_t ds = 0);
    
    void popBuffer32(const uint8_t **buffer, uint32_t *size, const uint8_t *db = nullptr, uint8_t ds = 0);
    
    void popByte();
    
    bool popBit(bool dv = false);
    
    bool decodeError(){return _decodeError;}
    
    uint32_t remainingDeBufferSize();
    
};

#endif /* ByteProtocol_h */
