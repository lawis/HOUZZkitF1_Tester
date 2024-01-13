#!/usr/bin/env python3

# from cgi import print_arguments
import hashlib
import os,sys
import subprocess
import time, threading

CurrentPath = sys.path[0] + "\\"

sys.path.append(CurrentPath+'..\\')
import COM

def printWithColor(printStr,colorId,showType = 0):
    print("\033["+str(showType)+";"+ str(colorId) +"m" + printStr +"\033[0m")

def findSerialPort():
    return COM.device_port()

def writeFirmware(serialPort):
    printWithColor(serialPort + ' 向串口设备写入固件....',93)
    cmd = "python " + CurrentPath +"esptoolpy\\esptool.py --chip auto -p "+serialPort+" -b 460800 --before=default_reset --after=hard_reset write_flash --flash_mode dio --flash_freq 80m --flash_size 4MB " \
        + " 0x00000000 " + CurrentPath + "bootloader.bin " \
        + " 0x00008000 " + CurrentPath + "partitions.bin " \
        + " 0x0000e000 " + CurrentPath + "boot_app0.bin " \
        + " 0x00010000 "  + CurrentPath + "firmware.bin "
    try:
        res = str(subprocess.check_output(cmd, shell=True))
        printWithColor(serialPort + ' 烧录完成',92)
    except Exception as e:
        printWithColor(serialPort + ' 烧录失败',91)

def burnOneDevice(serial_port_name):
    writeFirmware(serial_port_name)

class myThread (threading.Thread):
    def __init__(self, threadID, name):
        threading.Thread.__init__(self)
        self.threadID = threadID
        self.name = name
    def run(self):
        #printWithColor ("开始线程：" + self.name,92)
        burnOneDevice(self.name)
        #printWithColor ("结束线程：" + self.name,92)

def burn():
    serial_port_name_list = findSerialPort()
    threadID = 1
    for serial_port_name in serial_port_name_list:
        t = myThread(threadID, serial_port_name)
        threadID += 1
        t.start()

        
def main():
    if os.name == "nt":
        os.system("")
    burn()


if __name__=="__main__":
    main()




