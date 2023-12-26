#include "SDTData.h"

template<typename DATA_NAME>
DATA_NAME* SDTDataTemplate<DATA_NAME>::create()
{
    DATA_NAME* ret = new DATA_NAME();                 
    if (ret && ret->init())                 
    {                                       
        ret->autorelease();                 
        return ret;                         
    }                                       
    DT_SAFE_DELETE(ret);                    
    return nullptr;
}

//template<typename DATA_NAME>
//DATA_NAME* SDTDataTemplate<DATA_NAME>::createWithBuff(const byte* buffer,uint32_t size)
//{
//    DATA_NAME* ret = new DATA_NAME();
//    if (ret && ret->initWithBuff(buffer,size))
//    {
//        ret->autorelease();
//        return ret;
//    }
//    DT_SAFE_DELETE(ret);
//    return nullptr;
//}

template<typename DATA_NAME>
DATA_NAME* SDTDataTemplate<DATA_NAME>::createWithPath(std::string path)
{
    DATA_NAME* ret = new DATA_NAME();                 
    if (ret && ret->initWithPath(path))                 
    {                                       
        ret->autorelease();                 
        return ret;                         
    }                                       
    DT_SAFE_DELETE(ret);                    
    return nullptr;
}

template<typename DATA_NAME>
DATA_NAME* SDTDataTemplate<DATA_NAME>::createWithKey(std::string key)
{
    DATA_NAME* ret = new DATA_NAME();                 
    if (ret && ret->initWithKey(key))                 
    {                                       
        ret->autorelease();                 
        return ret;                         
    }                                       
    DT_SAFE_DELETE(ret);                    
    return nullptr;
}
