#ifndef __APP_Spectrum_H__
#define __APP_Spectrum_H__

#include "HOUZZkitTester/app/AppBase.h"
#include "HOUZZkitTester/data/SDTData.h"
#include <arduinoFFT.h>
NS_DT_BEGIN

#define AUDIO_INPUT_PIN 35              // Signal in on this pin

#define SAMPLES         512            // Must be a power of 2
#define SAMPLING_FREQ   40000           // Hz, must be 40000 or less due to ADC conversion time. Determines maximum frequency that can be analysed by the FFT Fmax=sampleF/2.
#define AMPLITUDE       130             // Depending on your audio source level, you may need to alter this value. Can be used as a 'sensitivity' control.
#define NOISE           250            // Used as a crude noise filter, values below this are ignored

#define AppSpectrum_APP_ID      10

class AppDataSpectrum : public AppDataTemplate<AppDataSpectrum,AppSpectrum_APP_ID>
{
public:

    enum class SpectrumEffectType : uint8_t
    {
        RANDOM = 0,
        LINE_RAINBOW,
        LINE_VOLCANO,
        LINE_CHROMATOGRAM,
        COUNT
    };

    SpectrumEffectType spectrumEffectType;

protected:

    bool subEncode(SDTData::DataSourceType type) override;

    bool subDecode(SDTData::DataSourceType type) override;

public:

    AppDataSpectrum();

    SpectrumEffectType randomEffect(){return (SpectrumEffectType)random(1,(uint8_t)SpectrumEffectType::COUNT-1);}

};


class AppScheduleSpectrum : public AppScheduleTemplate<AppScheduleSpectrum,AppDataSpectrum,AppSpectrum_APP_ID>
{

public:

	void scheduleAction(float dt) override;

};


class AppSpectrum : public AppTemplate<AppSpectrum,AppDataSpectrum,AppScheduleSpectrum,AppSpectrum_APP_ID>
{

protected:

    bool init() override;

public:



};


class AppSpectrumLayer : public AppLayerTemplate<AppSpectrumLayer,AppSpectrum,AppDataSpectrum>
{
    
private:
    arduinoFFT      FFT;

    double          _vReal[SAMPLES];
    double          _vImag[SAMPLES];
    unsigned int    _sampling_period_us;

    uint16_t        _bandsCount = 16;    

    uint32_t*       _bandValues = nullptr;
    uint8_t*        _bandValuesCount = nullptr;
    uint16_t*       _binWidth = nullptr;
    uint8_t*        _peak = nullptr;
    uint8_t*        _oldBarHeights = nullptr;
    
    unsigned long   _newTime;
    
    AppDataSpectrum::SpectrumEffectType _dataPreSpectrumEffectType;

protected:

    SpriteCanvas *canvas = nullptr;

    AppDataSpectrum::SpectrumEffectType spectrumEffectType;

public:


public:

    AppSpectrumLayer();

    ~AppSpectrumLayer();

    bool initLayer() override;

    void effectSwitch(float dt);
    
    void update(float dt) override;

    void peakDown(float dt);

    void drawLineAndPeak();

};


NS_DT_END

#endif //__APP_Spectrum_H__

