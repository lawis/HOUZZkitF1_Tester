#ifndef __APP_BASE_H__
#define __APP_BASE_H__

#include "Dot2D/dot2d.h"
#include "HOUZZkitTester/SDTConfig.h"
#include "HOUZZkitTester/data/SDTData.h"
#include "HOUZZkitTester/app/SDTAppManager.h"
#include "HOUZZkitTester/network/SDTNetManager.h"
#include <map>
#include <ArduinoJson.h>

class AppDataBase : public SDTData
{
public:

	enum class AppSizeType
	{
		AUTO 	= 0,
		S3208 	= 1,
		S16016 	= 2,
		S808 	= 3,
		S12012 	= 4,
		S32016 	= 5
	};

	enum class AppAlignType
	{
		TOPLEFT 		= 0,
		TOPCENTER 		= 1,
		TOPRIGHT 		= 2,
		CENTERLEFT 		= 3,
		CENTER 			= 4,
		CENTERRIGHT		= 5,
		BOTTOMLEFT 		= 6,
		BOTTOMCENTER 	= 7,
		BOTTOMRIGHT 	= 8,
	};

	enum class ScheduleType
    {
        NONE        = 0,
        SCHEDULE    = 1,
    };
    
    enum class ConfigureType
    {
        NONE = 0,
        WAIT = 1,
        COMPLETE = 2,
    };
    
protected:
    
    //无需序列化数据
    ScheduleType scheduleType;
    
    uint16_t scheduleInterval;
    
    friend class AppScheduleBase;
    
    friend class AppManager;

public:
    
    //需序列化数据
    uint8_t id;
    
	String appAlias;

    uint16_t showDuration;
    
    bool canPlay;
    
    bool openPlay;
    
    bool isActive;

	AppSizeType appSizeType;

	AppAlignType appAlignType;
    


protected:

    virtual bool subEncode(SDTData::DataSourceType type){return false;}

    virtual bool subDecode(SDTData::DataSourceType type){return false;}

    virtual void setDefault() override{};
    
    virtual bool detectActive(){return true;}

public:
    
    AppDataBase();
    
    virtual bool init() override;
    
    static uint16_t APPID();
    
    virtual uint16_t appId();

    virtual bool appAddInit();

    bool encode(SDTData::DataSourceType type) override;

    bool decode(SDTData::DataSourceType type) override;

    bool encodeSummary();
    
    virtual void attachmentRemove(){};
    
    uint32_t fileId();

};


template<typename APP_DATA,uint16_t APP_ID>
class AppDataTemplate : public AppDataBase
{
public:

    static APP_DATA* create();

    static APP_DATA* createWithBuff(const byte* buffer,uint32_t size);
    
    static uint16_t APPID();
    
    virtual uint16_t appId();

};

//---------------------------AppScheduleBase-----------------------

class AppScheduleBase : public dot2d::Node
{
protected:

    AppDataBase* appData;

    typedef bool(dot2d::Ref::*HandleRequestCallback)(const String&);

protected:

    virtual bool init();

    WebRequestTask* pushRequestTask(const String& url,bool ssl,dot2d::Ref* target,AppScheduleBase::HandleRequestCallback callback,uint8_t retryTimes = 5);

public:
    virtual bool initWithData(AppDataBase* data);

    AppScheduleBase();

    ~AppScheduleBase();

    static uint16_t APPID();
    
    virtual uint16_t appId();

    AppDataBase* getData();

    void startSchedule(bool executeNow);

    void startScheduleAction(float dt);

    virtual void scheduleAction(float dt);

    String substringBetween(const String& str,const String& left,const String& right);

};


template<typename APP,typename APP_DATA,uint16_t APP_ID>
class AppScheduleTemplate : public AppScheduleBase
{
public:

    static APP* createWithData(APP_DATA* data);

    static uint16_t APPID();
    
    uint16_t appId();

    APP_DATA* getData();

public:
    AppScheduleTemplate();
 
    virtual ~AppScheduleTemplate();
};



//---------------------------AppRootLayer-----------------------

class AppLayerBase : public dot2d::Layer
{
protected:

	AppBase* app = nullptr;

protected:

    bool initWithApp(AppBase* app,const dot2d::Size& size);
    
    virtual bool initLayer() = 0;

public:
    
    static dot2d::Size getSizeByAppSizeType(AppDataBase::AppSizeType type);

	void initSize(const dot2d::Size& size);

};


template<typename APP_LAYER,typename APP,typename APP_DATA>
class AppLayerTemplate : public AppLayerBase
{
public:
    
    APP_DATA* getData();
    
    APP* getApp();

    static APP_LAYER* createWithApp(APP* app,const dot2d::Size& size);
    
};

#define APP_LAYER_CREATE_WITH_APP(T,A)  static T* createWithApp(A* app,const dot2d::Size& size) \
                                        {                                                       \
                                            T* ret = new T();                                   \
                                            if (ret && ret->initWithApp(app,size))              \
                                            {                                                   \
                                                ret->autorelease();                             \
                                                return ret;                                     \
                                            }                                                   \
                                            DT_SAFE_DELETE(ret);                                \
                                            return nullptr;                                     \
                                        }                                                       \


//---------------------------AppBase-----------------------

class AppBase : public dot2d::Scene
{
protected:
    
    typedef bool(dot2d::Ref::*HandleRequestCallback)(const String&);

    AppDataBase* appData;

    AppLayerBase* rootLayer;
    
    friend class AppLayerBase;

protected:

    bool initWithData(AppDataBase* data);

	void autoSwitchUpdate(float dt);

public:

    AppBase();

    ~AppBase();

    virtual void onEnter();

    static uint16_t APPID();

    virtual uint16_t appId();
    
    AppDataBase* getData();

    bool setAutoSwitch();

    bool removeAutoSwitch();

	void resetLayer();

    void resumeEventDispatcherDt(float dt);

    void pauseEventDispatcherDt(float dt);

    virtual void buttonEventEnable(){};

    virtual void buttonEventDisable(){};

	AppDataBase::AppSizeType getAppSizeTypeByScreen();
    
    void pushRequestTask(const String& url,bool ssl,dot2d::Ref* target,AppBase::HandleRequestCallback callback,uint8_t retryTimes);
    
};


template<typename APP,typename APP_DATA,typename APP_SCHEDULE,uint16_t APP_ID>
class AppTemplate : public AppBase
{
public:
    static void* createByFactory(uint8_t type, uint16_t appId, AppDataBase* appData);

    static APP* createWithData(APP_DATA* data);

    static uint16_t APPID();

    uint16_t appId();

    APP_DATA* getData();

    struct Registor
    {
        Registor();
        inline void do_nothing()const {}
    };

    static Registor s_registor;
 
public:
    AppTemplate();
 
    virtual ~AppTemplate();
};








template <typename APP,typename APP_DATA,typename APP_SCHEDULE,uint16_t APP_ID>
void* AppTemplate<APP,APP_DATA,APP_SCHEDULE,APP_ID>::createByFactory(uint8_t type, uint16_t appId,AppDataBase* appData)
{
    //TODO
    switch (type) {
        case 0:
        {
            APP_DATA* appData = new APP_DATA();
            if(appData && appData->init())
            {
                appData->autorelease();
                return appData;
            }
            DT_SAFE_DELETE(appData);
            return nullptr;
        }
            break;
        case 1:
        {
            APP* app = new APP();
            if(app && app->initWithData(appData))
            {
                app->autorelease();
                return app;
            }
            DT_SAFE_DELETE(app);
            return nullptr;
        }
            break;
        case 2:
        {
            APP_SCHEDULE* appSchedule = new APP_SCHEDULE();
            if(appSchedule && appSchedule->initWithData(appData))
            {
                appSchedule->autorelease();
                return appSchedule;
            }
            DT_SAFE_DELETE(appSchedule);
            return nullptr;
        }
            break;
        default:
            break;
    }
    return nullptr;
}



class AppFactory
{
public:
    
    /*
     uint8_t    type
     uint16_t   appId
     type == 0 create AppData
     type == 1 create App
     type == 2 create AppSchedule
     */
    typedef void* (*HandleCreateByFactory)(uint8_t,uint16_t,AppDataBase *);
    
private:
    
    std::map<uint16_t, HandleCreateByFactory> _createByFactoryHandleMap;

public:
 
    static AppFactory& getInstance()
    {
        static AppFactory factory;
        return factory;
    }
 
    bool regist(uint16_t appId, HandleCreateByFactory func)
    {
        if (!func){return false;}
        return _createByFactoryHandleMap.insert(std::make_pair(appId, func)).second;
    }
    
    AppDataBase* createInitAppData(uint16_t appId)
    {
        std::map<uint16_t, HandleCreateByFactory>::iterator it = _createByFactoryHandleMap.find(appId);
        if (it == _createByFactoryHandleMap.end()){return nullptr;}
        return (AppDataBase *)it->second(0,appId,nullptr);
    }
    
    AppBase* createAppWithData(AppDataBase *appData)
    {
        std::map<uint16_t, HandleCreateByFactory>::iterator it = _createByFactoryHandleMap.find(appData->appId());
        if (it == _createByFactoryHandleMap.end()){return nullptr;}
        return (AppBase *)it->second(1,appData->appId(),appData);
    }
    
    AppScheduleBase* createAppScheduleWithData(AppDataBase *appData)
    {
        std::map<uint16_t, HandleCreateByFactory>::iterator it = _createByFactoryHandleMap.find(appData->appId());
        if (it == _createByFactoryHandleMap.end()){return nullptr;}
        return (AppScheduleBase *)it->second(2,appData->appId(),appData);
    }
    
};


#include "AppBase.tpp"


#endif //__APP_BASE_H__
