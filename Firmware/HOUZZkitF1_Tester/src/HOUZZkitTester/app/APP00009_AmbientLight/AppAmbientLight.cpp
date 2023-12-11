#include "AppAmbientLight.h"
NS_DT_BEGIN

AppDataAmbientLight::AppDataAmbientLight()
:ambientEffectType(AmbientEffectType::RANDOM)
{
    appAlias = "氛围灯";
    canPlay = true;
    openPlay = true;
    isActive = true;
}

bool AppDataAmbientLight::subEncode(SDTData::DataSourceType type)
{
    pushUint8((uint8_t)ambientEffectType);
    return true;
}

bool AppDataAmbientLight::subDecode(SDTData::DataSourceType type)
{
    ambientEffectType = (AppDataAmbientLight::AmbientEffectType)popUint8();
    return true;
}


void AppScheduleAmbientLight::scheduleAction(float dt)
{
    
}


bool AppAmbientLight::init()
{
    Size size = AppAmbientLightLayer::getSizeByAppSizeType(this->getData()->appSizeType);
    rootLayer = AppAmbientLightLayer::createWithApp(this,size);
    this->addChild(rootLayer);
    return true;
}

void AppAmbientLightLayer::freeData()
{
    DT_SAFE_FREE(fireData);
    
    x = random(0,UINT16_MAX);
    y = random(0,UINT16_MAX);
    z = random(0,UINT16_MAX);
    speed = 20;
    scale = 30;
    colorLoop = 1;
    DT_SAFE_FREE(noiseData);
}

AppAmbientLightLayer::~AppAmbientLightLayer()
{
    freeData();
}

bool AppAmbientLightLayer::initLayer()
{
    canvas = SpriteCanvas::create(this->getContentSize());
    dot2d::CanvasSprite *sprite = dot2d::CanvasSprite::create(canvas);
    _dataPreAmbientEffectType = this->getData()->ambientEffectType;
    this->addChild(sprite);
    this->effectSwitch(0);
    this->schedule(DT_SCHEDULE_SELECTOR(AppAmbientLightLayer::effectSwitch),10);
    this->scheduleUpdate();
    return true;
}

void AppAmbientLightLayer::effectSwitch(float dt)
{
    AppDataAmbientLight::AmbientEffectType preEffect = ambientEffectType;
    if (this->getData()->ambientEffectType == AppDataAmbientLight::AmbientEffectType::RANDOM) {
        ambientEffectType = this->getData()->randomEffect();
    }else
    {
        ambientEffectType = this->getData()->ambientEffectType;
    }
    if (preEffect != ambientEffectType) {
        freeData();
        changePaletteAndSettingsPeriodically(ambientEffectType);
    }
}

void AppAmbientLightLayer::update(float dt)
{
    if (_dataPreAmbientEffectType != this->getData()->ambientEffectType)
    {
        this->effectSwitch(0);
        _dataPreAmbientEffectType = this->getData()->ambientEffectType;
        this->unschedule(DT_SCHEDULE_SELECTOR(AppAmbientLightLayer::effectSwitch));
        this->schedule(DT_SCHEDULE_SELECTOR(AppAmbientLightLayer::effectSwitch),10);
    }
    switch (this->ambientEffectType) {
        case AppDataAmbientLight::AmbientEffectType::FIRE:
            fireEffect();
            break;
        case AppDataAmbientLight::AmbientEffectType::CONFETTI:
            confettiEffect();
            break;
        case AppDataAmbientLight::AmbientEffectType::RAINBOW:
        case AppDataAmbientLight::AmbientEffectType::PURPLEDREAM:
        case AppDataAmbientLight::AmbientEffectType::SNOW:
        case AppDataAmbientLight::AmbientEffectType::FOREST:
        case AppDataAmbientLight::AmbientEffectType::CLOUD:
        case AppDataAmbientLight::AmbientEffectType::LAVA:
        case AppDataAmbientLight::AmbientEffectType::OCEAN:
            paletteNoiseEffect();
            break;
            
        default:
            break;
    }
}


#define SPARKING 120
#define COOLING  50
void AppAmbientLightLayer::fireEffect()
{
    Size size = this->getContentSize();
    if (!fireData) {
        fireData = (uint8_t *)malloc(size.width*size.height);
    }
    for (int w = 0; w < size.width; w++)
    {
      for (int h = 0; h < size.height; h++)
      {
          fireData[w*size.height+h] = qsub8(fireData[w*size.height+h],  random(0, ((COOLING * 10) / size.height) + 2));
      }
      for ( int k = size.height - 1; k >= 2; k--) {
        fireData[w*size.height+k] = (fireData[w*size.height+k-1] + fireData[w*size.height+k-2] + fireData[w*size.height+k-2] ) / 3;
      }
      if ( random(0,255) < SPARKING ) {
        int y = random(0,1);
        fireData[w*size.height+y] = qadd8(fireData[w*size.height+y], random(160, 255) );
      }
      for ( int j = 0; j < size.height; j++) {
        DTRGB color = HeatColor(fireData[w*size.height+j]);
        canvas->drawPixel(w, size.height - 1 - j, color);
      }
    }
}

void AppAmbientLightLayer::confettiEffect()
{
    gHue++;
    Size size = this->getContentSize();
    for (uint16_t x = 0; x < size.width; x++)
    {
      for (uint16_t y = 0; y < size.height; y++)
      {
          canvas->drawPixel(x, y, canvas->colorWithXY(x, y).fadeToBlackBy(10));
      }
    }
    for (uint8_t t = 0; t<1; t++) {
        uint16_t posX = random(0,size.width);
        uint16_t posY = random(0,size.height);
        DTRGB color = canvas->colorWithXY(posX,posY);
        color += DTHSV( gHue + random(0,64), 200, 255);
        canvas->drawPixel(posX, posY, color);
    }
}


void AppAmbientLightLayer::changePaletteAndSettingsPeriodically(AppDataAmbientLight::AmbientEffectType effectType)
{
#ifndef TARGET_OS_MAC
    switch (effectType)
    {
    case AppDataAmbientLight::AmbientEffectType::RAINBOW:
    {
        currentPalette = CRGBPalette16(0xFF0000, 0xD52A00, 0xAB5500, 0xAB7F00,
                        0xABAB00, 0x56D500, 0x00FF00, 0x00D52A,
                        0x00AB55, 0x0056AA, 0x0000FF, 0x2A00D5,
                        0x5500AB, 0x7F0081, 0xAB0055, 0xD5002B);         
        speed = 5; 
        scale = 10; 
        colorLoop = 0;
    }
        break;
    case AppDataAmbientLight::AmbientEffectType::PURPLEDREAM:
    {
        CRGB purple = CHSV( HUE_PURPLE, 255, 255);
        CRGB green  = CHSV( HUE_GREEN, 255, 255);
        CRGB black  = CRGB::Black;
    
        currentPalette = CRGBPalette16(
            green,  green,  black,  black,
            purple, purple, black,  black,
            green,  green,  black,  black,
            purple, purple, black,  black );             
            speed = 10; 
            scale = 20; 
            colorLoop = 1;
    }
        break;

    case AppDataAmbientLight::AmbientEffectType::SNOW:
    {
        fill_solid( currentPalette, 16, CRGB::Black);
        currentPalette[0] = CRGB::White;
        currentPalette[4] = CRGB::White;
        currentPalette[8] = CRGB::White;
        currentPalette[12] = CRGB::White;       
        speed = 10; 
        scale = 10; 
        colorLoop = 1; 
    }
        break;
    case AppDataAmbientLight::AmbientEffectType::FOREST:
    {
        currentPalette = ForestColors_p;          
        speed =  8; 
        scale = 20; 
        colorLoop = 0;
    }
        break;
    case AppDataAmbientLight::AmbientEffectType::CLOUD:
    {
        currentPalette = CloudColors_p;           
        speed =  4; 
        scale = 30; 
        colorLoop = 0;
    }
        break;
    case AppDataAmbientLight::AmbientEffectType::LAVA:
    {
        currentPalette = LavaColors_p;            
        speed =  8; 
        scale = 50; 
        colorLoop = 0; 
    }
        break;
    case AppDataAmbientLight::AmbientEffectType::OCEAN:
    {
        currentPalette = OceanColors_p;           
        speed = 20; 
        scale = 90; 
        colorLoop = 0; 
    }
        break;
    default:
        break;
    }
#endif
}




void AppAmbientLightLayer::fillnoise8(uint16_t w,uint16_t h) {
#ifndef TARGET_OS_MAC
  uint8_t dataSmoothing = 0;
  if( speed < 50) {
    dataSmoothing = 200 - (speed * 4);
  }
  for(int i = 0; i < w; i++) {
    int ioffset = scale * i;
    for(int j = 0; j < h; j++) {
      int joffset = scale * j;
      uint8_t data = inoise8(x + ioffset,y + joffset,z);
      data = qsub8(data,16);
      data = qadd8(data,scale8(data,39));
      if( dataSmoothing ) {
        uint8_t olddata = noiseData[i * w + j];
        uint8_t newdata = scale8( olddata, dataSmoothing) + scale8( data, 256 - dataSmoothing);
        data = newdata;
      }
      noiseData[i * w + j] = data;
    }
  }
  z += speed;
  x += speed / 8;
  y -= speed / 16;
#endif
}

void AppAmbientLightLayer::mapNoiseToLEDsUsingPalette(uint16_t w,uint16_t h)
{
#ifndef TARGET_OS_MAC
  static uint8_t ihue=0;
  for(int i = 0; i < w; i++) {
    for(int j = 0; j < h; j++) {
      uint8_t index = noiseData[i * w + j];
      uint8_t bri =   noiseData[i * w + j];
      if( colorLoop) {
        index += ihue;
      }
      if( bri > 127 ) {
        bri = 255;
      } else {
        bri = dim8_raw( bri * 2);
      }
      CRGB color = ColorFromPalette( currentPalette, index, bri);
      canvas->drawPixel(i,j,DTRGB(color.r,color.g,color.b));
    }
  }
  ihue+=1;
#endif
}

void AppAmbientLightLayer::paletteNoiseEffect()
{
    Size size = this->getContentSize();
    maxDimension =  MAX(size.width, size.height);
    if (!noiseData) {
        noiseData = (uint8_t *)malloc(maxDimension*maxDimension);
    }
    fillnoise8(size.width,size.height);
    mapNoiseToLEDsUsingPalette(size.width,size.height);
}


NS_DT_END
