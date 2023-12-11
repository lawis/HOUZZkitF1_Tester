#include "AppMatrix.h"

NS_DT_BEGIN

AppDataMatrix::AppDataMatrix()
{
    appAlias = "黑客帝国绿屏特效";
    canPlay = true;
    openPlay = true;
    isActive = true;
}

bool AppDataMatrix::subEncode(SDTData::DataSourceType type)
{
    return true;
}

bool AppDataMatrix::subDecode(SDTData::DataSourceType type)
{
    return true;
}

bool AppRequestMatrix::parseRequest(const String& res)
{
    return true;
}

void AppRequestMatrix::scheduleAction(float dt)
{
    
}

bool AppMatrix::init()
{
    Size size = AppLayerBase::getSizeByAppSizeType(this->getData()->appSizeType);
    rootLayer = AppMatrixLayer::createWithApp(this,size);
    this->addChild(rootLayer);
    return true;
}


AppMatrixLayer::~AppMatrixLayer()
{
    DT_SAFE_FREE(animateList);
}

bool AppMatrixLayer::initLayer()
{
    Size size = this->getContentSize();
    showInterval = size.height / 12.0;
    animateCount = size.width;

    animateList = (Animate*)malloc(sizeof(Animate)*animateCount);
    for (int16_t i=0; i<animateCount; i++) {
        Animate& animate = animateList[i];
        animate.offset = INT16_MAX;
    }
    
    canvasSprite = CanvasSprite::create(size.width,size.height);
    this->addChild(canvasSprite);
    
    this->update(0);
    this->scheduleUpdate();
    
    return true;
}

void AppMatrixLayer::update(float dt)
{
    Size size = this->getContentSize();
    SpriteCanvas* canvas = canvasSprite->getSpriteCanvas();
    canvas->canvasReset();
    for (int16_t i=0; i<animateCount; i++) {
        Animate& animate = animateList[i];
        if (animate.offset > size.height * 2) {
            animate.offset = -random(0,size.height);
            animate.interval = random(showInterval * 1,showInterval * 2) / (float)(size.height*2);
            animate.dInterval = animate.interval;
        }
        animate.dInterval-=dt;
        if (animate.dInterval<=0) {
            animate.offset++;
            animate.dInterval = animate.interval;
        }
        canvas->writeGradientLine(i, animate.offset, i, animate.offset - (size.height-1), DTHSV(100,50,200), DTHSV(100,255,0));
        canvas->writeGradientLine(i, animate.offset - size.height, i, animate.offset - (size.height*2-1), DTHSV(100,50,200), DTHSV(100,255,0));
    }
}

NS_DT_END
