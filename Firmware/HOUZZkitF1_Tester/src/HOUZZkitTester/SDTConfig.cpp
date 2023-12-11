#include "SDTConfig.h"

String FirmwareVersion(){
    return  String(FIRMWARE_VERSION>>16 & 0xFF) + String(".") + 
            String(FIRMWARE_VERSION>> 8 & 0xFF) + String(".") + 
            String(FIRMWARE_VERSION>> 0 & 0xFF);
};

