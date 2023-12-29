#include "HFTScreen.h"

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
    ledMatrix->setPixel(functionList, color);
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
lv_obj_t *panelArr[PANEL_COUNT];
lv_obj_t *rollerArr[PANEL_COUNT];

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

    panelArr[0] = ui_panelNormal;
    panelArr[1] = ui_panelSetting1;
    panelArr[2] = ui_panelSetting2;
    panelArr[3] = ui_panelSetting3;
    panelArr[4] = ui_panelSetting4;
    panelArr[5] = ui_panelSetting5;

    rollerArr[0] = nullptr;
    rollerArr[1] = ui_roller1;
    rollerArr[2] = ui_roller2;
    rollerArr[3] = ui_roller3;
    rollerArr[4] = ui_roller4;
    rollerArr[5] = ui_roller5;
}

void LcdDisplay::loop()
{
    lv_timer_handler();
    vTaskDelay(5);
}

void lv_panel_slide_right_value(lv_obj_t *obj, int16_t value)
{
    lv_obj_t *scrObj = panelArr[panelPointer + 1];
    lv_obj_t *desObj = panelArr[panelPointer];
    lv_obj_set_x(scrObj, value);
    lv_obj_set_x(desObj, value - 240);
    // Serial.printf("value :%d\n",value);
}

void LcdDisplay::right_slide()
{
    // right_slide_ani();
    if (panelPointer > 0)
    {
        lv_obj_t *scrObj = panelArr[panelPointer];
        lv_obj_t *desObj = panelArr[panelPointer - 1];
        if (!scrObj)
        {
            Serial.printf("scrObj is Null\n");
            return;
        }
        if (!desObj)
        {
            Serial.printf("desObj is Null\n");
            return;
        }

        lv_obj_clear_flag(scrObj, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(desObj, LV_OBJ_FLAG_HIDDEN);
        lv_obj_set_x(scrObj, 0);
        lv_obj_set_x(desObj, -240);
        lv_anim_t a;
        lv_anim_init(&a);
        lv_anim_set_var(&a, panelArr[panelPointer--]);
        lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_panel_slide_right_value);
        lv_anim_set_values(&a, 0, 240);
        lv_anim_set_time(&a, 500);
        lv_anim_start(&a);
    }
}

void lv_panel_slide_left_value(lv_obj_t *obj, int16_t value)
{
    lv_obj_t *scrObj = panelArr[panelPointer - 1];
    lv_obj_t *desObj = panelArr[panelPointer];
    lv_obj_set_x(scrObj, -value);
    lv_obj_set_x(desObj, 240 - value);
    // Serial.printf("value :%d\n",value);
}

void LcdDisplay::left_slide()
{
    if (panelPointer < PANEL_COUNT - 1)
    {
        lv_obj_t *scrObj = panelArr[panelPointer];
        lv_obj_t *desObj = panelArr[panelPointer + 1];
        if (!scrObj)
        {
            Serial.printf("scrObj is Null\n");
            return;
        }
        if (!desObj)
        {
            Serial.printf("desObj is Null\n");
            return;
        }

        lv_obj_clear_flag(scrObj, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(desObj, LV_OBJ_FLAG_HIDDEN);
        lv_obj_set_x(scrObj, 0);
        lv_obj_set_x(desObj, 240);
        lv_anim_t a;
        lv_anim_init(&a);
        lv_anim_set_var(&a, panelArr[panelPointer++]);
        lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_panel_slide_left_value);
        lv_anim_set_values(&a, 0, 240);
        lv_anim_set_time(&a, 500);
        lv_anim_start(&a);
    }
}