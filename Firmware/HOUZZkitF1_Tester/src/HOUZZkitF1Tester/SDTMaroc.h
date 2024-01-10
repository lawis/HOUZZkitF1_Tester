#ifndef __SDT_MACROS_H__
#define __SDT_MACROS_H__

#include <Arduino.h>
#include "dtMacros.h"


INLINE uint32_t GetUint32(const uint8_t* buffer)
{
    return (uint32_t(buffer[0]) << 24) 
    + (uint32_t(buffer[1]) << 16)
    + (uint32_t(buffer[2]) << 8)
    + uint32_t(buffer[3]); 
}

INLINE void SetUint32(byte* buffer,uint32_t i)
{
    buffer[0] = i>>24 & 0xFF;
    buffer[1] = i>>16 & 0xFF; 
    buffer[2] = i>>8 & 0xFF;
    buffer[3] = i & 0xFF; 
}
                
INLINE uint16_t GetUint16(const uint8_t* buffer)
{
    return (uint16_t(buffer[0]) << 8) 
    + uint16_t(buffer[1]);
}

INLINE void SetUint16(byte* buffer,uint16_t i)
{
    buffer[0] = i>>8 & 0xFF;
    buffer[1] = i & 0xFF; 
}

INLINE uint8_t GetUint8(const byte* buffer)
{
    return buffer[0];
}

INLINE void SetUint8(byte* buffer,uint8_t i)
{
    buffer[0] = i;
}

INLINE int8_t GetInt8(const byte* buffer)
{
    return (int8_t)buffer[0];
}

INLINE void SetInt8(byte* buffer,int8_t i)
{
    buffer[0] = i;
}

INLINE bool GetBit(byte b,uint8_t w)
{
    return (b >> w) & 0x01;
}

INLINE void Setbit(byte* b,uint8_t w)
{
    (*b)|=(1<<w);
}

INLINE void Clrbit(byte* b,uint8_t w)
{
    (*b)&=~(1<<w);
}

INLINE void Setbit(byte* b,uint8_t w,bool v)
{
    if (v){Setbit(b,w);}
    else {Clrbit(b,w);}
}

INLINE uint8_t GetNumberDigits(uint32_t n)
{
    if(n==0)
    {
        return 1;
    }
    return log10(n)+1;
}

String NumberFormat(int32_t n,uint8_t l);

String split(String origin,String symbol,int index);




#endif //__SDT_MACROS_H__
