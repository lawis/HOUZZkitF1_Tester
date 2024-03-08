#include <Arduino.h>
#include <HardwareSerial.h>
#include <WiFi.h>
#include <WiFiAP.h>
#include "EspUsbHost.h"
// #include "SPIFFS.h"
// #define UNIT_TEST

#ifndef UNIT_TEST

#include "HOUZZkitF1Tester/device/HFTScreen.h"
#include "HOUZZkitF1Tester/device/HFTEncoder.h"
#include "HOUZZkitF1Tester/device/HFTDevice.h"
#include "HOUZZkitF1Tester/connection/HFTConnectionManager.h"

#define WIFI_SSID "HOUZZKitF1_5961"
#define WIFI_PSWD  "88888888"


#define ENCODER_BUTTON_PIN 6

ScreenManager *m_screenManager = nullptr;
HFTEncoder *m_hftEncoder = nullptr;
HFTDevice *m_hftDevice = nullptr;
ConnectionManager *m_connectionManager = nullptr;

QueueHandle_t g_xQueue = nullptr;
xSemaphoreHandle g_xMutex = nullptr;
typedef bool (*xMessageCallback)(const uint8_t *buffer, const uint32_t size);
typedef struct xMESSAGE
{
  xMessageCallback handle;
  uint8_t *buffer;
  uint32_t size;
  xMESSAGE()
      : handle(nullptr), buffer(nullptr), size(0){

                                          };

  xMESSAGE(xMessageCallback h)
      : handle(h), buffer(nullptr), size(0){

                                    };

  xMESSAGE(xMessageCallback h, const uint8_t *b, const uint32_t s)
      : handle(h), buffer(nullptr), size(s)
  {
    if (b)
    {
      buffer = (uint8_t *)malloc(size);
      memcpy(buffer, b, size);
    }
  };

  void freeBuffer()
  {
    if (buffer)
    {
      free(buffer);
      buffer = nullptr;
    }
  };
} xMESSAGE;

#define NET_QUEUE_SEND(msg) xQueueSend(g_xQueue, msg, portMAX_DELAY);
#define NET_QUEUE_RECEIVE(msg) (pdPASS != xQueueReceive(g_xQueue, msg, 0))

#define NET_MUTEX_LOCK() \
  do                     \
  {                      \
  } while (xSemaphoreTake(g_xMutex, portMAX_DELAY) != pdPASS)
#define NET_MUTEX_UNLOCK() xSemaphoreGive(g_xMutex)

void checkFlowTask(void *pvParameters);
void connectTask(void *pvParameters);

void checkFlow_1(uint16_t *errCode)
{
  m_screenManager->showDeviceStatus(FL_SERVER_CONNECT, FS_CHECK_1);
  if (!m_connectionManager->ethernetConn->ethernetReady())
  {
    *errCode = 10101;
    m_screenManager->showDeviceStatus(FL_SERVER_CONNECT, FS_FAIL);
    return;
  }
  if (!m_connectionManager->ethernetConn->serverConnected())
  {
    *errCode = 10102;
    m_screenManager->showDeviceStatus(FL_SERVER_CONNECT, FS_FAIL);
    return;
  }
  m_screenManager->showDeviceStatus(FL_SERVER_CONNECT, FS_PASS);
}

void checkFlow_2(uint16_t *errCode)
{
  m_screenManager->showDeviceStatus(FL_DEVICE_POWER_ON, FS_CHECK_1);
  m_hftDevice->relayOn();
  m_screenManager->showDeviceStatus(FL_DEVICE_POWER_ON, FS_PASS);
}

bool _spiffsInit = false;

void checkFlow_sendFile(const String& filename, uint16_t *errCode,uint8_t err)
{
  if (!_spiffsInit)
  {
      if(!SPIFFS.begin(true))
      {
          *errCode = 20300 + err;
          return;
      }
      _spiffsInit = true;
  }
  File file = SPIFFS.open(filename.c_str());
  if (!file)
  {
      *errCode = 30300 + err;
      return;
  }
  size_t fileSize = file.size();

  String res =  m_connectionManager->serialConn->sendString(FL_SEND_FILE,filename.substring(1)+","+String(fileSize),2);
  if (res != "ready")
  {
      file.close();
      *errCode = 40300 + err;
      return;
  }
  res = m_connectionManager->serialConn->sendFile(FL_SEND_FILE,file,2);
  if (res != "ok")
  {
      file.close();
      *errCode = 50300 + err;
      return;
  }
}

void checkFlow_3(uint16_t *errCode)
{
  m_screenManager->showDeviceStatus(FL_DEVICE_CONNECTED, FS_CHECK_1);
  bool deviceConnected = false;
  uint8_t checkTimes = 90;
  while (!deviceConnected && checkTimes-- > 0)
  {
    // Serial.println("checkFlow 4");
    delay(1000);
    deviceConnected = m_connectionManager->deviceConnected();
    m_screenManager->showDeviceStatus(FL_DEVICE_CONNECTED, FunctionStatus((89 - checkTimes) / 18 + 1));
  }

  if (!deviceConnected)
  {
    *errCode = 10301;
    m_screenManager->showDeviceStatus(FL_DEVICE_CONNECTED, FS_FAIL);
    return;
  }
  // m_screenManager->showDeviceStatus(FL_DEVICE_CONNECTED, FS_PASS);
  m_screenManager->lcdDisplay->setFirmwareVersion(m_connectionManager->firmwareVersion);
  m_screenManager->lcdDisplay->setWanMac(m_connectionManager->eno0Mac);
  m_screenManager->lcdDisplay->setLanMac(m_connectionManager->eno1Mac);
  m_screenManager->lcdDisplay->setSnCode(m_connectionManager->snCode);
  // m_screenManager->lcdDisplay->setNsCode(m_connectionManager->)

  checkFlow_sendFile("/F1Checker.py",errCode ,1);
  if (*errCode != 0)
  {
    m_screenManager->showDeviceStatus(FL_DEVICE_CONNECTED, FS_FAIL);
    return;
  }

  checkFlow_sendFile("/gpio.py",errCode ,2);
  if (*errCode != 0)
  {
    m_screenManager->showDeviceStatus(FL_DEVICE_CONNECTED, FS_FAIL);
    return;
  }

  String res = m_connectionManager->serialConn->sendString(FL_LOAD_PYFILE, "F1Checker.py", 2);
  
  if (res.length() == 0)
  {
    *errCode = 10302;
    m_screenManager->showDeviceStatus(FL_DEVICE_CONNECTED, FS_FAIL);
    return;
  }
  if (res != "ok")
  {
    *errCode = 10303;
    m_screenManager->showDeviceStatus(FL_DEVICE_CONNECTED, FS_FAIL);
    return;
  }
  m_screenManager->showDeviceStatus(FL_DEVICE_CONNECTED, FS_PASS);
}

void checkFlow_4(uint16_t *errCode)
{
  m_screenManager->showDeviceStatus(FL_DEVICE_ACTIVATED, FS_CHECK_1);
  String res = m_connectionManager->serialConn->sendString(FL_DEVICE_ACTIVATED, "device_activated", 2);
  if (res.length() == 0)
  {
    // *errCode = 10401;
    m_screenManager->showDeviceStatus(FL_DEVICE_ACTIVATED, FS_CHECK_3);
    return;
  }
  if (res == "ok")
  {
    m_screenManager->showDeviceStatus(FL_DEVICE_ACTIVATED, FS_PASS);
    return;
  }
  m_screenManager->showDeviceStatus(FL_DEVICE_ACTIVATED, FS_CHECK_3);
  // *errCode = 10402;
}

void checkFlow_5(uint16_t *errCode)
{
  m_screenManager->showDeviceStatus(FL_HOST_USB, FS_CHECK_1);
  String res = m_connectionManager->serialConn->sendString(FL_HOST_USB, "host_usb", 2);
  if (res.length() == 0)
  {
    *errCode = 10501;
    m_screenManager->showDeviceStatus(FL_HOST_USB, FS_FAIL);
    return;
  }
  if (res == "ok")
  {
    m_screenManager->showDeviceStatus(FL_HOST_USB, FS_PASS);
    return;
  }
  m_screenManager->showDeviceStatus(FL_HOST_USB, FS_FAIL);
  *errCode = 10502;
}

void checkFlow_6(uint16_t *errCode)
{
  m_screenManager->showDeviceStatus(FL_DDR, FS_CHECK_1);
  String res = m_connectionManager->serialConn->sendString(FL_DDR, "ddr", 2);
  if (res.length() == 0)
  {
    *errCode = 10601;
    m_screenManager->showDeviceStatus(FL_DDR, FS_FAIL);
    return;
  }
  if (res == "ok")
  {
    m_screenManager->showDeviceStatus(FL_DDR, FS_PASS);
    return;
  }
  m_screenManager->showDeviceStatus(FL_DDR, FS_FAIL);
  *errCode = 10602;
}

void checkFlow_7(uint16_t *errCode)
{
  m_screenManager->showDeviceStatus(FL_EMMC, FS_CHECK_1);
  String res = m_connectionManager->serialConn->sendString(FL_EMMC, "emmc", 15);
  if (res.length() == 0)
  {
    *errCode = 10701;
    m_screenManager->showDeviceStatus(FL_EMMC, FS_FAIL);
    return;
  }
  if (res.toFloat() < 5.0)
  {
    m_screenManager->showDeviceStatus(FL_EMMC, FS_PASS);
    return;
  }
  m_screenManager->showDeviceStatus(FL_EMMC, FS_FAIL);
  *errCode = 10702;
}

void checkFlow_8(uint16_t *errCode)
{
  m_screenManager->showDeviceStatus(FL_RTC, FS_CHECK_1);
  String res = m_connectionManager->serialConn->sendString(FL_RTC, "rtc", 2);
  if (res.length() == 0)
  {
    *errCode = 10801;
    m_screenManager->showDeviceStatus(FL_RTC, FS_FAIL);
    return;
  }
  if (res == "ok")
  {
    m_screenManager->showDeviceStatus(FL_RTC, FS_PASS);
    return;
  }
  m_screenManager->showDeviceStatus(FL_RTC, FS_FAIL);
  *errCode = 10802;
}

void checkFlow_9(uint16_t *errCode)
{
  m_screenManager->showDeviceStatus(FL_IIC_ENCRYPT, FS_CHECK_1);
  String res = m_connectionManager->serialConn->sendString(FL_IIC_ENCRYPT, "iic_encrypt", 2);
  if (res.length() == 0)
  {
    *errCode = 10901;
    m_screenManager->showDeviceStatus(FL_IIC_ENCRYPT, FS_FAIL);
    return;
  }
  if (res == "ok")
  {
    m_screenManager->showDeviceStatus(FL_IIC_ENCRYPT, FS_PASS);
    return;
  }
  m_screenManager->showDeviceStatus(FL_IIC_ENCRYPT, FS_FAIL);
  *errCode = 10902;
}

void rs485CheckFlow(void *pvParameters)
{
  SerialConnection rs485Serial(nullptr);
  rs485Serial.initInner(9, 46);
  String res = rs485Serial.readString(5);
  if (res == "10:ping")
  {
    rs485Serial.sendString(10, "pong");
  }
  vTaskDelete(NULL);
}

void checkFlow_10(uint16_t *errCode)
{
  m_screenManager->showDeviceStatus(FL_RS485, FS_CHECK_1);

  xTaskCreatePinnedToCore(rs485CheckFlow, "rs485CheckFlow", 8192, NULL, 1, NULL, 0);
  String res = m_connectionManager->serialConn->sendString(FL_RS485, "rs485", 5);
  if (res.length() == 0)
  {
    *errCode = 11001;
    m_screenManager->showDeviceStatus(FL_RS485, FS_FAIL);
    return;
  }
  if (res == "ok")
  {
    m_screenManager->showDeviceStatus(FL_RS485, FS_PASS);
    return;
  }
  m_screenManager->showDeviceStatus(FL_RS485, FS_FAIL);
  *errCode = 11002;
}

// //SerialRS485.begin(115200, SERIAL_8N1, 40, 39);   RS232 M1

// //SerialRS485.begin(115200, SERIAL_8N1, 42, 41);   RS232 M2

void rs232M1CheckFlow(void *pvParameters)
{
  SerialConnection rs232Serial(nullptr);
  rs232Serial.initInner(40, 39);
  String res = rs232Serial.readString(10);
  if (res == "11:ping")
  {
    rs232Serial.sendString(11, "pong");
  }
  vTaskDelete(NULL);
}

void rs232M2CheckFlow(void *pvParameters)
{
  SerialConnection rs232Serial(nullptr);
  rs232Serial.initInner(42, 41);
  String res = rs232Serial.readString(10);
  if (res == "11:ping")
  {
    rs232Serial.sendString(11, "pong");
  }
  vTaskDelete(NULL);
}

void checkFlow_11(uint16_t *errCode)
{
  m_screenManager->showDeviceStatus(FL_RS232, FS_CHECK_1);

  xTaskCreatePinnedToCore(rs232M1CheckFlow, "rs232M1CheckFlow", 8192, NULL, 1, NULL, 0);
  delay(500);
  String res = m_connectionManager->serialConn->sendString(FL_RS232, "/dev/ttyS9", 5);
  if (res.length() == 0)
  {
    *errCode = 11101;
    m_screenManager->showDeviceStatus(FL_RS232, FS_FAIL);
    return;
  }
  if (res != "ok")
  {
    *errCode = 11102;
    m_screenManager->showDeviceStatus(FL_RS232, FS_FAIL);
    return;
  }

  m_screenManager->showDeviceStatus(FL_RS232, FS_CHECK_2);
  xTaskCreatePinnedToCore(rs232M2CheckFlow, "rs232M2CheckFlow", 8192, NULL, 1, NULL, 0);
  delay(500);
  res = m_connectionManager->serialConn->sendString(FL_RS232, "/dev/ttyS4", 5);
  if (res.length() == 0)
  {
    *errCode = 11103;
    m_screenManager->showDeviceStatus(FL_RS232, FS_FAIL);
    return;
  }
  if (res != "ok")
  {
    *errCode = 11104;
    m_screenManager->showDeviceStatus(FL_RS232, FS_FAIL);
    return;
  }
  m_screenManager->showDeviceStatus(FL_RS232, FS_PASS);
}

void checkFlow_12(uint16_t *errCode)
{
  m_screenManager->showDeviceStatus(FL_SSD, FS_CHECK_1);
  String res = m_connectionManager->serialConn->sendString(FL_SSD, "ssd", 2);
  if (res.length() == 0)
  {
    *errCode = 11201;
    m_screenManager->showDeviceStatus(FL_SSD, FS_FAIL);
    return;
  }
  if (res == "ok")
  {
    m_screenManager->showDeviceStatus(FL_SSD, FS_PASS);
    return;
  }
  m_screenManager->showDeviceStatus(FL_SSD, FS_FAIL);
  *errCode = 11202;
}

void checkFlow_13(uint16_t *errCode)
{
  m_screenManager->showDeviceStatus(FL_TF, FS_CHECK_1);
  String res = m_connectionManager->serialConn->sendString(FL_TF, "tf", 2);
  if (res.length() == 0)
  {
    *errCode = 11301;
    m_screenManager->showDeviceStatus(FL_TF, FS_FAIL);
    return;
  }
  if (res == "ok")
  {
    m_screenManager->showDeviceStatus(FL_TF, FS_PASS);
    return;
  }
  m_screenManager->showDeviceStatus(FL_TF, FS_FAIL);
  *errCode = 11302;
}

void gpioCheck(uint16_t *errCode, const String &gpio, uint8_t uType, uint8_t pin,uint8_t err)
{
  // Serial.println(gpio);
  delay(500);
  String res = m_connectionManager->serialConn->sendString(FL_GPIO, gpio, 5);
  if (res.length() == 0)
  {
    *errCode = 21400 + err;
    return;
  }
  if (res != "ok")
  {
    *errCode = 31400 + err;
    return;
  }
  uint8_t dr = 0;
  switch (uType)
  {
  case 6:
    dr = m_hftDevice->mcpU6Read(pin);
    break;
  case 7:
    dr = m_hftDevice->mcpU7Read(pin);
    break;
  default:
    *errCode = 41400 + err;
    return;
  }

  // m_connectionManager->serialConn->sendDebug(gpio + " " + String(dr));
  // Serial.println(gpio.substring(3));
  // Serial.println(dr);
  if (gpio.substring(3).toInt() != dr)
  {
    *errCode = 11400 + err;
    return;
  }
}

void checkFlow_14(uint16_t *errCode)
{
  m_screenManager->showDeviceStatus(FL_GPIO, FS_CHECK_1);

  // gpioCheck(errCode, "3A40", 6, MCP23016_PIN_GPIO1_5,1);
  // if (*errCode != 0)
  // {
  //   m_screenManager->showDeviceStatus(FL_GPIO, FS_FAIL);
  //   return;
  // }
  // gpioCheck(errCode, "3A41", 6, MCP23016_PIN_GPIO1_5,2);
  // if (*errCode != 0)
  // {
  //   m_screenManager->showDeviceStatus(FL_GPIO, FS_FAIL);
  //   return;
  // }
  m_screenManager->lcdDisplay->setProgress(41);



  gpioCheck(errCode, "3A50", 6, MCP23016_PIN_GPIO1_6,3);
  if (*errCode != 0)
  {
    m_screenManager->showDeviceStatus(FL_GPIO, FS_FAIL);
    return;
  }
  gpioCheck(errCode, "3A51", 6, MCP23016_PIN_GPIO1_6,4);
  if (*errCode != 0)
  {
    m_screenManager->showDeviceStatus(FL_GPIO, FS_FAIL);
    return;
  }
  m_screenManager->lcdDisplay->setProgress(43);

  gpioCheck(errCode, "3A10", 6, MCP23016_PIN_GPIO0_7,5);
  if (*errCode != 0)
  {
    m_screenManager->showDeviceStatus(FL_GPIO, FS_FAIL);
    return;
  }
  gpioCheck(errCode, "3A11", 6, MCP23016_PIN_GPIO0_7,6);
  if (*errCode != 0)
  {
    m_screenManager->showDeviceStatus(FL_GPIO, FS_FAIL);
    return;
  }
  m_screenManager->lcdDisplay->setProgress(45);

  m_screenManager->showDeviceStatus(FL_GPIO, FS_CHECK_2);

  gpioCheck(errCode, "2D60", 6, MCP23016_PIN_GPIO0_6,7);
  if (*errCode != 0)
  {
    m_screenManager->showDeviceStatus(FL_GPIO, FS_FAIL);
    return;
  }
  gpioCheck(errCode, "2D61", 6, MCP23016_PIN_GPIO0_6,8);
  if (*errCode != 0)
  {
    m_screenManager->showDeviceStatus(FL_GPIO, FS_FAIL);
    return;
  }
  m_screenManager->lcdDisplay->setProgress(47);

  gpioCheck(errCode, "2D70", 6, MCP23016_PIN_GPIO0_4,9);
  if (*errCode != 0)
  {
    m_screenManager->showDeviceStatus(FL_GPIO, FS_FAIL);
    return;
  }
  gpioCheck(errCode, "2D71", 6, MCP23016_PIN_GPIO0_4,10);
  if (*errCode != 0)
  {
    m_screenManager->showDeviceStatus(FL_GPIO, FS_FAIL);
    return;
  }
  m_screenManager->lcdDisplay->setProgress(49);

  gpioCheck(errCode, "3A20", 6, MCP23016_PIN_GPIO0_5,11);
  if (*errCode != 0)
  {
    m_screenManager->showDeviceStatus(FL_GPIO, FS_FAIL);
    return;
  }
  gpioCheck(errCode, "3A21", 6, MCP23016_PIN_GPIO0_5,12);
  if (*errCode != 0)
  {
    m_screenManager->showDeviceStatus(FL_GPIO, FS_FAIL);
    return;
  }
  m_screenManager->lcdDisplay->setProgress(51);

  m_screenManager->showDeviceStatus(FL_GPIO, FS_CHECK_3);

  gpioCheck(errCode, "3A00", 6, MCP23016_PIN_GPIO0_2,13);
  if (*errCode != 0)
  {
    m_screenManager->showDeviceStatus(FL_GPIO, FS_FAIL);
    return;
  }
  gpioCheck(errCode, "3A01", 6, MCP23016_PIN_GPIO0_2,14);
  if (*errCode != 0)
  {
    m_screenManager->showDeviceStatus(FL_GPIO, FS_FAIL);
    return;
  }
  m_screenManager->lcdDisplay->setProgress(53);

  gpioCheck(errCode, "2D50", 6, MCP23016_PIN_GPIO0_3,15);
  if (*errCode != 0)
  {
    m_screenManager->showDeviceStatus(FL_GPIO, FS_FAIL);
    return;
  }
  gpioCheck(errCode, "2D51", 6, MCP23016_PIN_GPIO0_3,16);
  if (*errCode != 0)
  {
    m_screenManager->showDeviceStatus(FL_GPIO, FS_FAIL);
    return;
  }
  m_screenManager->lcdDisplay->setProgress(55);

  gpioCheck(errCode, "2D40", 6, MCP23016_PIN_GPIO0_1,17);
  if (*errCode != 0)
  {
    m_screenManager->showDeviceStatus(FL_GPIO, FS_FAIL);
    return;
  }
  gpioCheck(errCode, "2D41", 6, MCP23016_PIN_GPIO0_1,18);
  if (*errCode != 0)
  {
    m_screenManager->showDeviceStatus(FL_GPIO, FS_FAIL);
    return;
  }
  m_screenManager->lcdDisplay->setProgress(57);

  m_screenManager->showDeviceStatus(FL_GPIO, FS_CHECK_4);

  // gpioCheck(errCode, "2D30", 7, MCP23016_PIN_GPIO1_0,19);
  // if (*errCode != 0)
  // {
  //   m_screenManager->showDeviceStatus(FL_GPIO, FS_FAIL);
  //   return;
  // }
  // gpioCheck(errCode, "2D31", 7, MCP23016_PIN_GPIO1_0,20);
  // if (*errCode != 0)
  // {
  //   m_screenManager->showDeviceStatus(FL_GPIO, FS_FAIL);
  //   return;
  // }
  
  m_screenManager->lcdDisplay->setProgress(59);

  // gpioCheck(errCode, "3C30", 7, MCP23016_PIN_GPIO1_1,21);
  // if (*errCode != 0)
  // {
  //   m_screenManager->showDeviceStatus(FL_GPIO, FS_FAIL);
  //   return;
  // }
  // gpioCheck(errCode, "3C31", 7, MCP23016_PIN_GPIO1_1,22);
  // if (*errCode != 0)
  // {
  //   m_screenManager->showDeviceStatus(FL_GPIO, FS_FAIL);
  //   return;
  // }
  m_screenManager->lcdDisplay->setProgress(61);

  gpioCheck(errCode, "2D20", 7, MCP23016_PIN_GPIO0_6,23);
  if (*errCode != 0)
  {
    m_screenManager->showDeviceStatus(FL_GPIO, FS_FAIL);
    return;
  }
  gpioCheck(errCode, "2D21", 7, MCP23016_PIN_GPIO0_6,24);
  if (*errCode != 0)
  {
    m_screenManager->showDeviceStatus(FL_GPIO, FS_FAIL);
    return;
  }

  m_screenManager->showDeviceStatus(FL_GPIO, FS_PASS);
}

void checkFlow_15(uint16_t *errCode)
{
  m_screenManager->showDeviceStatus(FL_PWM, FS_CHECK_1);
  String res = m_connectionManager->serialConn->sendString(FL_PWM, "pwm", 5);
  if (res.length() == 0)
  {
    *errCode = 11501;
    m_screenManager->showDeviceStatus(FL_PWM, FS_FAIL);
    return;
  }
  if (res == "ok")
  {
    m_screenManager->showDeviceStatus(FL_PWM, FS_PASS);
    return;
  }
  m_screenManager->showDeviceStatus(FL_PWM, FS_FAIL);
  *errCode = 11502;
}

void checkFlow_16(uint16_t *errCode)
{
  m_screenManager->showDeviceStatus(FL_WAN, FS_CHECK_1);
  String res = m_connectionManager->serialConn->sendString(FL_WAN, "wan", 5);
  if (res.length() == 0)
  {
    *errCode = 11601;
    m_screenManager->showDeviceStatus(FL_WAN, FS_FAIL);
    return;
  }
  if (res == "ok")
  {
    m_screenManager->showDeviceStatus(FL_WAN, FS_PASS);
    return;
  }
  m_screenManager->showDeviceStatus(FL_WAN, FS_FAIL);
  *errCode = 11602;
}

void checkFlow_17(uint16_t *errCode)
{
  m_screenManager->showDeviceStatus(FL_LAN, FS_CHECK_1);
  String res = m_connectionManager->serialConn->sendString(FL_LAN, "lan", 5);
  if (res.length() == 0)
  {
    *errCode = 11701;
    m_screenManager->showDeviceStatus(FL_LAN, FS_FAIL);
    return;
  }
  if (res == "ok")
  {
    m_screenManager->showDeviceStatus(FL_LAN, FS_PASS);
    return;
  }
  m_screenManager->showDeviceStatus(FL_LAN, FS_FAIL);
  *errCode = 11702;
}

void checkFlow_18(uint16_t *errCode)
{
  m_screenManager->showDeviceStatus(FL_WIFI, FS_CHECK_1);
  // if (!WiFi.softAP("HOUZZKitF1", "88888888"))
  // {
  //   *errCode = 11801;
  //   return;
  // }
  // delay(2000);
  String res = m_connectionManager->serialConn->sendString(FL_WIFI, String(WIFI_SSID)+","+String(WIFI_PSWD), 30);
  if (res.length() == 0)
  {
    *errCode = 11802;
    m_screenManager->showDeviceStatus(FL_WIFI, FS_FAIL);
    // WiFi.softAPdisconnect(true);
    return;
  }
  if (res.toInt() > 10)
  {
    m_screenManager->showDeviceStatus(FL_WIFI, FS_PASS);
    // WiFi.softAPdisconnect(true);
    return;
  }
  m_screenManager->showDeviceStatus(FL_WIFI, FS_FAIL);
  // WiFi.softAPdisconnect(true);
  *errCode = 11803;
}

void checkFlow_19(uint16_t *errCode)
{
  m_screenManager->showDeviceStatus(FL_BLUETOOTH, FS_CHECK_1);
  String res = m_connectionManager->serialConn->sendString(FL_BLUETOOTH, "bluetooth", 2);
  if (res.length() == 0)
  {
    *errCode = 11901;
    m_screenManager->showDeviceStatus(FL_BLUETOOTH, FS_FAIL);
    return;
  }
  if (res == "ok")
  {
    m_screenManager->showDeviceStatus(FL_BLUETOOTH, FS_PASS);
    return;
  }
  m_screenManager->showDeviceStatus(FL_BLUETOOTH, FS_FAIL);
  *errCode = 11902;
}

void checkFlow_20(uint16_t *errCode)
{
  m_screenManager->showDeviceStatus(FL_ZIGBEE, FS_CHECK_1);
  String res = m_connectionManager->serialConn->sendString(FL_ZIGBEE, "zigbee", 2);
  if (res.length() == 0)
  {
    *errCode = 12001;
    m_screenManager->showDeviceStatus(FL_ZIGBEE, FS_FAIL);
    return;
  }
  if (res == "ok")
  {
    m_screenManager->showDeviceStatus(FL_ZIGBEE, FS_PASS);
    return;
  }
  m_screenManager->showDeviceStatus(FL_ZIGBEE, FS_FAIL);
  *errCode = 12002;
}

void checkFlow_21(uint16_t *errCode)
{
  m_screenManager->showDeviceStatus(FL_HDMI, FS_CHECK_1);
  NET_MUTEX_LOCK();
  m_screenManager->lcdDisplay->setProgressTitle("检测HDMI后按下", true);
  NET_MUTEX_UNLOCK();
  // TODO: 提示检测员肉眼检测HDMI到屏幕的输出
  uint64_t checkStart = millis();
  while (millis() - checkStart < 15 * 1000)
  {
    uint8_t rd = digitalRead(ENCODER_BUTTON_PIN);
    if (rd == 0)
    {
      m_screenManager->showDeviceStatus(FL_HDMI, FS_PASS);
      NET_MUTEX_LOCK();
      m_screenManager->lcdDisplay->setProgressTitle("检测中.....");
      NET_MUTEX_UNLOCK();
      return;
    }
    delay(1);
  }
  m_screenManager->showDeviceStatus(FL_HDMI, FS_FAIL);
  *errCode = 12101;
}

void checkFlow_22(uint16_t *errCode)
{
  m_screenManager->showDeviceStatus(FL_IIC_INTERFACE, FS_CHECK_1);
  String res = m_connectionManager->serialConn->sendString(FL_IIC_INTERFACE, "iic_interface", 2);
  if (res.length() == 0)
  {
    *errCode = 12201;
    m_screenManager->showDeviceStatus(FL_IIC_INTERFACE, FS_FAIL);
    return;
  }
  if (res == "ok")
  {
    m_screenManager->showDeviceStatus(FL_IIC_INTERFACE, FS_PASS);
    return;
  }
  m_screenManager->showDeviceStatus(FL_IIC_INTERFACE, FS_FAIL);
  *errCode = 12202;
}

void checkFlow_23(uint16_t *errCode)
{
  m_screenManager->showDeviceStatus(FL_MICROPHONE, FS_CHECK_1);
  String res = m_connectionManager->serialConn->sendString(FL_MICROPHONE, "microphone", 2);
  if (res.length() == 0)
  {
    *errCode = 12301;
    m_screenManager->showDeviceStatus(FL_MICROPHONE, FS_FAIL);
    return;
  }
  if (res == "ok")
  {
    m_screenManager->showDeviceStatus(FL_MICROPHONE, FS_PASS);
    return;
  }
  m_screenManager->showDeviceStatus(FL_MICROPHONE, FS_FAIL);
  *errCode = 12302;
}

void checkFlow_24(uint16_t *errCode)
{
  m_screenManager->showDeviceStatus(FL_JACKPOT, FS_CHECK_1);
  String res = m_connectionManager->serialConn->sendString(FL_JACKPOT, "jackpot", 2);
  if (res.length() == 0)
  {
    *errCode = 12401;
    m_screenManager->showDeviceStatus(FL_JACKPOT, FS_FAIL);
    return;
  }
  if (res == "ok")
  {
    m_screenManager->showDeviceStatus(FL_JACKPOT, FS_PASS);
    return;
  }
  m_screenManager->showDeviceStatus(FL_JACKPOT, FS_FAIL);
  *errCode = 12402;
}

void checkFlow_25(uint16_t *errCode)
{
  m_screenManager->showDeviceStatus(FL_MIPI, FS_CHECK_1);
  String res = m_connectionManager->serialConn->sendString(FL_MIPI, "mipi", 2);
  if (res.length() == 0)
  {
    *errCode = 12501;
    m_screenManager->showDeviceStatus(FL_MIPI, FS_FAIL);
    return;
  }
  if (res == "ok")
  {
    m_screenManager->showDeviceStatus(FL_MIPI, FS_PASS);
    return;
  }
  m_screenManager->showDeviceStatus(FL_MIPI, FS_FAIL);
  *errCode = 12502;
}

class MyEspUsbHost : public EspUsbHost
{
public:
  String snCode = "";
  uint8_t snCodeRecived = 0;

  void onKeyboardKey(uint8_t ascii, uint8_t keycode, uint8_t modifier)
  {
    if (' ' <= ascii && ascii <= '~')
    {
      // Serial.printf("%c", ascii);
      snCode += (char)ascii;
    }
    else if (ascii == '\r')
    {
      // Serial.println();
      snCodeRecived = 1;
    }
  };
};

void activateDevice(uint16_t *errCode)
{
  // 请求mac1 mac2 的地址
  NET_MUTEX_LOCK();
  m_screenManager->lcdDisplay->setProgressTitle("等待扫SN码");
  m_screenManager->showDeviceStatus(FL_DEVICE_ACTIVATED, FS_CHECK_3);
  NET_MUTEX_UNLOCK();

  MyEspUsbHost usbHost;
  usbHost.begin();
  usbHost.setHIDLocal(HID_LOCAL_Japan_Katakana);

  // 提示扫码枪
  uint64_t checkStart = millis();
  while (millis() - checkStart < 30 * 1000)
  {
    usbHost.task();
    if (usbHost.snCodeRecived)
    {
      NET_MUTEX_LOCK();
      m_screenManager->lcdDisplay->setProgress(97);
      m_screenManager->lcdDisplay->setProgressTitle("扫码成功");
      m_screenManager->lcdDisplay->setSnCode(usbHost.snCode);
      m_screenManager->showDeviceStatus(FL_DEVICE_ACTIVATED, FS_CHECK_4);
      NET_MUTEX_UNLOCK();
      String checkCode;
      bool requestRes = m_connectionManager->ethernetConn->activateDevice(m_connectionManager->eno0Mac, m_connectionManager->eno1Mac, usbHost.snCode, m_connectionManager->verify, checkCode);
      if (requestRes)
      {
        m_screenManager->lcdDisplay->setProgress(99);
        delay(500);
        String res = m_connectionManager->serialConn->sendString(31, usbHost.snCode + "," + checkCode, 2);
        if (res.length() == 0)
        {
          *errCode = 13103;
          NET_MUTEX_LOCK();
          m_screenManager->lcdDisplay->setProgressTitle("激活失败");
          m_screenManager->showDeviceStatus(FL_DEVICE_ACTIVATED, FS_FAIL);
          NET_MUTEX_UNLOCK();
          return;
        }
        if (res == "ok")
        {
          NET_MUTEX_LOCK();
          m_screenManager->lcdDisplay->setProgressTitle("激活成功");
          m_screenManager->showDeviceStatus(FL_DEVICE_ACTIVATED, FS_PASS);
          NET_MUTEX_UNLOCK();
          return;
        }
      }
      else
      {
        NET_MUTEX_LOCK();
        m_screenManager->lcdDisplay->setProgressTitle("激活失败");
        m_screenManager->showDeviceStatus(FL_DEVICE_ACTIVATED, FS_FAIL);
        NET_MUTEX_UNLOCK();
        *errCode = 13101;
      }
      return;
    }
  }
  NET_MUTEX_LOCK();
  m_screenManager->lcdDisplay->setProgressTitle("扫码失败");
  m_screenManager->showDeviceStatus(FL_DEVICE_ACTIVATED, FS_FAIL);
  *errCode = 13102;
}

uint16_t checkFlowAction()
{
  uint16_t errCode = 0;
  bool deviceActivated = false;
  // m_connectionManager->reset();
  // checkFlow_1(&errCode);
  // if (errCode != 0)
  // {
  //   return errCode;
  // }

  // String res = m_connectionManager->serialConn->sendFile("/F1Checker.py",5);
  // Serial.println(res);
  // return 0;

  m_screenManager->lcdDisplay->setProgress(3);
  checkFlow_2(&errCode);
  if (errCode != 0)
  {
    return errCode;
  }
  m_screenManager->lcdDisplay->setProgress(6);
  checkFlow_3(&errCode);
  if (errCode != 0)
  {
    return errCode;
  }
  m_screenManager->lcdDisplay->setProgress(9);
  checkFlow_4(&errCode);
  if (errCode == 0)
  {
    // deviceActivated = true;
  }
  m_screenManager->lcdDisplay->setProgress(12);
  checkFlow_5(&errCode);
  if (errCode != 0)
  {
    return errCode;
  }
  m_screenManager->lcdDisplay->setProgress(15);
  checkFlow_6(&errCode);
  if (errCode != 0)
  {
    return errCode;
  }
  m_screenManager->lcdDisplay->setProgress(18);
  checkFlow_7(&errCode);
  if (errCode != 0)
  {
    return errCode;
  }
  m_screenManager->lcdDisplay->setProgress(21);
  checkFlow_8(&errCode);
  if (errCode != 0)
  {
    return errCode;
  }
  m_screenManager->lcdDisplay->setProgress(24);
  checkFlow_9(&errCode);
  if (errCode != 0)
  {
    return errCode;
  }
  m_screenManager->lcdDisplay->setProgress(27);
  checkFlow_10(&errCode);
  if (errCode != 0)
  {
    return errCode;
  }
  m_screenManager->lcdDisplay->setProgress(30);
  checkFlow_11(&errCode);
  if (errCode != 0)
  {
    return errCode;
  }
    // m_screenManager->lcdDisplay->setProgress(33);
    // checkFlow_12(&errCode);
    // if (errCode != 0)
    // {
    //   return errCode;
    // }
  // m_screenManager->lcdDisplay->setProgress(36);
  // checkFlow_13(&errCode);
  // if (errCode != 0)
  // {
  //   return errCode;
  // }
  m_screenManager->lcdDisplay->setProgress(39);
  checkFlow_14(&errCode);
  if (errCode != 0)
  {
    return errCode;
  }
  m_screenManager->lcdDisplay->setProgress(64);
  checkFlow_15(&errCode);
  if (errCode != 0)
  {
    return errCode;
  }
  m_screenManager->lcdDisplay->setProgress(67);
  checkFlow_16(&errCode);
  if (errCode != 0)
  {
    return errCode;
  }
  m_screenManager->lcdDisplay->setProgress(70);
  checkFlow_17(&errCode);
  if (errCode != 0)
  {
    return errCode;
  }
  m_screenManager->lcdDisplay->setProgress(73);
  checkFlow_18(&errCode);
  if (errCode != 0)
  {
    return errCode;
  }
  m_screenManager->lcdDisplay->setProgress(76);
  checkFlow_19(&errCode);
  if (errCode != 0)
  {
    return errCode;
  }
  m_screenManager->lcdDisplay->setProgress(79);
  checkFlow_20(&errCode);
  if (errCode != 0)
  {
    return errCode;
  }
  m_screenManager->lcdDisplay->setProgress(82);
  checkFlow_21(&errCode);
  if (errCode != 0)
  {
    return errCode;
  }
  m_screenManager->lcdDisplay->setProgress(85);
  checkFlow_22(&errCode);
  if (errCode != 0)
  {
    return errCode;
  }
  m_screenManager->lcdDisplay->setProgress(88);
  checkFlow_23(&errCode);
  if (errCode != 0)
  {
    return errCode;
  }
  m_screenManager->lcdDisplay->setProgress(91);
  checkFlow_24(&errCode);
  if (errCode != 0)
  {
    return errCode;
  }
  m_screenManager->lcdDisplay->setProgress(94);
  checkFlow_25(&errCode);
  if (errCode != 0)
  {
    return errCode;
  }
  m_screenManager->lcdDisplay->setProgress(95);
  if (!deviceActivated)
  {
    activateDevice(&errCode);
    if (errCode != 0)
    {
      return errCode;
    }
  }
  return errCode;
}

bool fullFlowRunning = false;
bool checker_connect_net = false;
bool fullFlowAction(const uint8_t *buffer, const uint32_t size)
{
  fullFlowRunning = true;
  m_screenManager->lcdDisplay->setProgressTitle("检测中.....");
  uint16_t errcode = checkFlowAction();
  if (errcode == 0)
  {
    m_screenManager->lcdDisplay->setProgress(100);
    m_screenManager->lcdDisplay->setProgressTitle("检测成功,按下后取下设备", true);
  }
  else
  {
    // 提示测试失败
    m_screenManager->lcdDisplay->showProgressBar(false);
    m_screenManager->lcdDisplay->setProgressTitle("检测失败,按下后取下设备", true);
    m_screenManager->lcdDisplay->setErrorCode(errcode);
  }

  while (1)
  {
    uint8_t rd = digitalRead(ENCODER_BUTTON_PIN);
    if (rd == 0)
    {
      m_hftDevice->relayOff();
      ESP.restart();
      return true;
    }
    delay(1);
  }
  // 提示可以开始下一台设备的测试
  // m_hftDevice->relayOff();

  fullFlowRunning = false;
  return true;
}

void encoderButtonClick()
{
  // Serial.println("encoderButtonClick");
  if (!fullFlowRunning && checker_connect_net)
  {
    xMESSAGE msg(fullFlowAction);
    NET_QUEUE_SEND(&msg);
  }

  // checkTimes = 60;
  // while (checkTimes-- > 0)
  // {
  //   delay(1000);
  //   m_connectionManager->serialConn->sendString(11, "AT+TEST");
  // }
}

void encoderRotation(RotationType type)
{
  switch (type)
  {
  case RT_LEFT:
    Serial.println("LEFT");
    // m_screenManager->lcdDisplay->left_slide();
    break;
  case RT_RIGHT:
    Serial.println("RIGHT");
    // m_screenManager->lcdDisplay->right_slide();
    break;
  default:
    break;
  }
}

void setup()
{
  //----------------Start Serial Communication----------------
  Serial.begin(115200);

  //----------------Print Hardware Information----------------
  Serial.printf("\n----------------BOOTING INFO----------------\n");
  // Serial.printf("------ESP SDK Version : %s\n",ESP.getSdkVersion());
  // Serial.printf("------Dot2d Version : %s\n",dot2d::dot2dVersion());
  // Serial.printf("------Firmware Version : %s\n",(FirmwareVersion()+"."+FirmwareBuildVersion()).c_str());
  // #if PRINT_DEBUG_INFO
  Serial.println("-----------------ESP INFO----------------");
  Serial.printf("------ESP Chip Version : %d\n", ESP.getChipRevision());
  Serial.printf("------ESP CPU Freq : %d MHz\n", ESP.getCpuFreqMHz());
  Serial.printf("------ESP Flash Chip Mode : %d\n", ESP.getFlashChipMode());
  Serial.printf("------ESP Flash Chip Size : %dM\n", ESP.getFlashChipSize() / 1024 / 1024);
  Serial.printf("------ESP Flash Chip Speed : %dM\n", ESP.getFlashChipSpeed() / 1000 / 1000);
  // Serial.printf("------ESP Firmware Size : %d\n",ESP.getSketchSize());
  // Serial.printf("ESP Firmware MD5 : %s\n",ESP.getSketchMD5().c_str());
  Serial.println("-----------------------------------------");
  // #endif

  // pinMode(38,OUTPUT);
  // digitalWrite(37,HIGH);
  // pinMode(35, OUTPUT);
  // pinMode(36, INPUT_PULLUP);
  // digitalWrite(35, HIGH);
  // digitalWrite(36, HIGH);

  //----------------Initialize Display----------------
  m_screenManager = ScreenManager::getInstance();
  m_screenManager->initScreen();

  // for (size_t i = 0; i < 25; i++)
  // {
  //   m_screenManager->ledMatrix->setPixel(i,CRGB::Green);
  // }
  // m_screenManager->ledMatrix->show();
  // return;

  // m_screenManager->ledCheck();

  m_hftEncoder = new HFTEncoder();
  m_hftEncoder->init(4, 5, 6);
  m_hftEncoder->attachClick(encoderButtonClick);
  m_hftEncoder->attachRotation(encoderRotation);

  m_hftDevice = new HFTDevice();
  m_hftDevice->init();

  WiFi.softAP(WIFI_SSID, WIFI_PSWD);

  g_xMutex = xSemaphoreCreateRecursiveMutex();
  g_xQueue = xQueueCreate(5, sizeof(xMESSAGE));

  // //Start network request thread  //Starting the network on a network thread
  xTaskCreatePinnedToCore(connectTask, "connectTask", 8192, NULL, 1, NULL, 0);
}

void loop()
{
  // Serial.println("loop");
  NET_MUTEX_LOCK();
  m_hftEncoder->tick();
  m_screenManager->lcdDisplay->loop();
  NET_MUTEX_UNLOCK();
  // delay(100);
}

void checkFlowTask(void *pvParameters)
{

  while (1)
  {
    xMESSAGE msg;
    if (!NET_QUEUE_RECEIVE(&msg))
    {
      msg.handle(msg.buffer, msg.size);
      msg.freeBuffer();
    }
    vTaskDelay(10);
  }
  vTaskDelete(NULL);
}

void connectTask(void *pvParameters)
{
  m_connectionManager = ConnectionManager::getInstance();
  m_screenManager->lcdDisplay->setProgressTitle("等待检测工具联网");
  uint16_t errCode = 0;
  checkFlow_1(&errCode);
  m_screenManager->lcdDisplay->setErrorCode(errCode);
  if (errCode == 0)
  {
    checker_connect_net = true;
    m_screenManager->lcdDisplay->setProgressTitle("按下检测", true);
  }
  xTaskCreatePinnedToCore(checkFlowTask, "checkFlowTask", 16384, NULL, 1, NULL, 0);
  while (1)
  {
    // Serial.println("workFlowLoop");
    m_connectionManager->loop();
    vTaskDelay(10);
  }
  vTaskDelete(NULL);
}

#else

// #include "SoftwareSerial.h"

// SoftwareSerial swSer(21, 14, false, 256);

// void setup()
// {
//   swSer.begin(115200);
//   Serial.begin(115200);
//   // swSer.listen();
// }

// void loop()
// {
//   // swSer.println("Hello Software Serial");
//   while (swSer.available() > 0) {
//     Serial.write(swSer.read());
//   }
//   while (Serial.available() > 0) {
//     char a = Serial.read();
//     swSer.write(a);
//     Serial.write(a);
//   }
//   // Serial.println(swSer.isListening()?"yes":"no");
//   // delay(1000);
// }

// #include <HardwareSerial.h>

// // SerialRS485.begin(115200, SERIAL_8N1, 9, 46);  //RS485

// //SerialRS485.begin(115200, SERIAL_8N1, 40, 39);   RS232 M1

// //SerialRS485.begin(115200, SERIAL_8N1, 42, 41);   RS232 M2

// HardwareSerial SerialRS485(1); // use UART1

// // void setup()
// // {
// //   pinMode(37,INPUT);
// //   pinMode(38,INPUT);
// // }

// // void loop()
// // {

// // }

// void setup()
// {
//   // SerialRS485.begin(115200, SERIAL_8N1, 9, 46);
//   SerialRS485.begin(9600, SERIAL_8N1, 42, 41);
//   Serial.begin(115200);
//   // pinMode(8,OUTPUT);
//   // swSer.listen();
// }

// void loop()
// {
//   // swSer.println("Hello Software Serial");
//   while (SerialRS485.available() > 0) {
//     Serial.write(SerialRS485.read());
//   }
//   while (Serial.available() > 0) {
//     char a = Serial.read();
//     SerialRS485.write(a);
//     Serial.write(a);
//   }
//   // Serial.println(swSer.isListening()?"yes":"no");
//   // delay(1000);
//   // digitalWrite(8,HIGH);
//   // delay(5000);
//   // digitalWrite(8,LOW);
//   // delay(5000);
// }

/*
  Web client

 This sketch connects to a website (http://www.google.com)
 using an Arduino Wiznet Ethernet shield.

 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13

 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe, based on work by Adrian McEwen

 */

// #include <SPI.h>
// #include <Ethernet2.h>

// // Enter a MAC address for your controller below.
// // Newer Ethernet shields have a MAC address printed on a sticker on the shield
// byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
// // if you don't want to use DNS (and reduce your sketch size)
// // use the numeric IP instead of the name for the server:
// //IPAddress server(74,125,232,128);  // numeric IP for Google (no DNS)
// char server[] = "www.taobao.com";    // name address for Google (using DNS)

// // Set the static IP address to use if the DHCP fails to assign
// IPAddress ip(192, 168, 13, 177);

// // Initialize the Ethernet client library
// // with the IP address and port of the server
// // that you want to connect to (port 80 is default for HTTP):
// EthernetClient client;

// void setup() {
//   // Open serial communications and wait for port to open:
//   Serial.begin(115200);
//   while (!Serial) {
//     ; // wait for serial port to connect. Needed for Leonardo only
//   }
//   Ethernet.init(45);

//   // start the Ethernet connection:
//   if (Ethernet.begin(mac) == 0) {
//     Serial.println("Failed to configure Ethernet using DHCP");
//     // no point in carrying on, so do nothing forevermore:
//     // try to congifure using IP address instead of DHCP:
//     Ethernet.begin(mac, ip);
//   }
//   // give the Ethernet shield a second to initialize:
//   delay(1000);
//   Serial.println("connecting...");

//   // if you get a connection, report back via serial:
//   if (client.connect(server, 80)) {
//     Serial.println("connected");
//     // Make a HTTP request:
//     client.println("GET /search?q=arduino HTTP/1.1");
//     client.println("Host: www.baidu.com");
//     client.println("Connection: close");
//     client.println();
//   }
//   else {
//     // kf you didn't get a connection to the server:
//     Serial.println("connection failed");
//   }
// }

// void loop()
// {
//   // if there are incoming bytes available
//   // from the server, read them and print them:
//   if (client.available()) {
//     char c = client.read();
//     Serial.print(c);
//   }

//   // if the server's disconnected, stop the client:
//   if (!client.connected()) {
//     Serial.println();
//     Serial.println("disconnecting.");
//     client.stop();

//     // do nothing forevermore:
//     while (true);
//   }
// }
// #include "EspUsbHost.h"

// class MyEspUsbHost : public EspUsbHost
// {
//   void onKeyboardKey(uint8_t ascii, uint8_t keycode, uint8_t modifier)
//   {
//     if (' ' <= ascii && ascii <= '~')
//     {
//       Serial.printf("%c", ascii);
//     }
//     else if (ascii == '\r')
//     {
//       Serial.println();
//     }
//   };
// };

// MyEspUsbHost usbHost;

// void setup()
// {
//   Serial.begin(115200);
//   delay(500);

//   usbHost.begin();
//   usbHost.setHIDLocal(HID_LOCAL_Japan_Katakana);
// }

// void loop()
// {
//   usbHost.task();
// }

// #include <Wire.h>

// void setup(){
//   Wire.begin(17,18);
//   Serial.begin(115200);
//   Serial.println("\nI2C Scanner");
// }
// void loop(){
//   byte error, address;
//   int nDevices;
//   Serial.println("Scanning...");
//   nDevices = 0;
//   for (address = 1; address < 127; address++ ){
//     // The i2c_scanner uses the return value of
//     // the Write.endTransmisstion to see if
//     // a device did acknowledge to the address.
//     Wire.beginTransmission(address);
//     error = Wire.endTransmission();
//     if (error == 0){
//       Serial.print("I2C device found at address 0x");
//       if (address < 16)
//         Serial.print("0");
//       Serial.print(address, HEX);
//       Serial.println(" !");
//       nDevices++;
//     }else if (error == 4){
//       Serial.print("Unknow error at address 0x");
//       if (address < 16)
//         Serial.print("0");
//       Serial.println(address, HEX);
//     }
//   }
//   if (nDevices == 0)
//     Serial.println("No I2C devices found\n");
//   else
//     Serial.println("done\n");
//   delay(5000); // wait 5 seconds for next scan
// }

/**
 * Basic input/output test for MCP23016 expander.
 */

// #include "CyMCP23016.h"

// // CyMCP23016 mcpU6;
// CyMCP23016 mcpU7;

// void setup()
// {
//   Serial.begin(115200);

//   // mcpU6.begin(17, 18, 0x26);

//   mcpU7.begin(17, 18, 0x27);

//   // // Set Pin 0 on Port 0 as an output.
//   // mcpU6.pinMode(MCP23016_PIN_GPIO1_5, INPUT);
//   // WiFi.softAP("HOUZZKitF1", "88888888");
//   mcpU7.pinMode(MCP23016_PIN_GPIO1_2, INPUT);
// }

// void loop()
// {
//   delay(400);
//   uint8_t val;
//   // Set the pin HIGH and read back the state.
//   // mcp.digitalWrite(MCP23016_PIN_GPIO1_0, HIGH);
//   // val = mcpU6.digitalRead(MCP23016_PIN_GPIO1_5);
//   // Serial.print(F("U6 1.5 is "));
//   // Serial.println(val == HIGH ? "HIGH" : "LOW");

//   val = mcpU7.digitalRead(MCP23016_PIN_GPIO1_2);
//   Serial.print(F("U7 1.2 is "));
//   Serial.println(val == HIGH ? "HIGH" : "LOW");

//   // delay(1000);

//   // // Set the pin LOW and read back the state.
//   // mcp.digitalWrite(MCP23016_PIN_GPIO1_0, LOW);
//   // val = mcp.digitalRead(MCP23016_PIN_GPIO1_0);
//   // Serial.print(F("Pin 0.0 is "));
//   // Serial.println(val == HIGH ? "HIGH" : "LOW");
// }

#include "SPIFFS.h"

void setup() 
{
  Serial.begin(115200);
  if(!SPIFFS.begin(true))
  {
    Serial.println("Start SPIFFS Error!");
    return;
  }

  File file = SPIFFS.open("/F1Checker.py");
  if (!file)
  {
    Serial.println("Open file Error!");
    return;
  }else
  {
    Serial.printf("文件大小：%ld  文件内容是：\n",file.size());
    while (file.available())
    {
      Serial.print((char)file.read());
    }
  }
  Serial.println("Open file Success!");
    //打印SPIFFS文件系统信息
  Serial.printf("SPIFFS文件系统总大小是： %d （字节）\n", SPIFFS.totalBytes());
  Serial.printf("SPIFFS文件系统已用大小是： %d （字节）\n", SPIFFS.usedBytes());
}

void loop()
{

}

#endif