#include "AppAwtrixClient3208.h"
#include "Fonts/TomThumb.h"
NS_DT_BEGIN


AppAwtrixClientLayer3208* m_appAwtrixClientLayer3208 = nullptr;
bool m_mqttConnected = false;
String version = "0.3";

void callback(char *topic, byte *payload, unsigned int length)
{
    // LOG_INT(length);
    m_mqttConnected = true;
    m_appAwtrixClientLayer3208->updateMatrix(payload,length);
}

static byte c1; // Last character buffer
byte utf8ascii(byte ascii)
{
	if (ascii < 128) // Standard ASCII-set 0..0x7F handling
	{
		c1 = 0;
		return (ascii);
	}
	// get previous input
	byte last = c1; // get last char
	c1 = ascii;		// remember actual character
	switch (last)	// conversion depending on first UTF8-character
	{
	case 0xC2:
		return (ascii)-34;
		break;
	case 0xC3:
		return (ascii | 0xC0) - 34;
		break;
	case 0x82:
		if (ascii == 0xAC)
			return (0xEA);
	}
	return (0);
}

String utf8ascii(String s)
{
	String r = "";
	char c;
	for (unsigned int i = 0; i < s.length(); i++)
	{
		c = utf8ascii(s.charAt(i));
		if (c != 0)
			r += c;
	}
	return r;
}

int GetRSSIasQuality(int rssi)
{
	int quality = 0;

	if (rssi <= -100)
	{
		quality = 0;
	}
	else if (rssi >= -50)
	{
		quality = 100;
	}
	else
	{
		quality = 2 * (rssi + 100);
	}
	return quality;
}

void AppAwtrixClientLayer3208::canvasShow()
{
    renderCanvas->copyBuffer(canvas->getBuffer(),canvas->width()*canvas->height());
}

void AppAwtrixClientLayer3208::sendMsgToServer(String s)
{
    mqttClient->publish("matrixClient",s.c_str());
}

AppAwtrixClientLayer3208::~AppAwtrixClientLayer3208()
{
    DT_SAFE_DELETE(espClient);
    DT_SAFE_DELETE(mqttClient);
    DT_SAFE_RELEASE(canvas);
}


bool AppAwtrixClientLayer3208::initLayer()
{
    m_appAwtrixClientLayer3208 = this;

    renderCanvas = SpriteCanvas::create(this->getContentSize());
    dot2d::CanvasSprite *sprite = dot2d::CanvasSprite::create(renderCanvas);
    this->addChild(sprite);
    this->scheduleUpdate();

    canvas = SpriteCanvas::create(this->getContentSize());
    canvas->setTextWrap(false);
    canvas->setFont(&TomThumb);
    canvas->setTextSize(1);
    canvas->canvasReset();
    canvas->retain();

    espClient = new WiFiClient();
    mqttClient = new PubSubClient(*espClient);

    mqttClient->setServer(this->getData()->serverIp.c_str(), this->getData()->serverPort);
	mqttClient->setCallback(callback);
    String clientId = "AWTRIXController-";
	clientId += String(random(0x1111,0xFFFF), HEX);
	if (mqttClient->connect(clientId.c_str()))
	{
		mqttClient->subscribe("awtrixmatrix/#");
		mqttClient->publish("matrixClient", "connected");
	}
    return true;
}

void AppAwtrixClientLayer3208::update(float dt)
{
    if(mqttClient)
    {
        mqttClient->loop();
    }
}

void AppAwtrixClientLayer3208::updateMatrix(const uint8_t* payload,const uint32_t length)
{
    uint8_t y_offset = 5;
    switch (payload[0])
	{
		case 0:
		{
			//Command 0: DrawText

			//Prepare the coordinates
			uint16_t x_coordinate = int(payload[1] << 8) + int(payload[2]);
			uint16_t y_coordinate = int(payload[3] << 8) + int(payload[4]);

			canvas->setCursor(x_coordinate + 1, y_coordinate + y_offset);
			canvas->setTextColor(DTRGB(payload[5], payload[6], payload[7]));
			String myText = "";
			for (int i = 8; i < length; i++)
			{
				char c = payload[i];
				myText += c;
			}
			canvas->print(utf8ascii(myText));
			break;
		}
		case 1:
		{
			//Command 1: DrawBMP

			//Prepare the coordinates
			uint16_t x_coordinate = int(payload[1] << 8) + int(payload[2]);
			uint16_t y_coordinate = int(payload[3] << 8) + int(payload[4]);

			int16_t width = payload[5];
			int16_t height = payload[6];

			unsigned short colorData[width * height];

			for (int i = 0; i < width * height * 2; i++)
			{
				colorData[i / 2] = (payload[i + 7] << 8) + payload[i + 1 + 7];
				i++;
			}

			for (int16_t j = 0; j < height; j++, y_coordinate++)
			{
				for (int16_t i = 0; i < width; i++)
				{
					canvas->drawPixel(x_coordinate + i, y_coordinate, (uint16_t)colorData[j * width + i]);
				}
			}
			break;
		}

		case 2:
		{
			//Command 2: DrawCircle

			//Prepare the coordinates
			uint16_t x0_coordinate = int(payload[1] << 8) + int(payload[2]);
			uint16_t y0_coordinate = int(payload[3] << 8) + int(payload[4]);
			uint16_t radius = payload[5];
			canvas->drawCircle(x0_coordinate, y0_coordinate, radius, DTRGB(payload[6], payload[7], payload[8]));
			break;
		}
		case 3:
		{
			//Command 3: FillCircle

			//Prepare the coordinates
			uint16_t x0_coordinate = int(payload[1] << 8) + int(payload[2]);
			uint16_t y0_coordinate = int(payload[3] << 8) + int(payload[4]);
			uint16_t radius = payload[5];
			canvas->fillCircle(x0_coordinate, y0_coordinate, radius, DTRGB(payload[6], payload[7], payload[8]));
			break;
		}
		case 4:
		{
			//Command 4: DrawPixel

			//Prepare the coordinates
			uint16_t x0_coordinate = int(payload[1] << 8) + int(payload[2]);
			uint16_t y0_coordinate = int(payload[3] << 8) + int(payload[4]);
			canvas->drawPixel(x0_coordinate, y0_coordinate, DTRGB(payload[5], payload[6], payload[7]));
			break;
		}
		case 5:
		{
			//Command 5: DrawRect

			//Prepare the coordinates
			uint16_t x0_coordinate = int(payload[1] << 8) + int(payload[2]);
			uint16_t y0_coordinate = int(payload[3] << 8) + int(payload[4]);
			int16_t width = payload[5];
			int16_t height = payload[6];
			canvas->drawRect(x0_coordinate, y0_coordinate, width, height, DTRGB(payload[7], payload[8], payload[9]));
			break;
		}
		case 6:
		{
			//Command 6: DrawLine

			//Prepare the coordinates
			uint16_t x0_coordinate = int(payload[1] << 8) + int(payload[2]);
			uint16_t y0_coordinate = int(payload[3] << 8) + int(payload[4]);
			uint16_t x1_coordinate = int(payload[5] << 8) + int(payload[6]);
			uint16_t y1_coordinate = int(payload[7] << 8) + int(payload[8]);
			canvas->drawLine(x0_coordinate, y0_coordinate, x1_coordinate, y1_coordinate, DTRGB(payload[9], payload[10], payload[11]));
			break;
		}

		case 7:
		{
			//Command 7: FillMatrix

			canvas->fillScreen(DTRGB(payload[1], payload[2], payload[3]));
			break;
		}

		case 8:
		{
			//Command 8: Show
            this->canvasShow();
			break;
		}
		case 9:
		{
			//Command 9: Clear
			// renderCanvas->canvasReset();
            canvas->canvasReset();
			break;
		}
		case 12:
		{
			//Command 12: GetMatrixInfo
			StaticJsonDocument<400> doc;
			doc["type"] = "MatrixInfo";
			doc["version"] = version;
			doc["wifirssi"] = String(WiFi.RSSI());
			doc["wifiquality"] = GetRSSIasQuality(WiFi.RSSI());
			doc["wifissid"] = WiFi.SSID();
			doc["IP"] = WiFi.localIP().toString();
            doc["LUX"] = DEVICE.ldr.getCurrentLux();
            DEVICE.sht.UpdateData();
            doc["Temp"] = DEVICE.sht.GetTemperature();
            doc["Hum"] = DEVICE.sht.GetRelHumidity();
            doc["hPa"] = 0;
			String str;
            serializeJson(doc,str);
            sendMsgToServer(str);
            break;
		}
		case 13:
		{
            //拒绝Awtrix服务器直接修改屏幕亮度
			// matrix->setBrightness(payload[1]);
			break;
		}
		case 14:
		{

			break;
		}
		case 15:
		{
            //拒绝wtrix服务器重置设备
			break;
		}
		case 16:
		{
            sendMsgToServer("ping");
			break;
		}
		case 21:
		{
            //TODO
			//multicolor...
			// uint16_t x_coordinate = int(payload[1] << 8) + int(payload[2]);
			// uint16_t y_coordinate = int(payload[3] << 8) + int(payload[4]);
			// matrix->setCursor(x_coordinate + 1, y_coordinate + y_offset);

			// String myJSON = "";
			// for (int i = 5; i < length; i++)
			// {
			// 	myJSON += (char)payload[i];
			// }
			// //Serial.println("myJSON: " + myJSON + " ENDE");
			// DynamicJsonBuffer jsonBuffer;
			// JsonArray &array = jsonBuffer.parseArray(myJSON);
			// if (array.success())
			// {
			// 	//Serial.println("Array erfolgreich geöffnet... =)");
			// 	for (int i = 0; i < (int)array.size(); i++)
			// 	{
			// 		String tempString = array[i]["t"];
			// 		String colorString = array[i]["c"];
			// 		JsonArray &color = jsonBuffer.parseArray(colorString);
			// 		if (color.success())
			// 		{
			// 			//Serial.println("Color erfolgreich geöffnet... =)");
			// 			String myText = "";
			// 			int r = color[0];
			// 			int g = color[1];
			// 			int b = color[2];
			// 			//Serial.println("Test: " + tempString + " / Color: " + r + "/" + g + "/" + b);
			// 			matrix->setTextColor(matrix->Color(r, g, b));
			// 			for (int y = 0; y < (int)tempString.length(); y++)
			// 			{
			// 				myText += (char)tempString[y];
			// 			}
			// 			matrix->print(utf8ascii(myText));
			// 		}
			// 	}
			// }
			break;
		}
		case 22:
		{
			//Text
			//scrollSpeed
			//icon
			//color
			//multicolor (textData?)
			//moveIcon
			//repeatIcon
			//duration
			//repeat
			//rainbow
			//progress
			//progresscolor
			//progressBackgroundColor
			//soundfile

			// String myJSON = "";
			// for (int i = 1; i < length; i++)
			// {
			// 	myJSON += (char)payload[i];
			// }
			// DynamicJsonBuffer jsonBuffer;
			// JsonObject &json = jsonBuffer.parseObject(myJSON);

			// String tempString = json["text"];
			// String colorString = json["color"];

			// JsonArray &color = jsonBuffer.parseArray(colorString);
			// int r = color[0];
			// int g = color[1];
			// int b = color[2];
			// int scrollSpeed = (int)json["scrollSpeed"];

			// Serial.println("Scrollspeed: " + (String)(scrollSpeed));

			// int textlaenge;
			// while (true)
			// {
			// 	matrix->setCursor(32, 6);
			// 	matrix->print(utf8ascii(tempString));
			// 	textlaenge = (int)matrix->getCursorX() - 32;
			// 	for (int i = 31; i > (-textlaenge); i--)
			// 	{
			// 		int starzeit = millis();
			// 		matrix->clear();
			// 		matrix->setCursor(i, 6);
			// 		matrix->setTextColor(matrix->Color(r, g, b));
			// 		matrix->print(utf8ascii(tempString));
			// 		matrix->show();
			// 		client.loop();
			// 		int endzeit = millis();
			// 		if ((scrollSpeed + starzeit - endzeit) > 0)
			// 		{
			// 			delay(scrollSpeed + starzeit - endzeit);
			// 		}
			// 	}
			// 	connectionTimout = millis();
			// 	break;
			// }
			break;
		}
		case 23:
		{
			//Command 23: DrawFilledRect

			//Prepare the coordinates
			uint16_t x0_coordinate = int(payload[1] << 8) + int(payload[2]);
			uint16_t y0_coordinate = int(payload[3] << 8) + int(payload[4]);
			int16_t width = payload[5];
			int16_t height = payload[6];
			canvas->fillRect(x0_coordinate, y0_coordinate, width, height, DTRGB(payload[7], payload[8], payload[9]));
			break;
		}
        default:
        {
            break;
        }
    }
}

NS_DT_END