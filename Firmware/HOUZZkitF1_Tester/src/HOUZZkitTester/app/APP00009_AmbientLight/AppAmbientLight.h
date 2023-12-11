#ifndef __APP_AmbientLight_H__
#define __APP_AmbientLight_H__

#include "HOUZZkitTester/app/AppBase.h"
#include "HOUZZkitTester/data/SDTData.h"

#ifndef TARGET_OS_MAC
#include "FastLED.h"
#include "colorutils.h"
#endif

NS_DT_BEGIN

#define AppAmbientLight_APP_ID      9

class AppDataAmbientLight : public AppDataTemplate<AppDataAmbientLight,AppAmbientLight_APP_ID>
{
public:

    enum class AmbientEffectType : uint8_t
    {
        RANDOM = 0,
        FIRE,
        CONFETTI,
        RAINBOW,
        PURPLEDREAM,
        SNOW,
        FOREST,
        CLOUD,
        LAVA,
        OCEAN,
        COUNT
    };

    AmbientEffectType ambientEffectType;

protected:

    bool subEncode(SDTData::DataSourceType type) override;

    bool subDecode(SDTData::DataSourceType type) override;

public:

    AppDataAmbientLight();
    
    AmbientEffectType randomEffect(){return (AmbientEffectType)random(1,(uint8_t)AmbientEffectType::COUNT-1);}

};


class AppScheduleAmbientLight : public AppScheduleTemplate<AppScheduleAmbientLight,AppDataAmbientLight,AppAmbientLight_APP_ID>
{

public:

	void scheduleAction(float dt) override;

};


class AppAmbientLight : public AppTemplate<AppAmbientLight,AppDataAmbientLight,AppScheduleAmbientLight,AppAmbientLight_APP_ID>
{

protected:

    bool init() override;

public:



};


class AppAmbientLightLayer : public AppLayerTemplate<AppAmbientLightLayer,AppAmbientLight,AppDataAmbientLight>
{
    
private:

    uint16_t w;
    uint16_t h;
    uint16_t maxDimension;

    uint8_t *fireData = nullptr;
    
    uint8_t gHue = 0;
    
    uint16_t x;
    uint16_t y;
    uint16_t z;

    uint16_t speed;
    uint16_t scale;
    uint8_t  colorLoop;

    AppDataAmbientLight::AmbientEffectType _dataPreAmbientEffectType;

#ifndef TARGET_OS_MAC
    CRGBPalette16 currentPalette;
#endif
    uint8_t *noiseData = nullptr; 

protected:

    SpriteCanvas *canvas = nullptr;
    
    AppDataAmbientLight::AmbientEffectType ambientEffectType;

    

private:

    void freeData();
    
public:
    
    virtual ~AppAmbientLightLayer();

    bool initLayer() override;

    void update(float dt) override;
    
    void effectSwitch(float dt);
    
    void fireEffect();
    
    void confettiEffect();


    void changePaletteAndSettingsPeriodically(AppDataAmbientLight::AmbientEffectType effectType);
    
    void fillnoise8(uint16_t w,uint16_t h);
    
    void mapNoiseToLEDsUsingPalette(uint16_t w,uint16_t h);
    
    void paletteNoiseEffect();
    
    
};


NS_DT_END

#endif //__APP_AmbientLight_H__

