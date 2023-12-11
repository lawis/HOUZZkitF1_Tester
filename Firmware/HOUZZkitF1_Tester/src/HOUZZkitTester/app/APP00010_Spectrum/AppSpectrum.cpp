#include "AppSpectrum.h"

NS_DT_BEGIN


AppDataSpectrum::AppDataSpectrum()
:spectrumEffectType(SpectrumEffectType::RANDOM)
{
    appAlias = "音乐频谱";
    canPlay = true;
    openPlay = true;
    isActive = true;
}

bool AppDataSpectrum::subEncode(SDTData::DataSourceType type)
{
    pushUint8((uint8_t)spectrumEffectType);
    return true;
}

bool AppDataSpectrum::subDecode(SDTData::DataSourceType type)
{
    spectrumEffectType = (AppDataSpectrum::SpectrumEffectType)popUint8();
    return true;
}


void AppScheduleSpectrum::scheduleAction(float dt)
{
    
}


bool AppSpectrum::init()
{
    Size size = AppSpectrumLayer::getSizeByAppSizeType(this->getData()->appSizeType);
    rootLayer = AppSpectrumLayer::createWithApp(this,size);
    this->addChild(rootLayer);
    return true;
}

AppSpectrumLayer::AppSpectrumLayer()
{
    FFT = arduinoFFT(_vReal, _vImag, SAMPLES, SAMPLING_FREQ);
    _sampling_period_us = round(1000000 * (1.0 / SAMPLING_FREQ));
}

AppSpectrumLayer::~AppSpectrumLayer()
{
    DT_SAFE_FREE(_peak);
    DT_SAFE_FREE(_oldBarHeights);
    DT_SAFE_FREE(_bandValues);
    DT_SAFE_FREE(_bandValuesCount);
}

bool AppSpectrumLayer::initLayer()
{
    _dataPreSpectrumEffectType = this->getData()->spectrumEffectType;

    this->effectSwitch(0);

    Size size = this->getContentSize();
    _bandsCount = MIN(size.width,32);

    _bandValues = (uint32_t *)malloc(_bandsCount*sizeof(uint32_t));
    _bandValuesCount = (uint8_t *)malloc(_bandsCount);
    _binWidth = (uint16_t *)malloc(_bandsCount*sizeof(uint16_t));
    _peak = (uint8_t *)malloc(_bandsCount);
    memset(_peak,0,_bandsCount);
    _oldBarHeights = (uint8_t *)malloc(_bandsCount);
    memset(_oldBarHeights,0,_bandsCount);

    uint16_t lowFreq = 100;
    uint32_t highFreq = 12000;
    double frequencyPerBand = pow(highFreq/lowFreq,1.0f/(_bandsCount-1));
    float binWidth = SAMPLING_FREQ / (float)SAMPLES;

    uint16_t frequency0 = lowFreq * pow(frequencyPerBand,0);
    uint16_t centerBin0 = frequency0 / binWidth + 0.5;
    for (uint8_t i = 1; i <= _bandsCount; i++)
    {
        uint16_t frequency1 = lowFreq * pow(frequencyPerBand,i);
        uint16_t centerBin1 = frequency1 / binWidth + 0.5;
        _binWidth[i-1] = (centerBin1 - centerBin0) / 2 + centerBin0;
        if (i>=2)
        {
            if (_binWidth[i-1] <= _binWidth[i-2])
            {
                _binWidth[i-1] = _binWidth[i-2] + 1;
            }
        }
        frequency0 = frequency1;
        centerBin0 = centerBin1;
    }

    canvas = SpriteCanvas::create(size);
    dot2d::CanvasSprite *sprite = dot2d::CanvasSprite::create(canvas);
    this->addChild(sprite);
#ifndef TARGET_OS_MAC
    pinMode(AUDIO_INPUT_PIN,INPUT);
#endif
    this->scheduleUpdate();
    this->schedule(DT_SCHEDULE_SELECTOR(AppSpectrumLayer::peakDown),0.2);
    return true;
}

void AppSpectrumLayer::effectSwitch(float dt)
{
    spectrumEffectType = this->getData()->spectrumEffectType;
    if (spectrumEffectType == AppDataSpectrum::SpectrumEffectType::RANDOM)
    {
        spectrumEffectType = this->getData()->randomEffect();
    }
}

void AppSpectrumLayer::update(float dt)
{
    if (_dataPreSpectrumEffectType != this->getData()->spectrumEffectType)
    {
        this->effectSwitch(0);
        _dataPreSpectrumEffectType = this->getData()->spectrumEffectType;
    }

    Size size = this->getContentSize();
    canvas->canvasReset();

    // Reset _bandValues
    memset(_bandValues,0,_bandsCount*sizeof(int32_t));
    memset(_bandValuesCount,0,_bandsCount);

    // Sample the audio pin
    for (uint16_t i = 0; i < SAMPLES; i++) {
#ifndef TARGET_OS_MAC
        _newTime = micros();
        _vReal[i] = analogRead(AUDIO_INPUT_PIN); // A conversion takes about 9.7uS on an ESP32
        _vImag[i] = 0;
        while ((micros() - _newTime) < _sampling_period_us) { /* chill */ }
#endif
    }

    // Compute FFT
    FFT.Windowing(_vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(_vReal, _vImag, SAMPLES, FFT_FORWARD);
    FFT.ComplexToMagnitude(_vReal, _vImag, SAMPLES);

    // Analyse FFT results
    for (uint16_t i = 2; i < (SAMPLES/2); i++)// Don't use sample 0 and only first SAMPLES/2 are usable. Each array element represents a frequency bin and its value the amplitude.
    {     
        if (_vReal[i] > NOISE) {
            if (i>_binWidth[_bandsCount-1])
            {
                _bandValues[_bandsCount-1]  += (uint32_t)_vReal[i];
                _bandValuesCount[_bandsCount-1]++;
            }else
            {
                for (uint8_t j = 0; j < _bandsCount-1; j++)
                {            
                    if (i<=_binWidth[j])
                    {
                        _bandValues[j]  += (uint32_t)_vReal[i];
                        _bandValuesCount[j]++;
                        break;
                    }
                }
            }
        }
    }    

    uint8_t maxHeight = size.height-1;
    // Process the FFT data into bar heights
    float amplitude = AMPLITUDE / (maxHeight / 7.0);
    for (uint8_t band = 0; band < _bandsCount; band++) {
        if (_bandValuesCount[band]==0)
        {
            _bandValuesCount[band] = 1;
        }
        // Scale the bars for the display
        uint8_t barHeight = _bandValues[band] / _bandValuesCount[band] / amplitude;
        barHeight = MIN(maxHeight, barHeight);
        barHeight = (_oldBarHeights[band] + barHeight * 2) / 3.0 + 0.5;
        if (barHeight > _peak[band]) {
            _peak[band] = barHeight;
        }
        _oldBarHeights[band] = barHeight;
    }
    drawLineAndPeak();
}

void AppSpectrumLayer::peakDown(float dt)
{
    for (uint8_t band = 0; band < _bandsCount; band++)
    {
        if (_peak[band] > 0) 
        {
            _peak[band] -= 1;
        }
    }
}

void AppSpectrumLayer::drawLineAndPeak()
{
    Size size = this->getContentSize();
    uint8_t offset = (size.width - _bandsCount) / 2;
    uint8_t maxHeight = size.height - 1;
    for (uint8_t x = 0; x < _bandsCount; x++) 
    {
        uint8_t h = _oldBarHeights[x];
        switch (spectrumEffectType)
        {
        case AppDataSpectrum::SpectrumEffectType::LINE_RAINBOW:
        {
            canvas->drawLine(offset + x,maxHeight,offset + x,maxHeight-h,DTHSV((x) * (255 / _bandsCount), 255, 255));
            canvas->drawPixel(offset + x,maxHeight-_peak[x],DTRGB(100,100,100));
        }
            break;
        case AppDataSpectrum::SpectrumEffectType::LINE_VOLCANO:
        {
            canvas->drawLine(offset + x,maxHeight,offset + x,maxHeight-h,DTRGB(100, 0, 0));
            canvas->drawPixel(offset + x,maxHeight-_peak[x],DTRGB(100,100,100));
        }
            break;
        case AppDataSpectrum::SpectrumEffectType::LINE_CHROMATOGRAM:
        {
            for (uint8_t y = 0; y < h; y++)
            {
                canvas->drawPixel(offset + x,maxHeight-y,DTHSV((maxHeight-y) * (255 / maxHeight),255,255));
            }
            canvas->drawPixel(offset + x,maxHeight-_peak[x],DTRGB(100,100,100));
        }
            break;
        default:
            break;
        }
    }


    
}

NS_DT_END
