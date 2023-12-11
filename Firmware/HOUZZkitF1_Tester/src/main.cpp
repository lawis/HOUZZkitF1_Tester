#include <Arduino.h>

// #define UNIT_TEST

#ifndef UNIT_TEST

#include "Dot2D/dot2d.h"
#include "HOUZZkitTester/device/SDTScreen.h"
#include "HOUZZkitTester/SDTSystem.h"
#include "HOUZZkitTester/SDTConfig.h"
#include <ESP_DoubleResetDetector.h> 

//Build version number, increase by 1 automatically each time you compile
#define FIRMWARE_BUILD_VERSION 29

//Build date and time, set to the time when editing starts each time you compile
#define FIRMWARE_BUILD_DATETIME "3872b"

String FirmwareBuildVersion()
{
    return FIRMWARE_BUILD_DATETIME + String(FIRMWARE_BUILD_VERSION,HEX);
}

dot2d::Director* director = nullptr;

SDTSystem* m_sdtSystem = nullptr;

LedScreenManager* m_screenManager = nullptr;

DoubleResetDetector* m_doubleResetDetector = nullptr;

class MainDelegate : public dot2d::DirectorDelegate
{

    void _render()
    {
        
        AppManager::getInstance()->drawTempStatus();
        m_screenManager->show();
        // Serial.printf("-----Free Heap Mem : %d [%.2f%%]-----\n",
        //         ESP.getFreeHeap(),
        //         ESP.getFreeHeap()/(double)ESP.getHeapSize()*100);
        // Serial.printf("-----Free PSRAM Mem: %d [%.2f%%]-----\n",
        //         ESP.getFreePsram(),
        //         ESP.getFreePsram()/(double)ESP.getPsramSize()*100);
    }

    size_t write(uint8_t c){return Serial.write(c);}

};

void network(void *pvParameters);

void setup() 
{
  //----------------Start Serial Communication----------------
  Serial.begin(115200);

  //----------------Print Hardware Information----------------
  Serial.printf("\n----------------BOOTING INFO----------------\n");
  Serial.printf("------ESP SDK Version : %s\n",ESP.getSdkVersion());
  Serial.printf("------Dot2d Version : %s\n",dot2d::dot2dVersion());
  Serial.printf("------Firmware Version : %s\n",(FirmwareVersion()+"."+FirmwareBuildVersion()).c_str());
#if PRINT_DEBUG_INFO
  Serial.println("-----------------ESP INFO----------------");
  Serial.printf("------ESP Chip Version : %d\n",ESP.getChipRevision());
  Serial.printf("------ESP CPU Freq : %d MHz\n",ESP.getCpuFreqMHz());
  Serial.printf("------ESP Flash Chip Mode : %d\n",ESP.getFlashChipMode());
  Serial.printf("------ESP Flash Chip Size : %dM\n",ESP.getFlashChipSize()/1024/1024);
  Serial.printf("------ESP Flash Chip Speed : %dM\n",ESP.getFlashChipSpeed()/1000/1000);
  // Serial.printf("------ESP Firmware Size : %d\n",ESP.getSketchSize());
  // Serial.printf("ESP Firmware MD5 : %s\n",ESP.getSketchMD5().c_str());
  Serial.println("-----------------------------------------");
#endif

  //----------------Initialize All System Data----------------
  m_sdtSystem = SDTSystem::getInstance();
  if(!m_sdtSystem->initSystemData())
  {
    Serial.printf("---ERROR!!!---System Data Init Failed.\n");
    return;
  }else
  {
    Serial.printf("------Device Model : %s\n",m_sdtSystem->deviceModel().c_str());
    Serial.printf("------Device UDID : %s\n",m_sdtSystem->UDID().c_str());
  }

  //----------------Initialize Display----------------
  m_screenManager = LedScreenManager::getInstance();
  m_screenManager->initScreen();
  dot2d::Size matrixSize = m_screenManager->getMatrixSize();

  //----------------Initialize Dot2d Engine And Render Canvas----------------
  director = dot2d::Director::getInstance();                      //Obtain director object
  director->setDelegate(new MainDelegate());                      //Set up director delegate
  director->setFramesPerSecond(30);                               //Set up farame rate
  Serial.printf("------Dot2d Director init succeed.\n");
  director->initDotCanvas(matrixSize.width,matrixSize.height);
  m_screenManager->initMatrix(director->getDotCanvas());
  m_screenManager->setBrightness(m_sdtSystem->brightness());
  Serial.printf("------Screen And Canvas init succeed.\n");

  //----------------Initialize APP management class----------------
  if(m_sdtSystem->initAppManager())
  {
    Serial.printf("------App Manager init succeed.\n");
  }else
  {
    Serial.printf("---ERROR!!!---App Manager init failed.\n");
  }

  //----------------Initialize message queues in network management classes----------------
  m_sdtSystem->initNetwork();


#if RDR_ENABLE
  //----------------Enable reset logic detection----------------
  m_doubleResetDetector = new DoubleResetDetector(DRD_TIMEOUT, DRD_ADDRESS);
#endif

  //----------------Enable reset logic detection---------------------
  if (m_sdtSystem->newDevice())
  {
    DEVICE.initBtChip();
    m_sdtSystem->setNewDevice(false);
    m_sdtSystem->saveDeviceData();
  }else if(m_sdtSystem->uninitDevice())
  {
    //Play the first time entering device animation
    
  }
  else
  {
#if RDR_ENABLE
    //----------------Check if the system is reset----------------  
    if (m_doubleResetDetector->detectDoubleReset()) 
    {
      Serial.printf("------Detect Double Reset Viable.\n");
      pinMode(12,INPUT_PULLUP);
      if (digitalRead(12) == LOW)
      {
        m_sdtSystem->resetSystemToFactoryState();
      }else
      {
        m_sdtSystem->resetSystem();
      }
      return;
    }else
    {
      Serial.printf("------Detect Double Reset Passed.\n");
    }
#endif
  }
  //----------------Initialize Device Management Class---------------- 
  if(m_sdtSystem->initDevice())
  {
    Serial.printf("------Device init succeed.\n");
  }else
  {
    Serial.printf("---ERROR!!!---Device init failed.\n");
  }

  // //Start network request thread  //Starting the network on a network thread
  xTaskCreatePinnedToCore(network,"network",16384,NULL,1,NULL,0);

}

void network(void *pvParameters)
{
  //----------------Initialize WIFI system----------------
  if(m_sdtSystem->initWifiSystem())
  {
    Serial.printf("------WIFI SYSTEM init succeed.\n");
  }else
  {
    Serial.printf("---ERROR!!!---WIFI SYSTEM init failed.\n");
  }

  SDTSystem::SystemState state = m_sdtSystem->resetState();

  if(state == SDTSystem::SystemState::INIT)
  {
    Serial.printf("------System Is Waiting Connecting.\n");
    //----------------Initialize BLE system----------------
    if(m_sdtSystem->initBleSystem())
    {
      Serial.printf("------BLE SYSTEM init succeed.\n");
      // DEVICE.btStartBuzzer();
    }else
    {
      Serial.printf("---ERROR!!!---BLE SYSTEM init failed.\n");
    }
    xMESSAGE msg(&AppManager::mtfSystemInitAction);
    NET_QUEUE_SEND(&msg);
  }else if(state == SDTSystem::SystemState::NORMAL)
  {
    if(m_sdtSystem->initWebSocket())
    {
      Serial.printf("------Websocket init succeed.\n");
    }else
    {
      Serial.printf("---ERROR!!!---Websocket init failed.\n");
    }
    
    if(m_sdtSystem->initMqttSystem())
    {
      Serial.printf("------MQTT SYSTEM init succeed.\n");
    }else
    {
      Serial.printf("---ERROR!!!---MQTT SYSTEM init failed.\n");
    }
    Serial.printf("------System Is Working.\n");

    m_sdtSystem->startHeartbeat();
    xMESSAGE msg(&AppManager::mtfSystemNormalAction);
    NET_QUEUE_SEND(&msg);// _appManager->initRequestList(); _appManager->playAppList();
  }

  while (1)
  {
    m_sdtSystem->networkLoop();
    vTaskDelay(10);
  }

  vTaskDelete(NULL);
}

void loop() 
{
  vTaskDelay(10);
#if RDR_ENABLE
    m_doubleResetDetector->loop();
#endif
  m_sdtSystem->loop();
  NET_MUTEX_LOCK();
  director->mainLoop();
  NET_MUTEX_UNLOCK();
}


#else

void unitTest()
{
  Serial.printf("-----Free Heap Mem : %d [%.2f%%]-----\n",
                  ESP.getFreeHeap(),
                  ESP.getFreeHeap()/(double)ESP.getHeapSize()*100);

}


#include "FastLED.h"
// #include "WiFi.h"

// static const char *ssid     = "DLINK-Y9C6";  // your network SSID (name of wifi network)
// static const char *password = "yc18610681168"; // your network password

#define LED_PIN             32
#define LED_WIDTH           32
#define LED_HEIGHT          8
#define LED_NUM             256

#define GIF_IMG             "/GIF_IMG"

CRGB data[LED_NUM];

uint16_t* _matrixIndex;

uint16_t XY(uint16_t x, uint16_t y)
{
    return _matrixIndex[y * LED_WIDTH + x];
}

void drawDot(int32_t x,int32_t y,const CRGB& c)
{
  if (x>=LED_WIDTH || x<0 || y>=LED_HEIGHT || y<0)
  {
    return;
  }
  data[XY(x,y)] = c;
}

struct LED_PWM_Def
{
  int freq = 500;
  int resolution = 8;
  int ledChannel;
  int ledPin;
  LED_PWM_Def(int c, int p) :ledChannel(c), ledPin(p){};
};

LED_PWM_Def LEDB = {1,15};

void LED_PWM_Init(LED_PWM_Def led)
{
  ledcSetup(led.ledChannel, led.freq , led.resolution);
  ledcAttachPin(led.ledPin, led.ledChannel);
}

#define BUZZER_PIN 15
#define BUZZER_CHANNEL 1
void Buzzer(void *pvParameters)
{
  for (size_t i = 0; i < 10; i++)
  {
      ledcAttachPin(BUZZER_PIN, BUZZER_CHANNEL);
      ledcWriteNote(BUZZER_CHANNEL, NOTE_B, 4);
      delay(200);
      ledcWriteNote(BUZZER_CHANNEL, NOTE_B, 4);
      delay(200);
      ledcDetachPin(BUZZER_PIN);
      delay(600);
      // if (i==5)
      // {
      //   vTaskDelete(nullptr);
      // }
      
  }
  vTaskDelete(NULL);
}


void setup(){

  delay(1000);

  Serial.begin(115200);

  // FastLED.setBrightness(50);

  // FastLED.addLeds<WS2812Controller800Khz,LED_PIN, GRB>((CRGB* )data,LED_NUM);
  //   // Serial.print("Attempting to connect to SSID: ");
  //   // WiFi.begin(ssid, password);
  // _matrixIndex = (uint16_t* )malloc(sizeof(uint16_t) * LED_NUM);
  // for (uint8_t y = 0; y < LED_HEIGHT; y++)
  // {
  //     for (uint8_t x = 0; x < LED_WIDTH; x++)
  //     {
  //         uint16_t order = y*LED_WIDTH;
  //         if (y%2==0)
  //         {
  //             order+=x;
  //         }else
  //         {
  //             order+=(31-x);
  //         }
  //         _matrixIndex[y * LED_WIDTH + x] = order;
  //     }
  // }
  
  // LED_PWM_Init(LEDB);

  // ledcWrite(LEDB.ledChannel,250);

  xTaskCreatePinnedToCore(Buzzer,"Buzzer",8192,NULL,1,NULL,0);

  
}



void loop() {
  // ledcAttachPin(BUZZER_PIN, BUZZER_CHANNEL);
  // ledcWriteNote(BUZZER_CHANNEL, NOTE_C, 4);
  // delay(500);
  // ledcWriteNote(BUZZER_CHANNEL, NOTE_D, 4);
  // delay(500);
  // ledcWriteNote(BUZZER_CHANNEL, NOTE_E, 4);
  // delay(500);
  // ledcWriteNote(BUZZER_CHANNEL, NOTE_F, 4);
  // delay(500);
  // ledcWriteNote(BUZZER_CHANNEL, NOTE_G, 4);
  // delay(500);
  // ledcWriteNote(BUZZER_CHANNEL, NOTE_A, 4);
  // delay(500);
  // ledcWriteNote(BUZZER_CHANNEL, NOTE_B, 4);
  // delay(500);
  // ledcDetachPin(BUZZER_PIN);
  delay(2000);
}

#endif