#ifndef __BT201_LIB__
#define __BT201_LIB__

#include <Arduino.h>
#include <map>
#define BT201_RECEIVED_BUFFER_LENGTH    100

class BT201
{
private:

    HardwareSerial* _serial = nullptr;

    uint8_t _enablePin;

    uint8_t _mutePin;

    unsigned long _timeOutDuration = 50;

    uint32_t _receivedIndex=0;

    uint8_t _recvBuffer[BT201_RECEIVED_BUFFER_LENGTH];

    typedef bool (BT201::*msgCallback)(String&);

    std::map<std::string,msgCallback> _msgCallbackMap;

    bool msgCallbackTT(String&);

protected:



private:

    bool clearSerial();

    String sendCommand(const std::string& cmd);

    String sendCommand(const char * format, ...);

    bool waitAvailable(unsigned long duration = 0);

    bool available();

    void parse();

    void parse(String& s);

    String parseParameter(String s);

    int32_t parseIntParameter(String s);

public:

    BT201();

    ~BT201();

    void begin(uint8_t enablePin,uint8_t mutePin,HardwareSerial& serial = Serial,unsigned long baud = 115200);

    void end();

    void loop();

    void setTimeOut(unsigned long timeOutDuration);

    void hardwareMute(bool e);

    //主要的通用命令

    //设置音量
    void setVolume(int8_t v){sendCommand("AT+CA%d",v);}
    int8_t readVolume(){return parseIntParameter(sendCommand("AT+QA"));}

    //播放暂停
    void play_pause(){sendCommand("AT+CB");}

    //下一曲
    void next(){sendCommand("AT+CC");}

    //上一曲
    void prev(){sendCommand("AT+CD");}

    //音量+
    void volumeUp(){sendCommand("AT+CE");}

    //音量-
    void volumeDown(){sendCommand("AT+CF");}

    //设置波特率 1-9600 2-19200 3-38400 4-57600 5-115200 6-256000
    void setBaud(int8_t b){sendCommand("AT+CT%02d",b);}
    int8_t readBaud(){return parseIntParameter(sendCommand("AT+QT"));}

    //设置模式 0-下一个模式  1-蓝牙 2-U盘 3-TF卡 4-音乐 
    void setMode(int8_t m){sendCommand("AT+CM%02d",m);}
    int8_t readMode(){return parseIntParameter(sendCommand("AT+QM"));}

    //设置静音 0-静音 1-解除 2-取反
    void setMute(int8_t m){sendCommand("AT+CU%02d",m);}

    //软复位
    void softReset(){sendCommand("AT+CZ");}

    //恢复为出厂设置
    void restore(){sendCommand("AT+CW");}

    //上电启动模式 0-蓝牙状态 1-等待命令 2-有设备播放，无设备蓝牙
    void setBootMode(int8_t m){sendCommand("AT+CP%02d",m);}

    //开启回传功能 0-关闭 1-开启 
    void setAutoReturn(int8_t r){sendCommand("AT+CR%02d",r);}

    //开启提示音 1-开启 0-关闭
    void setPromptTone(int8_t p){sendCommand("AT+CN%02d",p);}
    int8_t readPromptTone(){return parseIntParameter(sendCommand("AT+QN"));}

    //蓝牙切换后台 0-关闭 1-开启
    void setBTBackground(int8_t b){sendCommand("AT+CK%02d",b);}

    //主要的 本地播放 控制命令

    //设置 TF 卡 U 盘播放一次停还是顺序循环播放  1 开启 -- 单曲触发播放  0  关闭 -- 循环顺序播放
    void setMusicTriggerMode(int8_t b){sendCommand("AT+CJ%02d",b);}

    //播放控制  0-停止 1-播放 2-暂停 3-播放暂停切换 4-快进 5-快退 6-下一个文件夹 7-上一个文件夹 8-删除当前播放的文件
    void localPlayMode(int8_t b){sendCommand("AT+AA%02d",b);}

    //指定文件序号进行播放
    void localPlayByOrderNumber(uint16_t b){sendCommand("AT+AB%02d",b);}

    //播放模式 0-全部循环 1-单设备循环 2-单曲循环 3-单设备随机 4-文件夹循环
    void setLocalLoopMode(int8_t b){sendCommand("AT+AC%02d",b);}
    
    //循环播放指定文件夹  "/01*/*.???" 指根目录下01前缀文件夹下的所有文件
    void loopPlayFolder(const std::string& f){sendCommand("AT+AF%s",f.c_str());}

    //指定文件名播放一次
    void localPlayByPath(const std::string& f){sendCommand("AT+AJ%s",f.c_str());}

    //主要的 蓝牙 控制命令

    //蓝牙控制 0-回拨上一个电话 1-断开连接 2-拒接 3-挂断电话 4-接听电话（接听过程发则挂断） 5-扫描设备
    void btControl(int8_t b){sendCommand("AT+BA%02d",b);}

    //设置蓝牙名称
    void setBTName(const std::string& n){sendCommand("AT+BD%s",n.c_str());}
    String readBTName(){return parseParameter(sendCommand("AT+TD"));}

    //设置通话功能 0-关闭 1-打开
    void setBTCall(int8_t b){sendCommand("AT+B2%02d",b);}

    //设置BLE功能 0-关闭 1-打开
    void setBTBle(int8_t b){sendCommand("AT+B4%02d",b);}

    //设置EDR功能 0-关闭 1-打开
    void setBTEdr(int8_t b){sendCommand("AT+B5%02d",b);}

    String readBtState(){return parseParameter(sendCommand("AT+TS"));}

    //指定服务UUID
    void setBTServiceUUID(const std::string& u){sendCommand("AT+U0%s",u.c_str());}
    
};

#endif //__BT201_LIB__