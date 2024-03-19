#include "HFTSerialConnection.h"


SerialConnection::SerialConnection(ConnectionBaseDelegate *delegate)
    : ConnectionBase(delegate)
{
}

SerialConnection::~SerialConnection()
{
    if (_serialType == ST_HARD_INNER)
    {
        delete _hardSerial;
        _hardSerial = nullptr;
    }
}

bool SerialConnection::init(uint8_t txPin, uint8_t rxPin)
{
    _serialType = ST_SOFT;
    return false;
}

bool SerialConnection::initInner(uint8_t rxPin,uint8_t txPin)
{
    _hardSerial = new HardwareSerial(1);
    _hardSerial->begin(115200, SERIAL_8N1, rxPin, txPin);  //RS485
    _serialType = ST_HARD_INNER;
    return false;
}

bool SerialConnection::init(HardwareSerial *serial)
{
    _hardSerial = serial;
    _serialType = ST_HARD;
    return true;
}

void SerialConnection::clear()
{
    switch (_serialType)
    {
    case ST_HARD:
    case ST_HARD_INNER:
        while (_hardSerial->available())
        {
            _hardSerial->read();
        }
        break;
    case ST_SOFT:
        // todo
        break;
    default:
        break;
    }
}

String SerialConnection::readString()
{
    String res;
    switch (_serialType)
    {
    case ST_HARD:
    case ST_HARD_INNER:
        while (_hardSerial->available())
        {
            char data = _hardSerial->read();
            res += data;
        }
        break;
    case ST_SOFT:
        // todo
        break;
    default:
        break;
    }
    return res;
}

void SerialConnection::sendString(uint16_t pid, const String &data)
{
    switch (_serialType)
    {
    case ST_HARD:
    case ST_HARD_INNER:
    {
        String msg = String(pid) + ":" + data;
        _hardSerial->print(msg);
        break;
    }
    case ST_SOFT:
    {
        // todo
        break;
    }
    default:
        break;
    }
}

String SerialConnection::sendString(uint16_t pid, const String &data, float timeout)
{
    // timeout = 100;
    // delay(200);
    _synchronous = true;
    String res;
    this->clear();
    this->sendString(pid, data);
    unsigned long start = millis();
    while (millis() - start < timeout * 1000)
    {
        delay(100);
        res = this->readString();
        if (res.length() > 0)
        {
            int index = res.indexOf(":");
            if (index < 1)
            {
                // 不合法字符串
                res = "";
                continue;
            }
            String pidStr = res.substring(0, index);
            if (pidStr.toInt() != pid)
            {
                res = "";
                continue;
            }
            res = res.substring(index + 1);
            break;
        }
    }
    _synchronous = false;
    return res;
}

String SerialConnection::sendFile(uint16_t pid, File& file,float timeout)
{
    _synchronous = true;
    this->clear();
    String res;
    while (file.available())
    {
      uint8_t ontByte = file.read();
      this->sendData(&ontByte,1);
    }
    unsigned long start = millis();
    while (millis() - start < timeout * 1000)
    {
        delay(100);
        res = this->readString();
        if (res.length() > 0)
        {
            int index = res.indexOf(":");
            if (index < 1)
            {
                // 不合法字符串
                res = "";
                continue;
            }
            String pidStr = res.substring(0, index);
            if (pidStr.toInt() != pid)
            {
                res = "";
                continue;
            }
            res = res.substring(index + 1);
            break;
        }
    }
    _synchronous = false;
    return res;
}

void SerialConnection::sendDebug(const String& data)
{
    this->sendString(100, data);
    delay(500);
}

String SerialConnection::readString(float timeout)
{
    _synchronous = true;
    String res;
    this->clear();
    unsigned long start = millis();
    while (millis() - start < timeout * 1000)
    {
        delay(100);
        res = this->readString();
        if (res.length() > 0)
        {
            break;
        }
    }
    _synchronous = false;
    return res;
}

void SerialConnection::receivedData(const String &data)
{
    int index = data.indexOf(":");
    if (index < 1)
    {
        // 不合法字符串
        return;
    }
    String pidStr = data.substring(0, index);
    String payload = data.substring(index + 1);
    uint16_t pid = pidStr.toInt();
    if (_delegate)
    {
        _delegate->dataParse(pid, payload);
    }
}

void SerialConnection::sendData(uint8_t *data, size_t length)
{
    switch (_serialType)
    {
    case ST_HARD:
    case ST_HARD_INNER:
    {
        _hardSerial->write(data,length);
        break;
    }
    case ST_SOFT:
    {
        // todo
        break;
    }
    default:
        break;
    }
}

void SerialConnection::loop()
{
    if (_synchronous)
    {
        return;
    }
    String res;
    switch (_serialType)
    {
    case ST_HARD:
    case ST_HARD_INNER:
        while (_hardSerial->available())
        {
            char data = _hardSerial->read();
            res += data;
        }
        delay(100);
        while (_hardSerial->available())
        {
            char data = _hardSerial->read();
            res += data;
        }
        break;
    case ST_SOFT:
        // todo
        break;
    default:
        break;
    }
    if (res.length() > 0)
    {
        this->receivedData(res);
    }
}