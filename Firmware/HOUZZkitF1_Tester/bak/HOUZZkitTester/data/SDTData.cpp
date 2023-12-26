#include "SDTData.h"
#include "HOUZZkitTester/SDTConfig.h"
#ifndef TARGET_OS_MAC
#include <Preferences.h>
#endif


bool SDTData::initWithBuff(const uint8_t* buffer,uint32_t size,DataSourceType type)
{
    if (buffer == nullptr || size == 0)
    {
        this->setDefault();
    }else
    {
        deBegin(buffer, size);
        if (!decode(type)) 
        {
            this->setDefault();
        }
    }
    return true;
}

bool SDTData::initWithPath(std::string path)
{
    uint32_t size = 0;
    uint8_t* buffer = dot2d::FileUtils::getInstance()->readDataFromPath(path,&size);
    bool res = initWithBuff(buffer, size, DataSourceType::File);
    free(buffer);
    return res;
}


bool SDTData::initWithKey(const std::string& key)
{
#ifndef TARGET_OS_MAC
    Preferences prefs;
    prefs.begin(PREFERENCES_NAMESPACE);
    uint32_t key_size = prefs.getBytesLength(key.c_str());
    uint8_t *buffer = (uint8_t*)malloc(key_size);
    uint32_t size = prefs.getBytes(key.c_str(),buffer,key_size);
    bool res = this->initWithBuff(buffer, size, DataSourceType::File);
    free(buffer);
    prefs.end();
    return res;
#else
    return false;
#endif
}

bool SDTData::init()
{
    return true;
}

bool SDTData::encode(DataSourceType type)
{   
    return false;
}

bool SDTData::decode(DataSourceType type)
{
    return false;
}

void SDTData::setDefault()
{

}


SDTData::SDTData()
{
    
}

SDTData::~SDTData()
{

}

SDTData* SDTData::createWithBuff(const byte* buffer,uint32_t size,DataSourceType type)
{
    SDTData* ret = new SDTData();                 
    if (ret && ret->initWithBuff(buffer, size, type))
    {                                       
        ret->autorelease();                 
        return ret;                         
    }                                       
    DT_SAFE_DELETE(ret);                    
    return nullptr;
}

SDTData* SDTData::createWithPath(const std::string& path)
{
    SDTData* ret = new SDTData();
    if (ret && ret->initWithPath(path))
    {
        ret->autorelease();
        return ret;
    }
    DT_SAFE_DELETE(ret);
    return nullptr;
}

SDTData* SDTData::createWithKey(const std::string& key)
{
    SDTData* ret = new SDTData();
    if (ret && ret->initWithKey(key))
    {
        ret->autorelease();
        return ret;
    }
    DT_SAFE_DELETE(ret);
    return nullptr;
}

bool SDTData::remvoeWithKey(const std::string& key)
{
#ifndef TARGET_OS_MAC
    Preferences prefs;
    if (!prefs.begin(PREFERENCES_NAMESPACE))
    {
        LOG("(Data Remove From Preferences) Preferences Begin error!!! Key:%s",key.c_str());
        return false;
    }
    if (prefs.remove(key.c_str()) == 0)
    {
        LOG("(Data Remove From Preferences) Put bytes error!!! Key:%s",key.c_str());
        prefs.end();
        return false;
    }
    prefs.end();
    return true;
#else
    return false;
#endif
}



bool SDTData::writeToPath(std::string path)
{
    enBegin();
    if(!encode(DataSourceType::File))
    {
        LOG("(Data Write To Path) Encode error!!! PATH:%s",path.c_str());
        return false;
    }
    uint32_t size = 0;
    uint8_t* buffer = nullptr;
    enEnd(&buffer, &size);
    if (!dot2d::FileUtils::getInstance()->writeDataToPath(buffer,size,path))
    {
        LOG("(Data Write To Path) Write error!!! PATH:%s",path.c_str());
        free(buffer);
        return false;
    }
    delete buffer;
    return true;
}

bool SDTData::writeToKey(std::string key)
{
#ifndef TARGET_OS_MAC
    enBegin();
    if(!encode(DataSourceType::File))
    {
        LOG("(Data Write To Preferences) Encode error!!! Key:%s",key.c_str());
        return false;
    }
    uint32_t size = 0;
    uint8_t* buffer = nullptr;
    enEnd(&buffer, &size);
    Preferences prefs;
    if (!prefs.begin(PREFERENCES_NAMESPACE))
    {
        LOG("(Data Write To Preferences) Preferences Begin error!!! Key:%s",key.c_str());
        free(buffer);
        return false;
    }
    if (prefs.putBytes(key.c_str(),buffer,size) == 0)
    {
        LOG("(Data Write To Preferences) Put bytes error!!! Key:%s",key.c_str());
        prefs.end();
        free(buffer);
        return false;
    }
    prefs.end();
    free(buffer);
    return true;
#else
    return false;
#endif
}
