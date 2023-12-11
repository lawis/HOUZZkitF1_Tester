#ifndef __APP_Matrix_H__
#define __APP_Matrix_H__

#include "HOUZZkitTester/app/AppBase.h"
#include "HOUZZkitTester/data/SDTData.h"

NS_DT_BEGIN

#define AppMatrix_APP_ID      5

class AppDataMatrix : public AppDataTemplate<AppDataMatrix,AppMatrix_APP_ID>
{
public:

    //增加自定义成员变量

protected:

    bool subEncode(SDTData::DataSourceType type) override;

    bool subDecode(SDTData::DataSourceType type) override;

public:
    
    AppDataMatrix();

};


class AppRequestMatrix : public AppScheduleTemplate<AppRequestMatrix,AppDataMatrix,AppMatrix_APP_ID>
{

public:

   	bool parseRequest(const String& res);

    void scheduleAction(float dt) override;

};


class AppMatrix : public AppTemplate<AppMatrix,AppDataMatrix,AppRequestMatrix,AppMatrix_APP_ID>
{

protected:

    bool init() override;

};



class AppMatrixLayer : public AppLayerTemplate<AppMatrixLayer,AppMatrix,AppDataMatrix>
{
protected:
    
    struct Animate
    {
        int16_t offset;
        float interval;
        float dInterval;
    };

    float showInterval = 0.0f;
    
    Animate* animateList = nullptr;
    
    uint8_t animateCount = 0;

public:

    virtual ~AppMatrixLayer();

protected:
    
    CanvasSprite* canvasSprite = nullptr;

public:

    virtual bool initLayer() override;
    
    void update(float dt) override;

};


NS_DT_END

#endif //__APP_Matrix_H__

