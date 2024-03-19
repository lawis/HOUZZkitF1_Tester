#!/usr/bin/env python3
import os,sys,string,random
s = os.path.abspath(__file__)
sys.path.append(os.path.dirname(s)+'/lib')
import serial
import time
from serial.tools import list_ports
import subprocess
import re
import ipaddress
from gpio import GPIO
import socket
import fcntl
import struct
import json
import ctypes
from enum import Enum
from collections import namedtuple

#import pygatt 
#sudo apt-get install libbluetooth-dev
#日志等级
#0为不输出
#1为输出ERROR
#2为输出ERROR和INFO，3为输出ERROR、INFO和DEBUG
control_port_name = ""
zigbee_test = False
log_level = 3
#写一下输出等级方法，方便调试
def log_e(msg):
    if log_level > 0:
        print("[ERROR]", msg)

def log_i(msg):
    if log_level > 1:
        print("[INFO]", msg)

def log_d(msg):
    if log_level > 2:
        print("[DEBUG]", msg)
#================功能func========================================
def generate_random_string(length):
    # 生成指定长度的随机字符串
    letters = string.ascii_letters + string.digits
    return ''.join(random.choice(letters) for _ in range(length))

def write_string_to_file(file_path,random_string):
    # 生成随机字符串并将其写入文件
    with open(file_path, 'w') as f:
        f.write(random_string)

def read_string_from_file(file_path):
    # 从文件中读取字符串
    with open(file_path, 'r') as f:
        content = f.read().strip()
    return content        

def get_mac_address(interface):
    # 创建一个socket对象
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    # 获取网络接口名称
    ifname = interface.encode('utf-8')

    # 使用ioctl函数获取MAC地址
    mac_address = fcntl.ioctl(s.fileno(), 0x8927, struct.pack('256s', ifname[:15]))

    # 解码MAC地址并返回
    mac_address = ':'.join(['{:02x}'.format(b) for b in mac_address[18:24]])
    return mac_address

#=================logic========================================

def handlerSSDandTF(device_path):
    if not os.path.exists(device_path):
        return False
    file_path = device_path + 'random_string.txt' #修改一下路径
    length = 64
    random_string = generate_random_string(length)
    write_string_to_file(file_path,random_string)
    random_string_from_file = read_string_from_file(file_path)
    os.remove(file_path)
    if random_string == random_string_from_file:
        return True
    return False
        
def handlerEMMC():
    start_time = time.time()
    file_path = 'large_file.txt'
    with open(file_path, 'wb') as f:
        # 每次写入 1MB 数据
        one_mb = b'1' * 1024 * 1024
        target_size = 1024 * 1024 * 200  # 200M
        current_size = 0

        while current_size < target_size:
            f.write(one_mb)
            current_size += len(one_mb)

    end_time = time.time()
    elapsed_time = end_time - start_time
    os.remove(file_path)
    return elapsed_time

def handlerWifi(ssid,password):
    # 执行命令以连接到 Wi-Fi 并获取其信号强度
    result = subprocess.run(['nmcli', 'device', 'wifi', 'connect', ssid, 'password', password,"ifname","wlan0"], capture_output=True, text=True)
    #sudo nmcli device wifi connect DLINK-Y9C6_plus password yc18610681168 ifname wlan0
    # 断开 Wi-Fi 连接
    subprocess.run(['nmcli', 'device', 'disconnect', 'wlan0'], capture_output=True)
    if result.returncode == 0:
        log_d(f"连接到 Wi-Fi '{ssid}'")
        return True
    else:
        log_d(f"未能连接到 Wi-Fi '{ssid}'")
        return False

        
def get_wifi_signal_strength(ssid):
    command = 'nmcli device wifi list'
    output = subprocess.check_output(command, shell=True).decode('utf-8')
     # 将输出按行拆分
    lines = output.split('\n')

     # 提取信号强度值
    signal_strength = None
    for line in lines:
      if ssid in line and 'Infra' in line:
        parts = line.split()
        if len(parts) >= 6:
            signal_strength = int(parts[5])
            break
    # 输出信号强度值
    if signal_strength is not None:
       log_d("信号强度:"+ str(signal_strength))
       return signal_strength
    else:
       return ""
def handlerRTC():
    subprocess.run(['date', '-s', '12:33:33'])
    # 使用subprocess运行hwclock -w命令
    result = subprocess.run(['hwclock', '-w'])
    # 判断命令是否报错
    if result.returncode == 0:
        return True
    else:
        return False
def handlerWAN():
    # 使用subprocess运行ifconfig命令
    output = subprocess.run(['ifconfig', 'eno0'], capture_output=True, text=True).stdout

    # 使用正则表达式匹配IP地址
    ip_match = re.search(r'inet (\d+\.\d+\.\d+\.\d+)', output)

    # 获取匹配到的IP地址
    if ip_match:
        ip = ip_match.group(1)
        ip_address = ipaddress.IPv4Address(ip)
        log_d("IP地址:"+str(ip_address))
        # 定义目标网段
        target_network = ipaddress.IPv4Network("192.168.55.0/24")
        if ip_address in target_network:
            red_file_str = read_string_from_file("/sys/class/net/eno0/speed")
            if red_file_str == "1000":
                return True
            log_d("/sys/class/net/eno1/speed is error ："+red_file_str)
        else:
            log_d("IP地址不在192.168.55.0网段")
       
    else:
        log_d("无法获取IP地址")
    return False

def handlerLAN():
    red_file_str = read_string_from_file("/sys/class/net/eno1/speed")
    if red_file_str == "1000":
        return True
    log_d("/sys/class/net/eno1/speed is error ："+red_file_str)
    return False

#msg ex 4D20 或者 4D21
# GPIO pin脚计算公式:pin = bank *32 + number
# GPIO 小组编号计算公式: number = group*8 + X
# bank = 4;//GPIO4_D5 => 4, bank E [0,4]
# group = 3;//GPIO4_D5 => 3, group E {(A=0),(B=1),(C=2),(D=3)]
# X = 5://GPIO4 _D5 => 5,X E [0.7]
# number = group *8+X=3*8+ 5 = 29
# pin = bank*32 + number= 4*32 + 29 = 157:
def getGpioIndex(msg):
    if len(msg) == 4:
        bank = msg[0]
        group = 0
        if msg[1] == "A":
            group = 0
        elif msg[1] == "B": 
            group = 1
        elif msg[1] == "C":     
            group = 2
        elif msg[1] == "D":      
            group = 3
        x = int(msg[2])    
        pin = int(bank)  * 32 + group*8 + x
        return str(pin)
    else:
        log_d("字符串的长度不是3") 
    return
   
def handlerGPIO(gpio_index,value):
    led = GPIO(gpio_index)
    led.set_direction("out")
    led.set_value(int(value)) # 0 or 1
    led.unexport_gpio()
    return   

def handlerPWM(value):
    value = "5"
    file = "/sys/class/thermal/cooling_device0/cur_state"
    write_string_to_file(file,value)    
    if value == read_string_from_file(file):
        return True
    else:
        return False
   
def handlerBluetooth(device_address):
    return

def WriteI2cAndRkVerdon(sn,check_code):        
    result = subprocess.run(f"./factory_check -S {sn}", shell=True, capture_output=True, text=True)
    #print(result.stdout.rstrip())
    if result.stdout.rstrip() != "ok":
        #print(f"factory_check -S {result.stdout}")
        return False
    result = subprocess.run(f"./factory_check -s {sn}", shell=True, capture_output=True, text=True)
    if result.stdout.rstrip() != "ok":
       # print(f"factory_check -s {result.stdout}")
        return False
    result = subprocess.run(f"./factory_check -V {check_code}", shell=True, capture_output=True, text=True)
    if result.stdout.rstrip() != "ok":
       # print(f"factory_check -V {result.stdout}")
        return False
    result = subprocess.run(f"./factory_check -v {check_code}", shell=True, capture_output=True, text=True)
    if result.stdout.rstrip() != "ok":
       # print(f"factory_check -v {result.stdout}")
        return False
    HosDeviceName = "HOUZZkit Force 1"    
    result = subprocess.run(f"./factory_check -M {HosDeviceName}", shell=True, capture_output=True, text=True)
    if result.stdout.rstrip() != "ok":
       # print(f"factory_check -M {result.stdout}")
        return False
    result = subprocess.run(f"./factory_check -m {HosDeviceName}", shell=True, capture_output=True, text=True)
    if result.stdout.rstrip() != "ok":
       # print(f"factory_check -m {result.stdout}")
        return False
    
    return  True    
#=========================================================
def dataParse(pid,data,controlPort):
    log_d("recvMsg:" + pid + ":" + data)
    if pid == "1":
            return  
    elif pid == "2":
            return
    elif pid == "3":
            return        
    elif pid == "4":
            output = subprocess.check_output(['hos_release', '-#'])
            decoded_output = output.decode('utf-8')
            sendMsg(controlPort,"4",decoded_output.strip())      
            return
    elif pid == "5":
            #host usb口 工作测试
            sendMsg(controlPort,"5","ok")
            return           
    elif pid == "6":
            #DDR
            sendMsg(controlPort,"6","ok")
            return       
    elif pid == "7":
            #get data 7:
            #eMMC
            elapsed_time = handlerEMMC()
            sendMsg(controlPort,"7",str(elapsed_time))
            return 
    elif pid == "8":
            #get data 8:
            #   执行特定命令 hwclock
            if handlerRTC():
                sendMsg(controlPort,"8","ok")
            else:
                sendMsg(controlPort,"8","error")
            return
    elif pid == "9":
             #IIC加密芯片
             #1.读取i2c芯片中是否存在数据且数据能够被正常校验
            p = "F1CheckTest"
            output = subprocess.check_output(['hos_release', '-^',p])
            decoded_output = output.decode('utf-8').strip()
            log_d("hos_release -^ " + p + "output :"+ decoded_output)
            if decoded_output == "ok":
                output = subprocess.check_output(['hos_release', '-*'])#校验芯片是否正常使用
                decoded_output = output.decode('utf-8').strip()
                log_d("hos_release -* output :"+ decoded_output)
                if decoded_output == p:
                    sendMsg(controlPort,"9","ok")
                    return   
            sendMsg(controlPort,"9","error")
            return
    elif pid == "10":
            controlserial = serial.Serial("/dev/ttyS7",115200,timeout=0.2)
            sendMsg(controlserial,"10","ping")
            start_time = time.time()  # 记录开始时间
            while True:
                current_time = time.time()  # 获取当前时间
                elapsed_time = current_time - start_time  # 计算已经过去的时间
                if elapsed_time >= 10:
                    log_d(data + " recv is time out ")  
                    sendMsg(controlPort,"10","error")
                    break
                res = controlserial.readall().decode()
                if res == "10:pong":
                    sendMsg(controlPort,"10","ok")
                    controlserial.close()  
                    break
            controlserial.close()  
            return
    elif pid == "11":
            controlserial = serial.Serial(data,115200,timeout=0.2)
            sendMsg(controlserial,"11","ping")
            start_time = time.time()  # 记录开始时间
            while True:
                current_time = time.time()  # 获取当前时间
                elapsed_time = current_time - start_time  # 计算已经过去的时间
                if elapsed_time >= 10:
                    log_d(data + " recv is time out ")  
                    sendMsg(controlPort,"11","error")
                    break
                res = controlserial.readall().decode()
                if res == "11:pong":
                   sendMsg(controlPort,"11","ok")
                   break
            controlserial.close()
            return
    elif pid == "12":
            #ssd 测试
            if handlerSSDandTF("/media/nvme0n1p1"):
                sendMsg(controlPort,"12","ok")
            else:
                sendMsg(controlPort,"12","error")    
            return    
    elif pid == "13":
            #tf 测试
            if handlerSSDandTF("/media/mmcblk1p1/"):
                sendMsg(controlPort,"13","ok")
            else:
                sendMsg(controlPort,"13","error")    
            return     
    elif pid == "14":
            #GPIO 测试 根据接脚标号和高低电位来操作接脚返回
            gpio_index = getGpioIndex(data)
            value = data[3]
            handlerGPIO(gpio_index,value)
            sendMsg(controlPort,"14","ok")
            return
    elif pid == "15":
            #PWM 测试  根据接脚标号和高低电位来操作接脚返回
            if handlerPWM("5"):
                sendMsg(controlPort,"15","ok")
            else:
                sendMsg(controlPort,"15","error")
            return
    elif pid == "16":
            #WAN口测试
            if handlerWAN():
                sendMsg(controlPort,"16","ok")
            else:
                sendMsg(controlPort,"16","error")   
            return    
    elif pid == "17":
            if handlerLAN():
                sendMsg(controlPort,"17","ok")
            else:
                sendMsg(controlPort,"17","error")    
            #LAN口测试
            return       
    elif pid == "18":
            #WIFI测试
            split_string =str(data).split(",")
            ssid = "HOUZZKitF1"
            password = "88888888"
            if len(split_string)>=2:
                ssid = split_string[0]
                password = split_string[1]                
            signal_strength = ""
            if handlerWifi(ssid,password):
                signal_strength = get_wifi_signal_strength(ssid)
            log_d(f"连接到 Wi-Fi '{ssid}',信号强度是'{signal_strength}'")    
            if signal_strength>0:
                sendMsg(controlPort,"18",str(signal_strength))        
            else:
                sendMsg(controlPort,"18","error")        
            return    
    elif pid == "19":
            #蓝牙测试
            sendMsg(controlPort,"19","ok")
            return   
    elif pid == "20":
        if zigbee_test:
            sendMsg(controlPort,"20","ok")
        else:
            sendMsg(controlPort,"20","error")
        return
    elif pid == "21":
        sendMsg(controlPort,"21","ok")
        return
    elif pid == "22":
        sendMsg(controlPort,"22","ok")
        return
    elif pid == "23":
        sendMsg(controlPort,"23","ok")
        return
    elif pid == "24":
        sendMsg(controlPort,"24","ok")
        return
    elif pid == "25":
        sendMsg(controlPort,"25","ok")
        return
    elif pid == "30":
        #返回mac地址
        # 获取本机的MAC地址
        eno0 = get_mac_address("eno0")
        eno1 = get_mac_address("eno1")
        sendMsg(controlPort,"25",eno0+","+eno1)
        return
    elif pid == "31":
        #保存sn码和认证设备token
        split_string =str(data).split(",")
        option = {
        "sn":split_string[0].strip(),
        "check_code":split_string[1].strip(),
        }
        json_str = json.dumps(option)
        print(json_str)
        #todo 添加sn码相关
        res = WriteI2cAndRkVerdon(option["sn"],option["check_code"])
        if res:
            sendMsg(controlPort,"31","ok")
        else:
            sendMsg(controlPort,"31","false")
            
        return   
def sendMsg(serial,pid,msg):
    serial.write((pid + ":" + msg).encode())

def recvMsg(serial):
    res = serial.readall().decode()
    if res == "":
        return res
    index = res.find(":")
    if index <= 0:
        return res
    pid = res[0:index]
    msg = res[index+1:]
    dataParse(pid,msg)
    return pid


##########===============sn写入方法实现======================#################
VENDOR_CONSTANTS = {
    'VENDOR_SN_ID': 1,
    'VENDOR_WIFI_MAC_ID': 2,
    'VENDOR_LAN_MAC_ID': 3,
    'VENDOR_BT_MAC_ID': 4,
    'VENDOR_HDCP_14_HDMI_ID': 5,
    'VENDOR_HDCP_14_DP_ID': 6,
    'VENDOR_HDCP_2x_ID': 7,
    'VENDOR_DRM_KEY_ID': 8,
    'VENDOR_PLAYREADY_Cert_ID': 9,
    'VENDOR_ATTENTION_KEY_ID': 10,
    'VENDOR_PLAYREADY_ROOT_KEY_0_ID': 11,
    'VENDOR_PLAYREADY_ROOT_KEY_1_ID': 12,
    'VENDOR_SENSOR_CALIBRATION_ID': 13,
    'VENDOR_IMEI_ID': 15,
    'VENDOR_VERIFY_CODE_ID': 27,
    'VENDOR_MODEL_NAME_ID': 28,
    'VENDOR_CHECK_ID': 29,
    'VENDOR_MAX_SIZE':1024,
    'VENDOR_REQ_TAG':0x56524551,
}

DBT_Sn = 0
DBT_ModelName = 1
DBT_Verify = 2
DBT_Check = 3
DBT_ERASE = 4
DBT_Size = 5

I2C_SLAVE = 0x0703

device_info_block_list = {
    DBT_Sn :{
        "type":DBT_Sn,
        "vendor_id" : VENDOR_CONSTANTS['VENDOR_SN_ID'],
        "offset":8,
        "size":16,
    },
    DBT_ModelName:{
        "type":DBT_ModelName,
        "vendor_id" : VENDOR_CONSTANTS['VENDOR_MODEL_NAME_ID'],
        "offset":24,
        "size":32,
    },
    DBT_Check:{
        "type":DBT_Check,
        "vendor_id" : VENDOR_CONSTANTS['VENDOR_VERIFY_CODE_ID'],
        "offset":56,
        "size":64,
    },
    DBT_ERASE:{
        "type":DBT_ERASE,
        "vendor_id" : 255,
        "offset":0,
        "size":255,
    },
}
class RKVendorReq(ctypes.Structure):
    _fields_ = [
        ('tag', ctypes.c_int),
        ('id', ctypes.c_int),
        ('len', ctypes.c_int),
        ('data', ctypes.c_char * 256)  # 这里假设 data 的最大长度为 256
    ]

VENDOR_WRITE_IO = 0x564802

def rk_vendor_write(dbt, data, size):
    if dbt >= DBT_Size or size > device_info_block_list[dbt]["size"]:
        return -1
    if size > VENDOR_CONSTANTS['VENDOR_MAX_SIZE']:
        print("vendor storage input data overflow")
        return -2

    sys_fd = os.open("/dev/vendor_storage", os.O_RDWR, 0)
    if sys_fd < 0:
        print("vendor storage open fail")
        return -3
    
    req = RKVendorReq(VENDOR_CONSTANTS['VENDOR_REQ_TAG'], device_info_block_list[dbt]["vendor_id"], len(data.encode('utf-8')), data.encode('utf-8'))
    ret = fcntl.ioctl(sys_fd, VENDOR_WRITE_IO, req.serialize(),1)
    os.close(sys_fd)
    if ret:
        print("vendor write error")
        return -4

    return 0


def i2c_write(dbt, data, size):
    if dbt >= DBT_Size or size > device_info_block_list[dbt]["size"]:
        return -1
    i2c_fd = os.open("/dev/i2c-5", os.O_RDWR, 0)
    if i2c_fd < 0:
        return -2
    device_address = 0x50  # Adjust to the desired I2C address
    ret = fcntl.ioctl(i2c_fd, I2C_SLAVE, device_address)
    os.close(i2c_fd)
    if ret:
        return -3
    time.sleep(0.004)
    i2c_fd = open("/dev/i2c-5","r+b")
    size_offset_buff = bytearray([0, 0])
    size_offset_buff[0] = device_info_block_list[dbt]["type"]
    size_offset_buff[1] = size
    i2c_fd.write(size_offset_buff)
    time.sleep(0.004)
    
    write_point = 0
    while write_point < size:
        write_size = min(size - write_point, 8)
        write_buff = bytearray([0] * (write_size + 1))
        write_buff[0] = device_info_block_list[dbt]["offset"] + write_point
        write_buff[1:write_size+1] = data[write_point:write_point+write_size].encode()
        i2c_fd.write(write_buff)
        bytes_written = i2c_fd.write(write_buff)
        if bytes_written != len(write_buff):
            i2c_fd.close()
            return -5
        write_point += write_size
        time.sleep(0.004)  # Sleep for 4ms
    
    i2c_fd.close()
    return 0

def main():
    # 访问脚本名称
    script_name = sys.argv[0]
    # print(f"脚本名称:{script_name}")
    # 替换文件内容中的占位符
    sn = sys.argv[1]
    check_code = sys.argv[2]
    re = WriteI2cAndRkVerdon(sn,check_code)
    print(re)
    
if __name__=="__main__":
    main()