#include "AppDataList.h"
#include "HOUZZkitTester/app/AppBase.h"
#include "HOUZZkitTester/app/APP00001_Clock/AppClock.h"

uint8_t AppDataList::findUnusedId()
{
    uint8_t id = 1;
    while (_appDataMap[id]) {
        id++;
        if (id == 255) {
            return 0;
        }
    }
    return id;
}

bool AppDataList::encode(SDTData::DataSourceType type)
{
    this->pushUint8((uint8_t)_appDataList.size());
    for (int i = 0; i<_appDataList.size(); i++) {
        AppDataBase* appData = _appDataList.at(i);
        this->pushUint16(appData->appId());
        appData->enBegin();
        appData->encode(type);
        uint8_t *buffer = nullptr;
        uint32_t size = 0;
        appData->enEnd(&buffer, &size);
        this->pushBuffer16(buffer, size);
        free(buffer);
    }
    return true;
}

bool AppDataList::decode(SDTData::DataSourceType type)
{
    uint8_t appCount = this->popUint8();
    for (int i = 0; i<appCount; i++) {
        uint16_t appId = this->popUint16();
        AppDataBase* appData = AppFactory::getInstance().createInitAppData(appId);
        const uint8_t *buffer = nullptr;
        uint16_t size = 0;
        this->popBuffer16(&buffer, &size);
        appData->deBegin(buffer, size);
        appData->decode(SDTData::DataSourceType::File);
        this->_appDataList.pushBack(appData);
        this->_appDataMap[appData->id] = appData;
    }
    return true;
}

void AppDataList::setDefault()
{
    dot2d::AppDataClock* appDataClock = dot2d::AppDataClock::create();
    this->addNewAppData(appDataClock);
    this->saveData();
}

AppDataList* AppDataList::createWithDefaultFile()
{
    return AppDataList::createWithPath(FILE_APP_LIST);
}

void AppDataList::removeAppDataListFile()
{
    this->removeAllAttachment();
    dot2d::FileUtils::getInstance()->removeFile(FILE_APP_LIST);
}

void AppDataList::saveData()
{
    this->writeToPath(FILE_APP_LIST);
}

void AppDataList::removeAllAttachment()
{
    for (int i = 0; i<_appDataList.size(); i++) {
        AppDataBase* appData = _appDataList.at(i);
        appData->attachmentRemove();
    }
}

void AppDataList::clear()
{
    removeAllAttachment();
    _appDataList.clear();
    _appDataMap.clear();
}

AppDataBase* AppDataList::getAppDataWithIndex(uint8_t index)
{
    if (index >= _appDataList.size())
    {
        return nullptr;
    }
    return _appDataList.at(index);
}

AppDataBase* AppDataList::getAppDataWithId(uint8_t id)
{
    return _appDataMap[id];
}

AppDataBase* AppDataList::getAppDataWithAlias(const String& alias)
{
    for (int i = 0; i<_appDataList.size(); i++) {
        AppDataBase* appData = _appDataList.at(i);
        if (appData->appAlias.equals(alias))
        {
            return appData;
        }
    }
    return nullptr;
}

bool AppDataList::addNewAppData(AppDataBase* appData)
{
    uint8_t unusedId = findUnusedId();
    if (unusedId == 0) {
        return false;
    }
    appData->id = unusedId;
    _appDataList.pushBack(appData);
    _appDataMap[unusedId] = appData;
    return true;
}

bool AppDataList::removeAppData(uint8_t id)
{
    AppDataBase* appData = _appDataMap.at(id);
    if (!appData)
    {
        return false;
    }
    appData->attachmentRemove();
    _appDataList.eraseObject(appData);
    _appDataMap.erase(id);
    return true;
}

bool AppDataList::reorderAppData(uint8_t fromIndex,uint8_t toIndex)
{
    AppDataBase* appData = this->getAppDataWithIndex(fromIndex);
    appData->retain();
    if (!appData || toIndex>_appDataList.size())
    {
        appData->release();
        return false;
    }
    _appDataList.erase(fromIndex);
    _appDataList.insert(toIndex,appData);
    appData->release();
    return true;
}

uint8_t AppDataList::getAppDataListSize()
{
    return _appDataList.size();
}
