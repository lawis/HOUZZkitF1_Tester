#ifndef __SDT_CONFIG_H__
#define __SDT_CONFIG_H__


#include <Arduino.h>

String FirmwareVersion();

#define FIRMWARE_VERSION            0x010301        //固件版本号

#define RDR_ENABLE                  true            //开启两次Rst设置重置功能

#define DRD_TIMEOUT                 5               //两次Rst最大间隔

#define LBE_USE_PSRAM               false           //BLE使用PSRAM

#ifndef TARGET_OS_MAC

#define APP_DEBUG_DATA              false           //开启APP debug数据

#define MQTT_DEBUG_DATA             false            //MQTT Server debug数据

#else

#define APP_DEBUG_DATA              true            //开启APP debug数据

#endif

#define MAX_WIFI_CONNECT_TIMES      30              //WIFI尝试重连次数

#define MAX_LED_BRIGHTNESS          32              //硬件层屏幕最高亮度

#define HEART_BEAT_INTERVAL         600             //心跳发送间隔

#define PREFERENCES_NAMESPACE       "PCDA"          //Preference 核心数据地址

#define PREFERENCES_CONFIG_DATA     "/PCDA_CD"      //系统配置信息 地址

#define PREFERENCES_DEVICE_DATA     "/PCDA_DD"      //设备

#define PREFERENCES_USER_DATA       "/PCDA_UD"      //用户

#define FILE_APP_LIST               "/APPLIST"

#define FILE_APP_DATA               "/APPDATA"

#define FILE_IMG_DATA               "/IMGDATA"
    
#define DRD_ADDRESS                 0

#define USE_SPIFFS                  true

#define ESP_DRD_USE_EEPROM          true

#define RENDER_TASK_WAIT_MS         100

#define PRINT_DEBUG_INFO            false

#define HEARTBEAT_URL               "https://app.pixforce.cn/api/v1/device/heartbeat"



#endif //__SDT_CONFIG_H__



