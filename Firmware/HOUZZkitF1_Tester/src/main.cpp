#include <Arduino.h>

// #define UNIT_TEST

#ifndef UNIT_TEST

#include "HOUZZkitF1Tester/device/HFTScreen.h"
#include "HOUZZkitF1Tester/device/HFTEncoder.h"
#include "HOUZZkitF1Tester/connection/HFTConnectionManager.h"

ScreenManager *m_screenManager = nullptr;
HFTEncoder *m_hftEncoder = nullptr;
ConnectionManager *m_connectionManager = nullptr;

void workFlow(void *pvParameters);

void encoderButtonClick()
{
  Serial.println("encoderButtonClick");
  

  Serial.println(m_connectionManager->ethernetConn->connected()?"connected" : "not connected");
  

  // Serial.println(res);
}

void encoderRotation(RotationType type)
{
  switch (type)
  {
  case RT_LEFT:
    Serial.println("LEFT");
    m_screenManager->lcdDisplay->left_slide();
    break;
  case RT_RIGHT:
    Serial.println("RIGHT");
    m_screenManager->lcdDisplay->right_slide();
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

  //----------------Initialize Display----------------
  m_screenManager = ScreenManager::getInstance();
  m_screenManager->initScreen();

  m_hftEncoder = new HFTEncoder();
  m_hftEncoder->init(4, 5, 6);
  m_hftEncoder->attachClick(encoderButtonClick);
  m_hftEncoder->attachRotation(encoderRotation);

  // //Start network request thread  //Starting the network on a network thread
  xTaskCreatePinnedToCore(workFlow, "workFlow", 16384, NULL, 1, NULL, 0);
}

void workFlow(void *pvParameters)
{
  m_connectionManager = ConnectionManager::getInstance();
  while (1)
  {
    // Serial.println("workFlowLoop");
    m_connectionManager->loop();
    vTaskDelay(10);
  }
  vTaskDelete(NULL);
}

// void network(void *pvParameters)
// {
//   //----------------Initialize WIFI system----------------
//   if(m_sdtSystem->initWifiSystem())
//   {
//     Serial.printf("------WIFI SYSTEM init succeed.\n");
//   }else
//   {
//     Serial.printf("---ERROR!!!---WIFI SYSTEM init failed.\n");
//   }

//   SDTSystem::SystemState state = m_sdtSystem->resetState();

//   if(state == SDTSystem::SystemState::INIT)
//   {
//     Serial.printf("------System Is Waiting Connecting.\n");
//     //----------------Initialize BLE system----------------
//     if(m_sdtSystem->initBleSystem())
//     {
//       Serial.printf("------BLE SYSTEM init succeed.\n");
//       // DEVICE.btStartBuzzer();
//     }else
//     {
//       Serial.printf("---ERROR!!!---BLE SYSTEM init failed.\n");
//     }
//     xMESSAGE msg(&AppManager::mtfSystemInitAction);
//     NET_QUEUE_SEND(&msg);
//   }else if(state == SDTSystem::SystemState::NORMAL)
//   {
//     if(m_sdtSystem->initWebSocket())
//     {
//       Serial.printf("------Websocket init succeed.\n");
//     }else
//     {
//       Serial.printf("---ERROR!!!---Websocket init failed.\n");
//     }

//     if(m_sdtSystem->initMqttSystem())
//     {
//       Serial.printf("------MQTT SYSTEM init succeed.\n");
//     }else
//     {
//       Serial.printf("---ERROR!!!---MQTT SYSTEM init failed.\n");
//     }
//     Serial.printf("------System Is Working.\n");

//     m_sdtSystem->startHeartbeat();
//     xMESSAGE msg(&AppManager::mtfSystemNormalAction);
//     NET_QUEUE_SEND(&msg);// _appManager->initRequestList(); _appManager->playAppList();
//   }

//   while (1)
//   {
//     m_sdtSystem->networkLoop();
//     vTaskDelay(10);
//   }

//   vTaskDelete(NULL);
// }

void loop()
{
  // Serial.println("loop");
  m_hftEncoder->tick();
  m_screenManager->lcdDisplay->loop();
  // delay(100);
}

#else

#include <SPI.h>
#include "LCD_Driver.h"
#include "GUI_Paint.h"
#include "image.h"

#include <SPI.h>
#include <Ethernet2.h>

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
// IPAddress server(74,125,232,128);  // numeric IP for Google (no DNS)
char server[] = "www.taobao.com"; // name address for Google (using DNS)

// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192, 168, 13, 177);

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;

void setup()
{
  Config_Init();
  LCD_Init();

  LCD_SetBacklight(100);

  // Serial.begin(115200);

  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  Ethernet.init(45);

  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0)
  {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.println("connecting...");

  // if you get a connection, report back via serial:
  if (client.connect(server, 80))
  {
    Serial.println("connected");
    // Make a HTTP request:
    client.println("GET /search?q=arduino HTTP/1.1");
    client.println("Host: www.baidu.com");
    client.println("Connection: close");
    client.println();
  }
  else
  {
    // kf you didn't get a connection to the server:
    Serial.println("connection failed");
  }
}

void loop()
{
  // #if 1
  //     Paint_NewImage(LCD_WIDTH, LCD_HEIGHT, 90, WHITE);
  //     Paint_Clear(WHITE);

  //     // Paint_DrawString_EN(30, 10, "123", &Font24, YELLOW, RED);
  //     // Paint_DrawString_EN(30, 34, "ABC", &Font24, BLUE, CYAN);

  //     // Paint_DrawString_CN(235,10, "微", &Font24CN, WHITE, RED);
  //     // Paint_DrawString_CN(235,40, "雪", &Font24CN, WHITE, RED);
  //     // Paint_DrawString_CN(235,70, "电", &Font24CN, WHITE, RED);
  //     // Paint_DrawString_CN(235,100, "子", &Font24CN, WHITE, RED);

  //     Paint_DrawRectangle(125, 10, 225, 58, RED,DOT_PIXEL_2X2, DRAW_FILL_EMPTY);
  //     Paint_DrawLine(125, 10, 225, 58, MAGENTA, DOT_PIXEL_2X2, LINE_STYLE_SOLID);
  //     Paint_DrawLine(225, 10, 125, 58, MAGENTA, DOT_PIXEL_2X2, LINE_STYLE_SOLID);
  //     Paint_DrawCircle(150, 100, 25, BLUE, DOT_PIXEL_2X2, DRAW_FILL_EMPTY);
  //     Paint_DrawCircle(180, 100, 25, BLACK, DOT_PIXEL_2X2, DRAW_FILL_EMPTY);
  //     Paint_DrawCircle(210, 100, 25, RED, DOT_PIXEL_2X2, DRAW_FILL_EMPTY);
  //     Paint_DrawCircle(165, 125, 25, YELLOW, DOT_PIXEL_2X2, DRAW_FILL_EMPTY);
  //     Paint_DrawCircle(195, 125, 25, GREEN, DOT_PIXEL_2X2, DRAW_FILL_EMPTY);

  //     Paint_DrawImage(gImage_70X70, 20, 80, 70, 70);
  //     delay(3000);
  // #endif

#if 1
  Paint_NewImage(LCD_WIDTH, LCD_HEIGHT, 0, WHITE);
  Paint_Clear(WHITE);

  Paint_DrawString_EN(30, 10, "123", &Font24, YELLOW, RED);
  Paint_DrawString_EN(30, 34, "ABC", &Font24, BLUE, CYAN);

  // Paint_DrawString_CN(10, 150, "微", &Font24CN,WHITE, RED);
  // Paint_DrawString_CN(45, 150, "雪", &Font24CN,WHITE, RED);
  // Paint_DrawString_CN(80, 150, "电", &Font24CN,WHITE, RED);
  Paint_DrawString_CN(115, 150, "子", &Font24CN, WHITE, RED);

  Paint_DrawImage(gImage_70X70, 10, 70, 70, 70);

  Paint_DrawRectangle(100, 20, 160, 120, RED, DOT_PIXEL_2X2, DRAW_FILL_EMPTY);
  Paint_DrawLine(100, 20, 160, 120, MAGENTA, DOT_PIXEL_2X2, LINE_STYLE_SOLID);
  Paint_DrawLine(100, 120, 160, 20, MAGENTA, DOT_PIXEL_2X2, LINE_STYLE_SOLID);

  Paint_DrawCircle(50, 220, 25, BLUE, DOT_PIXEL_2X2, DRAW_FILL_EMPTY);
  Paint_DrawCircle(80, 220, 25, BLACK, DOT_PIXEL_2X2, DRAW_FILL_EMPTY);
  Paint_DrawCircle(110, 220, 25, RED, DOT_PIXEL_2X2, DRAW_FILL_EMPTY);
  Paint_DrawCircle(65, 245, 25, YELLOW, DOT_PIXEL_2X2, DRAW_FILL_EMPTY);
  Paint_DrawCircle(95, 245, 25, GREEN, DOT_PIXEL_2X2, DRAW_FILL_EMPTY);

  delay(300);

  while (client.available())
  {
    char c = client.read();
    Serial.print(c);
  }

  // if the server's disconnected, stop the client:
  if (!client.connected())
  {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();

    // do nothing forevermore:
    // while (true);
  }
#endif
}

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

// HardwareSerial SerialPort(1); // use UART1

// void setup()
// {
//   pinMode(37,INPUT);
//   pinMode(38,INPUT);
// }

// void loop()
// {

// }

// void setup()
// {
//   SerialPort.begin(115200, SERIAL_8N1, 13, 12);
//   Serial.begin(115200);
//   pinMode(8,OUTPUT);
//   // swSer.listen();
// }

// void loop()
// {
//   // swSer.println("Hello Software Serial");
//   while (SerialPort.available() > 0) {
//     Serial.write(SerialPort.read());
//   }
//   while (Serial.available() > 0) {
//     char a = Serial.read();
//     SerialPort.write(a);
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

// class MyEspUsbHost : public EspUsbHost {
//   void onKeyboardKey(uint8_t ascii, uint8_t keycode, uint8_t modifier) {
//     if (' ' <= ascii && ascii <= '~') {
//       Serial.printf("%c", ascii);
//     } else if (ascii == '\r') {
//       Serial.println();
//     }
//   };
// };

// MyEspUsbHost usbHost;

// void setup() {
//   Serial.begin(115200);
//   delay(500);

//   usbHost.begin();
//   usbHost.setHIDLocal(HID_LOCAL_Japan_Katakana);
// }

// void loop() {
//   usbHost.task();
// }

#endif