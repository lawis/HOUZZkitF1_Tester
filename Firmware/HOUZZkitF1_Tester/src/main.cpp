#include <Arduino.h>

// #define UNIT_TEST

#ifndef UNIT_TEST

#include "HOUZZkitF1Tester/device/HFTScreen.h"
#include "HOUZZkitF1Tester/device/HFTEncoder.h"
#include "HOUZZkitF1Tester/device/HFTDevice.h"
#include "HOUZZkitF1Tester/connection/HFTConnectionManager.h"

ScreenManager *m_screenManager = nullptr;
HFTEncoder *m_hftEncoder = nullptr;
HFTDevice *m_hftDevice = nullptr;
ConnectionManager *m_connectionManager = nullptr;

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

void checkFlow_3(uint16_t *errCode)
{
  m_screenManager->showDeviceStatus(FL_DEVICE_CONNECTED, FS_CHECK_1);
  bool deviceConnected = false;
  uint8_t checkTimes = 60;
  while (!deviceConnected && checkTimes-- > 0)
  {
    delay(1000);
    deviceConnected = m_connectionManager->deviceConnected();
    m_screenManager->showDeviceStatus(FL_DEVICE_CONNECTED, FunctionStatus((59 - checkTimes) / 12 + 1));
  }

  if (!deviceConnected)
  {
    *errCode = 10301;
    m_screenManager->showDeviceStatus(FL_DEVICE_CONNECTED, FS_FAIL);
    return;
  }
  m_screenManager->showDeviceStatus(FL_DEVICE_CONNECTED, FS_PASS);
}

void checkFlow_4(uint16_t *errCode)
{
  m_screenManager->showDeviceStatus(FL_DEVICE_ACTIVED, FS_CHECK_1);
  String res = m_connectionManager->serialConn->sendString(FL_DEVICE_ACTIVED, "device_actived", 2);
  if (res.length() == 0)
  {
    *errCode = 10401;
    m_screenManager->showDeviceStatus(FL_DEVICE_ACTIVED, FS_FAIL);
    return;
  }
  if (res == "ok")
  {
    m_screenManager->showDeviceStatus(FL_DEVICE_ACTIVED, FS_PASS);
    return;
  }
  m_screenManager->showDeviceStatus(FL_DEVICE_ACTIVED, FS_FAIL);
  *errCode = 10402;
}

void checkFlowAction()
{
  uint16_t errCode = 0;
  m_connectionManager->reset();
  checkFlow_1(&errCode);if (errCode != 0){return;}
  checkFlow_2(&errCode);if (errCode != 0){return;}
  checkFlow_3(&errCode);if (errCode != 0){return;}
  checkFlow_4(&errCode);if (errCode != 0){return;}
}

void encoderButtonClick()
{
  // Serial.println("encoderButtonClick");
  checkFlowAction();

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

  // pinMode(38,OUTPUT);
  // digitalWrite(37,HIGH);
  // pinMode(35, OUTPUT);
  // pinMode(36, INPUT_PULLUP);
  // digitalWrite(35, HIGH);
  // digitalWrite(36, HIGH);

  //----------------Initialize Display----------------
  m_screenManager = ScreenManager::getInstance();
  m_screenManager->initScreen();

  // m_screenManager->ledCheck();

  m_hftEncoder = new HFTEncoder();
  m_hftEncoder->init(4, 5, 6);
  m_hftEncoder->attachClick(encoderButtonClick);
  m_hftEncoder->attachRotation(encoderRotation);

  m_hftDevice = new HFTDevice();
  m_hftDevice->init();

  // //Start network request thread  //Starting the network on a network thread
  xTaskCreatePinnedToCore(connectTask, "connectTask", 16384, NULL, 1, NULL, 0);
}

void loop()
{
  // Serial.println("loop");
  m_hftEncoder->tick();
  m_screenManager->lcdDisplay->loop();
  // delay(100);
}

void connectTask(void *pvParameters)
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
//   SerialRS485.begin(115200, SERIAL_8N1, 42, 41);
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

// #ifdef ESP8266
// #define PIN_SDA 4
// #define PIN_SCL 5
// #endif

// CyMCP23016 mcp;

// void setup() {
//     Serial.begin(115200);

//     mcp.begin(17, 18, 0x26);

//     // Set Pin 0 on Port 0 as an output.
//     mcp.pinMode(MCP23016_PIN_GPIO1_0, OUTPUT);
// }

// void loop() {
//     delay(1000);

//     // Set the pin HIGH and read back the state.
//     mcp.digitalWrite(MCP23016_PIN_GPIO1_0, HIGH);
//     uint8_t val = mcp.digitalRead(MCP23016_PIN_GPIO1_0);
//     Serial.print(F("Pin 0.0 is "));
//     Serial.println(val == HIGH ? "HIGH" : "LOW");

//     delay(1000);

//     // Set the pin LOW and read back the state.
//     mcp.digitalWrite(MCP23016_PIN_GPIO1_0, LOW);
//     val = mcp.digitalRead(MCP23016_PIN_GPIO1_0);
//     Serial.print(F("Pin 0.0 is "));
//     Serial.println(val == HIGH ? "HIGH" : "LOW");
// }

#endif