#ifndef __SDT_APP_LIST_DATA_H__
#define __SDT_APP_LIST_DATA_H__


#include "HOUZZkitTester/data/SDTData.h"
#include <map>

class AppDataBase;

class AppDataList : public SDTDataTemplate<AppDataList>
{
    
private:

    dot2d::Vector<AppDataBase*> _appDataList;
    
    std::map<uint8_t,AppDataBase*> _appDataMap;
    
private:
    
    uint8_t findUnusedId();

    void removeAllAttachment();

protected:

    bool encode(SDTData::DataSourceType type) override;

    bool decode(SDTData::DataSourceType type) override;

    void setDefault() override;

public:
    
    static AppDataList* createWithDefaultFile();

    void removeAppDataListFile();
    
    void saveData();
    
    void clear();

    AppDataBase* getAppDataWithIndex(uint8_t index);
    
    AppDataBase* getAppDataWithId(uint8_t id);

    AppDataBase* getAppDataWithAlias(const String& alias);
    
    bool addNewAppData(AppDataBase* appData);

    bool removeAppData(uint8_t id);

    bool reorderAppData(uint8_t fromIndex,uint8_t toIndex);
    
    uint8_t getAppDataListSize();

};

#endif //__SDT_APP_LIST_DATA_H__
