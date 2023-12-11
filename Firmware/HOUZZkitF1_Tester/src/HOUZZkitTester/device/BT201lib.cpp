#include "BT201lib.h"
#include "Ticker.h"

bool BT201::clearSerial()
{
    _receivedIndex = 0;
    while(_serial->available()){
        delay(0);
        _serial->read();
    }
    return _serial->read() == -1;
}

String BT201::sendCommand(const std::string& cmd)
{
    this->clearSerial();
    _serial->printf("%s\r\n",cmd.c_str());
    String res = "";
    if(this->waitAvailable())
    {
        res = String((const char *)_recvBuffer);
    }
    // Serial.print("sendCommand:");
    // Serial.println(res);
    this->clearSerial();
    return res;
}

String BT201::sendCommand(const char * format, ...)
{
    char loc_buf[64];
    char * temp = loc_buf;
    va_list arg;
    va_list copy;
    va_start(arg, format);
    va_copy(copy, arg);
    int len = vsnprintf(temp, sizeof(loc_buf), format, copy);
    va_end(copy);
    if(len < 0) {
        va_end(arg);
        return String("");
    };
    if(len >= sizeof(loc_buf)){
        temp = (char*) malloc(len+1);
        if(temp == NULL) {
            va_end(arg);
            return String("");
        }
        len = vsnprintf(temp, len+1, format, arg);
    }
    va_end(arg);
    const char* buffer = (char *)temp;
    std::string cmd(buffer,len);
    String res = this->sendCommand(cmd);
    if(temp != loc_buf){
        free(temp);
    }
    return res;
}


bool BT201::waitAvailable(unsigned long duration)
{
    unsigned long timer = millis();
    if (!duration) 
    {
        duration = _timeOutDuration;
    }
    while (!available())
    {
        if (millis() - timer > duration) 
        {
            // Serial.println("Error!!! Bt Cmd Timeout");
            return false;
        }
        delay(0);
    }
    return true;
}

bool BT201::available()
{
    while (_serial->available()) 
    {
        delay(0);
        uint8_t c = _serial->read();
        // Serial.write(c);
        _recvBuffer[_receivedIndex++] = c;
        // //TODO 超过 BT201_RECEIVED_BUFFER_LENGTH  时如何处理
        if (c == '\n')
        {
            _recvBuffer[_receivedIndex] = 0;
            return true;
        }
    }
    return false;
}

void BT201::parse()
{
    String recvStr((const char *)_recvBuffer);
    this->parse(recvStr);
    this->clearSerial();
}

void BT201::parse(String& s)
{
    int16_t p = s.indexOf('+');
    if (p != -1)
    {
        String cmdStr = s.substring(0,p);
        String parStr = s.substring(p+1);
        if(_msgCallbackMap.find(cmdStr.c_str()) != _msgCallbackMap.end())
        {
            (this->*_msgCallbackMap[cmdStr.c_str()])(parStr);
        }
    }
}

String BT201::parseParameter(String s)
{
    int16_t p = s.indexOf('+');
    String parameter("");
    if (p != -1)
    {
        parameter = s.substring(p+1);
    }
    return parameter;
}

int32_t BT201::parseIntParameter(String s)
{
    String res = parseParameter(s);
    if (res.equals(""))
    {
        return -1;
    }
    return res.toInt();   
}

bool BT201::msgCallbackTT(String& msg)
{
    return true;
}

void BT201::setTimeOut(unsigned long timeOutDuration)
{
    _timeOutDuration = timeOutDuration;
}

void BT201::hardwareMute(bool e)
{
    if (e)
    {
        digitalWrite(_mutePin,LOW);
    }else
    {
        digitalWrite(_mutePin,HIGH);
    }
    
}

BT201::BT201()
{
    _msgCallbackMap["TT"] = &BT201::msgCallbackTT;
}

BT201::~BT201()
{
    
}


void BT201::begin(uint8_t enablePin,uint8_t mutePin,HardwareSerial& serial,unsigned long baud)
{
    _serial = &serial;
    _enablePin = enablePin;
    _mutePin = mutePin;

    pinMode(enablePin,OUTPUT);
    digitalWrite(_enablePin,HIGH);
    pinMode(mutePin,OUTPUT);
    digitalWrite(mutePin,LOW);
    delay(2500);
    digitalWrite(mutePin,HIGH);

    _serial->begin(baud);
}

void BT201::end()
{
    // _serial->end();//不注释会crash
    digitalWrite(_enablePin,LOW);
    digitalWrite(_mutePin,LOW);
}


void BT201::loop()
{
    // if(this->available())
    // {
    //     // this->parse();
    // }
}
