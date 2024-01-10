#include "HFTScreen.h"
#include "HFTBuildConfig.h"

static ScreenManager *s_screenManager = nullptr;

bool ScreenManager::init()
{
    ledMatrix = new LedMatrix();
    lcdDisplay = new LcdDisplay();
    return true;
}

ScreenManager *ScreenManager::getInstance()
{
    if (s_screenManager == nullptr)
    {
        s_screenManager = new ScreenManager();
        s_screenManager->init();
    }
    return s_screenManager;
}

void ScreenManager::initScreen()
{
    ledMatrix->initMatrix();
    lcdDisplay->initDisplay();
}

void ScreenManager::showDeviceStatus(FunctionList functionList, FunctionStatus functionStatus)
{
    CRGB color = CRGB::Black;
    switch (functionStatus)
    {
    case FS_CHECK_1:
        color = CRGB::Orange;
        break;
    case FS_CHECK_2:
        color = CRGB::Yellow;
        break;
    case FS_CHECK_3:
        color = CRGB::Cyan;
        break;
    case FS_CHECK_4:
        color = CRGB::Blue;
        break;
    case FS_CHECK_5:
        color = CRGB::Purple;
        break;
    case FS_PASS:
        color = CRGB::Green;
        break;
    case FS_FAIL:
        color = CRGB::Red;
        break;
    }
    ledMatrix->setPixel(functionList-1, color);
    ledMatrix->show();
}

void ScreenManager::ledCheck()
{
    while (1)
    {
        ledMatrix->clear();
        Serial.println(ledMatrix->getLedCount());
        for (int i = 0; i < ledMatrix->getLedCount(); i++)
        {
            ledMatrix->setPixel(i, CRGB::Red);
        }
        ledMatrix->show();
        delay(1000);

        ledMatrix->clear();
        for (int i = 0; i < ledMatrix->getLedCount(); i++)
        {
            ledMatrix->setPixel(i, CRGB::Green);
        }
        ledMatrix->show();
        delay(1000);

        ledMatrix->clear();
        for (int i = 0; i < ledMatrix->getLedCount(); i++)
        {
            ledMatrix->setPixel(i, CRGB::Blue);
        }
        ledMatrix->show();
        delay(1000);
    }
}

//=====================LedMatrix==================//

LedMatrix::~LedMatrix()
{
    if (leds)
    {
        free(leds);
    }
}

void LedMatrix::initMatrix()
{
    leds = (CRGB *)malloc(sizeof(CRGB) * this->getLedCount());
    uint32_t ledCount = this->getLedCount();
    FastLED.addLeds<WS2812Controller800Khz, LED_PIN, GRB>(leds, ledCount);
    this->clear();
    this->show();
}

void LedMatrix::setBrightness(uint8_t scale)
{
    FastLED.setBrightness(50 / 100.0 * scale);
}

void LedMatrix::close()
{
    FastLED.clear();
    FastLED.show();
}

void LedMatrix::clear()
{
    FastLED.clear();
}

void LedMatrix::setPixel(int order, struct CRGB color)
{
    FastLED.leds()[order] = color;
}

//=====================LcdDisplay==================//

Arduino_DataBus *bus;
Arduino_GFX *gfx;
static uint32_t screenWidth;
static uint32_t screenHeight;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t *disp_draw_buf;
static lv_disp_drv_t disp_drv;

int8_t panelPointer = 0;

void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);
    gfx->draw16bitBeRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);
    lv_disp_flush_ready(disp);
}

void LcdDisplay::initDisplay()
{
    bus = new Arduino_ESP32SPI(13, 10, 12, 11, -1, HSPI);
    // gfx = new Arduino_GC9A01(bus, 19, 0, true);
    gfx = new Arduino_ST7789(bus, 2, 0, true);
    gfx->begin();
    gfx->fillScreen(BLACK);
    lv_init();
    screenWidth = gfx->width();
    screenHeight = gfx->height();
    disp_draw_buf = (lv_color_t *)malloc(sizeof(lv_color_t) * screenWidth * 150);
    if (!disp_draw_buf)
    {
        Serial.println("LVGL disp_draw_buf allocate failed!");
        return;
    }
    lv_disp_draw_buf_init(&draw_buf, disp_draw_buf, NULL, screenWidth * 150);
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    lv_indev_drv_register(&indev_drv);
    ui_init();

    arrowshake_Animation(ui_img_arrow,0);
    this->setProgress(0);
    this->showArrow(false);
    this->showProgressTitle(false);
    this->showProgressBar(false);
    this->setCheckerVersion(FIRMWARE_VERSION);
    this->setFirmwareVersion("____._._");
    this->setWanMac("__:__:__:__:__:__");
    this->setLanMac("__:__:__:__:__:__");
    this->setSnCode("_____________");
    this->setErrorCode(0);
}

void LcdDisplay::loop()
{
    lv_timer_handler();
    vTaskDelay(5);
}



void LcdDisplay::setCheckerVersion(const String& ver)
{
    String str = "Checker Ver. " + ver;
    lv_label_set_text(ui_lb_version,str.c_str());
}

void LcdDisplay::setFirmwareVersion(const String& ver)
{
    String str = "FW: " + ver;
    lv_label_set_text(ui_lb_firmware,str.c_str());
}

void LcdDisplay::setWanMac(const String& mac)
{
    String str = "WMac: " + mac;
    lv_label_set_text(ui_lb_wan_mac,str.c_str());
}

void LcdDisplay::setLanMac(const String& mac)
{
    String str = "LMac: " + mac;
    lv_label_set_text(ui_lb_lan_mac,str.c_str());
}

void LcdDisplay::setSnCode(const String& sn)
{
    String str = "SN: " + sn;
    lv_label_set_text(ui_lb_sn_code,str.c_str());
}

void LcdDisplay::setErrorCode(uint16_t code)
{
    String str = "Error: " + String(code);
    lv_label_set_text(ui_lb_error_code,str.c_str());
}

void LcdDisplay::setProgress(uint8_t progress)
{
    this->showProgressBar(true);
    lv_bar_set_value(ui_pb_progress,progress,LV_ANIM_ON);
}

void LcdDisplay::setProgressTitle(const String& title,bool showArrow)
{
    this->showProgressTitle(true);
    lv_label_set_text(ui_lb_title,title.c_str());
    this->showArrow(showArrow);
    lv_obj_set_width(ui_lb_title, lv_pct(showArrow?75:92));
}

void LcdDisplay::showArrow(bool e)
{
    if (!e)
    {
        lv_obj_add_flag(ui_img_arrow,LV_OBJ_FLAG_HIDDEN);
    }else
    {
        lv_obj_clear_flag(ui_img_arrow,LV_OBJ_FLAG_HIDDEN);
    }
}

void LcdDisplay::showProgressTitle(bool e)
{
    if (!e)
    {
        lv_obj_add_flag(ui_lb_title,LV_OBJ_FLAG_HIDDEN);
    }else
    {
        lv_obj_clear_flag(ui_lb_title,LV_OBJ_FLAG_HIDDEN);
    }
}

void LcdDisplay::showProgressBar(bool e)
{
    if (!e)
    {
        lv_obj_add_flag(ui_pb_progress,LV_OBJ_FLAG_HIDDEN);
    }else
    {
        lv_obj_clear_flag(ui_pb_progress,LV_OBJ_FLAG_HIDDEN);
    }
}

void LcdDisplay::startCheck()
{
    this->showArrow(true);
    this->showProgressTitle(true);
    this->setProgressTitle("按下检测");
}

void LcdDisplay::waitCheckerConnectNet()
{
    this->showArrow(false);
    this->showProgressTitle(true);
    this->setProgressTitle("等待检测设备联网");
}