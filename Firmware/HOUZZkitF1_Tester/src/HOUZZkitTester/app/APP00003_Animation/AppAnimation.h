#ifndef __APP_Animation_H__
#define __APP_Animation_H__

#include "HOUZZkitTester/app/AppBase.h"
#include "HOUZZkitTester/data/SDTData.h"

#ifndef TARGET_OS_MAC
#include "GifDecoder_Impl.h"
#endif
NS_DT_BEGIN

#define AppAnimation_APP_ID      3

class AppDataAnimation : public AppDataTemplate<AppDataAnimation,AppAnimation_APP_ID>
{
public:
    
    bool bufferRefresh;

    String imgType;

    uint16_t width;

    uint16_t height;


protected:
    
    bool subEncode(SDTData::DataSourceType type) override;

    bool subDecode(SDTData::DataSourceType type) override;

public:
    
    ~AppDataAnimation();
    
    AppDataAnimation();

    bool appAddInit() override;
    
    uint8_t* getImageBuffer(uint32_t *size);

    bool isImageExist();
    
    void saveImageBuffer(const uint8_t* buffer,uint32_t size);
    
    void attachmentRemove() override;

    String imgPath();

};


class AppRequestAnimation : public AppScheduleTemplate<AppRequestAnimation,AppDataAnimation,AppAnimation_APP_ID>
{

public:

   	bool parseRequest(const String& res);

    void scheduleAction(float dt) override;

};


class AppAnimation : public AppTemplate<AppAnimation,AppDataAnimation,AppRequestAnimation,AppAnimation_APP_ID>
{

protected:

    bool init() override;

};



class AppAnimationLayer : public AppLayerTemplate<AppAnimationLayer,AppAnimation,AppDataAnimation>
{
protected:

    SpriteCanvas* renderCanvas = nullptr;

    SpriteCanvas* canvas = nullptr;

    CanvasSprite *sprite = nullptr;
#ifndef TARGET_OS_MAC
    GifDecoder<32, 32, 12> decoder;

    File file;
#endif
    int openResult;

public:



protected:



public:

    AppAnimationLayer();

    ~AppAnimationLayer();

    void onExit() override;

    void onEnter() override;

    bool initLayer() override;

    void update(float dt) override;

    void customUpdate(float dt);

};

NS_DT_END

#endif //__APP_Animation_H__

