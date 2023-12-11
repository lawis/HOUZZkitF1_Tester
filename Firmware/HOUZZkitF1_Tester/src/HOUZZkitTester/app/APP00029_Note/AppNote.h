#ifndef __APP_Note_H__
#define __APP_Note_H__

#include "HOUZZkitTester/app/AppBase.h"
#include "HOUZZkitTester/data/SDTData.h"

NS_DT_BEGIN

#define AppNote_APP_ID      29

class AppNoteTheme : public AppTheme
{
public:
    

};

class AppDataNote : public AppDataTemplate<AppDataNote,AppNote_APP_ID>
{
private:
    
    uint8_t _theme;

public:

    AppNoteTheme appTheme;

    String message;

    uint8_t scrollType; //0:None 1:translate 2:shake

    uint8_t alignment;  //0:left 1:center 2:right

    int16_t offsetX;

    bool hasRefresh;

    bool scrollForward;


protected:

    bool subEncode(SDTData::DataSourceType type) override;

    bool subDecode(SDTData::DataSourceType type) override;

public:

    AppDataNote();

    void setTheme(uint8_t t);

};


class AppScheduleNote : public AppScheduleTemplate<AppScheduleNote,AppDataNote,AppNote_APP_ID>
{

public:

   	bool parseRequest(const String& res);

	void scheduleAction(float dt) override;

};


class AppNote : public AppTemplate<AppNote,AppDataNote,AppScheduleNote,AppNote_APP_ID>
{

protected:

    bool init() override;

public:



};


class AppNoteLayer : public AppLayerTemplate<AppNoteLayer,AppNote,AppDataNote>
{
    
private:


protected:


public:


public:

    bool initLayer() override;
    
};


NS_DT_END

#endif //__APP_Note_H__

