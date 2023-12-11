#ifndef __SDT_DATA_H__
#define __SDT_DATA_H__

#include "Dot2D/dot2d.h"
#include "Dot2D/extended/ByteProtocol.h"
#include "HOUZZkitTester/SDTMaroc.h"
#include <vector>

class SDTData : public dot2d::Ref , public ByteProtocol
{
public:
    
    enum class DataSourceType
    {
        File = 0,
        Net = 1,
    };
    
protected:

    //通过数据流初始化
    bool initWithBuff(const uint8_t* buffer,uint32_t size,DataSourceType type);
    
    //通过flash文件路径初始化
    bool initWithPath(std::string path);

    //通过Preferences key值初始化
    bool initWithKey(const std::string& key);
    
    //完成数据准备后需要进行的其他初始化工作
    virtual bool init();

    //序列化编码
    virtual bool encode(DataSourceType type);

    //反序列化解码
    virtual bool decode(DataSourceType type);

    //当反序列化失败时生成一份默认数据
    virtual void setDefault();
    
   

public:

    SDTData();

    ~SDTData();

    STATIC_CREATE(SDTData);

    static SDTData* createWithBuff(const uint8_t* buffer,uint32_t size,DataSourceType type);

    static SDTData* createWithPath(const std::string& path);
    
    static SDTData* createWithKey(const std::string& key);

    static bool remvoeWithKey(const std::string& key);
    
    bool writeToPath(std::string path);

    bool writeToKey(std::string key);

};

template<typename DATA_NAME>
class SDTDataTemplate : public SDTData
{
public:

    static DATA_NAME* create();

    static DATA_NAME* createWithBuff(const byte* buffer,uint32_t size);

    static DATA_NAME* createWithPath(std::string path);

    static DATA_NAME* createWithKey(std::string key);
    
};



#include "SDTData.tpp"

#endif // __SDT_DATA_H__
