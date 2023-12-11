#include "AppBase.h"


//---------------------------AppDataTemplate-----------------------

template<typename APP_DATA,uint16_t APP_ID>
APP_DATA* AppDataTemplate<APP_DATA,APP_ID>::create()
{
    APP_DATA* ret = new APP_DATA();
    if (ret && ret->init())
    {
        ret->autorelease();
        return ret;
    }
    DT_SAFE_DELETE(ret);
    return nullptr;
}

template<typename APP_DATA,uint16_t APP_ID>
APP_DATA* AppDataTemplate<APP_DATA,APP_ID>::createWithBuff(const byte* buffer,uint32_t size)
{
    APP_DATA* ret = new APP_DATA();
    if (ret && ret->initWithBuff(buffer,size))
    {
        ret->autorelease();
        return ret;
    }
    DT_SAFE_DELETE(ret);
    return nullptr;
}

template<typename APP_DATA,uint16_t APP_ID>
uint16_t AppDataTemplate<APP_DATA,APP_ID>::APPID()
{
    return APP_ID;
}
    
template<typename APP_DATA,uint16_t APP_ID>
uint16_t AppDataTemplate<APP_DATA,APP_ID>::appId()
{
    return APP_ID;
}


//---------------------------AppLayerTemplate-----------------------

template<typename APP_LAYER,typename APP,typename APP_DATA>
APP* AppLayerTemplate<APP_LAYER,APP,APP_DATA>::getApp()
{
    return (APP*)app;
}

template<typename APP_LAYER,typename APP,typename APP_DATA>
APP_LAYER* AppLayerTemplate<APP_LAYER,APP,APP_DATA>::createWithApp(APP* app,const dot2d::Size& size)
{
    APP_LAYER* ret = new APP_LAYER();
    if (ret && ret->initWithApp(app,size))
    {
        ret->autorelease();
        return ret;
    }
    DT_SAFE_DELETE(ret);
    return nullptr;
}


template<typename APP_LAYER,typename APP,typename APP_DATA>
APP_DATA* AppLayerTemplate<APP_LAYER,APP,APP_DATA>::getData()
{
    return (APP_DATA*)app->getData();
}




//---------------------------AppTemplate-----------------------

//template <typename APP,typename APP_DATA,typename APP_SCHEDULE,uint16_t APP_ID>
//bool AppTemplate<APP,APP_DATA,APP_SCHEDULE,APP_ID>::initWithData(APP_DATA* data)
//{
//    appData = data;
//    return this->init();
//}







template <typename APP,typename APP_DATA,typename APP_SCHEDULE,uint16_t APP_ID>
APP * AppTemplate<APP,APP_DATA,APP_SCHEDULE,APP_ID>::createWithData(APP_DATA* data)
{
    APP* ret = new APP();
    if (ret && ret->initWithData((AppDataBase*)data))
    {
        ret->autorelease();
        return ret;
    }
    DT_SAFE_DELETE(ret);
    return nullptr;
}

template <typename APP,typename APP_DATA,typename APP_SCHEDULE,uint16_t APP_ID>
uint16_t AppTemplate<APP,APP_DATA,APP_SCHEDULE,APP_ID>::APPID()
{
    return APP_ID;
}

template <typename APP,typename APP_DATA,typename APP_SCHEDULE,uint16_t APP_ID>
uint16_t AppTemplate<APP,APP_DATA,APP_SCHEDULE,APP_ID>::appId()
{
    return APP_ID;
}

template <typename APP,typename APP_DATA,typename APP_SCHEDULE,uint16_t APP_ID>
APP_DATA* AppTemplate<APP,APP_DATA,APP_SCHEDULE,APP_ID>::getData()
{
    return (APP_DATA *)appData;
}


template <typename APP,typename APP_DATA,typename APP_SCHEDULE,uint16_t APP_ID>
AppTemplate<APP,APP_DATA,APP_SCHEDULE,APP_ID>::Registor::Registor()
{
    AppFactory::getInstance().regist(APP::APPID(), (AppFactory::HandleCreateByFactory)createByFactory);
}

template <typename APP,typename APP_DATA,typename APP_SCHEDULE,uint16_t APP_ID>
typename AppTemplate<APP,APP_DATA,APP_SCHEDULE,APP_ID>::Registor AppTemplate<APP,APP_DATA,APP_SCHEDULE,APP_ID>::s_registor;

template <typename APP,typename APP_DATA,typename APP_SCHEDULE,uint16_t APP_ID>
AppTemplate<APP,APP_DATA,APP_SCHEDULE,APP_ID>::AppTemplate()
{
    s_registor.do_nothing();
}

template <typename APP,typename APP_DATA,typename APP_SCHEDULE,uint16_t APP_ID>
AppTemplate<APP,APP_DATA,APP_SCHEDULE,APP_ID>::~AppTemplate()
{
    s_registor.do_nothing();
}


//---------------------------AppScheduleTemplate-----------------------

template <typename APP,typename APP_DATA,uint16_t APP_ID>
APP * AppScheduleTemplate<APP,APP_DATA,APP_ID>::createWithData(APP_DATA* data)
{
    APP* ret = new APP();
    if (ret && ret->initWithData((AppDataBase*)data))
    {
        ret->autorelease();
        return ret;
    }
    DT_SAFE_DELETE(ret);
    return nullptr;
}

template <typename APP,typename APP_DATA,uint16_t APP_ID>
uint16_t AppScheduleTemplate<APP,APP_DATA,APP_ID>::APPID()
{
    return APP_ID;
}


template <typename APP,typename APP_DATA,uint16_t APP_ID>
uint16_t AppScheduleTemplate<APP,APP_DATA,APP_ID>::appId()
{
    return APP_ID;
}


template <typename APP,typename APP_DATA,uint16_t APP_ID>
APP_DATA* AppScheduleTemplate<APP,APP_DATA,APP_ID>::getData()
{
    return (APP_DATA *)appData;
}


template <typename APP,typename APP_DATA,uint16_t APP_ID>
AppScheduleTemplate<APP,APP_DATA,APP_ID>::AppScheduleTemplate()
{
    
}

template <typename APP,typename APP_DATA,uint16_t APP_ID>
AppScheduleTemplate<APP,APP_DATA,APP_ID>::~AppScheduleTemplate()
{
    
}
