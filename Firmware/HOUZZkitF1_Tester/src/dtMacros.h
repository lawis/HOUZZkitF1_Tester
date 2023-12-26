#ifndef __DT_MACROS_H__
#define __DT_MACROS_H__

#include <stdint.h>

#define INLINE inline __attribute__((always_inline))


const int32_t DT_INVALID_INDEX = -1;

/// @name namespace dot2d
/// @{
#ifdef __cplusplus
    #define NS_DT_BEGIN                     namespace dot2d {
    #define NS_DT_END                       }
    #define USING_NS_DT                     using namespace dot2d
    #define NS_DT                           ::dot2d
#else
    #define NS_DT_BEGIN 
    #define NS_DT_END 
    #define USING_NS_DT 
    #define NS_DT
#endif 
//  end of namespace group
/// @}


#ifndef MIN
#define MIN(x,y) (((x) > (y)) ? (y) : (x))
#endif  // MIN

#ifndef MAX
#define MAX(x,y) (((x) < (y)) ? (y) : (x))
#endif  // MAX


#define DT_SAFE_DELETE(p)           do { delete (p); (p) = nullptr; } while(0)
#define DT_SAFE_DELETE_ARRAY(p)     do { if(p) { delete[] (p); (p) = nullptr; } } while(0)
#define DT_SAFE_FREE(p)             do { if(p) { free(p); (p) = nullptr; } } while(0)
#define DT_SAFE_RELEASE(p)          do { if(p) { (p)->release(); } } while(0)
#define DT_SAFE_RELEASE_NULL(p)     do { if(p) { (p)->release(); (p) = nullptr; } } while(0)
#define DT_SAFE_RETAIN(p)           do { if(p) { (p)->retain(); } } while(0)
#define DT_BREAK_IF(cond)           if(cond) break


/// @name dot2d debug
/// @{

//TEMP LOG METHOD

NS_DT_BEGIN
void log(const char * format, ...);
NS_DT_END


#define LOG(format, ...)    Serial.printf(format"\n", ##__VA_ARGS__)
#define LOG_INT(n)          Serial.printf(#n " = %d\n",n)
#define LOG_INT_STR(n,s)    Serial.printf(#n " = %d ----- %s\n",n,s.c_str())
#define LOG_FLOAT(n)        Serial.printf(#n " = %f\n",n)
#define LOG_FLOAT_STR(n,s)  Serial.printf(#n " = %f ----- %s\n",n,s.c_str())
#define LOG_BOOL(n)         Serial.printf(#n " = %s\n",n?"True":"False")
#define LOG_BOOL_STR(n,s)   Serial.printf(#n " = %s ----- %s\n",n?"True":"False",s.c_str())
#define LOG_STR(n)          Serial.printf(#n " = %s\n",n.c_str())
#define LOG_STR_STR(n,s)    Serial.printf(#n " = %s ----- %s\n",n.c_str(),s.c_str())



#ifndef FLT_EPSILON
#define FLT_EPSILON     1.192092896e-07F
#endif // FLT_EPSILON

#define DT_REPEAT_FOREVER (UINT_MAX -1)

#define kRepeatForever DT_REPEAT_FOREVER

#define DT_FORMAT_PRINTF(formatPos, argPos) __attribute__((__format__(printf, formatPos, argPos)))




#define STATIC_CREATE(type)         static type* create()                       \
                                    {                                           \
                                        type* ret = new type();                 \
                                        if (ret && ret->init())                 \
                                        {                                       \
                                            ret->autorelease();                 \
                                            return ret;                         \
                                        }                                       \
                                        DT_SAFE_DELETE(ret);                    \
                                        return nullptr;                         \
                                    }





#endif // __DT_MACROS_H__
