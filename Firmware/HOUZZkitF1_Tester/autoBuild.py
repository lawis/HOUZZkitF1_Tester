#!/usr/bin/env python3

import os,sys
import shutil
import hashlib
import uuid

CurrentPath = sys.path[0] + "/"
ProjRootPath = os.path.abspath("") + "/"

def hardware_type_get():
    defineStr = str("#define HARDWARE_TYPE ")
    fileName = ProjRootPath + "src/HOUZZkitTester/SDTConfig.h"
    with open(fileName, "r",encoding='utf-8') as f:
        configFile = f.read()
        typeLeftIndex = configFile.find(defineStr)
        if typeLeftIndex == -1:
            return -1
        typeLeftIndex = typeLeftIndex + len(defineStr)
        typeRightIndex = configFile.find("\n",typeLeftIndex)
        oldType = configFile[typeLeftIndex:typeRightIndex]
        return oldType

def hardware_type_set(type):
    defineStr = str("#define HARDWARE_TYPE ")
    fileName = ProjRootPath + "src/HOUZZkitTester/SDTConfig.h"
    with open(fileName, "r",encoding='utf-8') as f:
        configFile = f.read()
        typeLeftIndex = configFile.find(defineStr)
        if typeLeftIndex == -1:
            return
        typeLeftIndex = typeLeftIndex + len(defineStr)
        typeRightIndex = configFile.find("\n",typeLeftIndex)
        configFile = configFile[:typeLeftIndex] + str(type) + configFile[typeRightIndex:]
        with open(fileName, "w",encoding='utf-8') as f:
            f.write(configFile)

def build_code():
    cmd = "~/.platformio/penv/bin/platformio run"
    os.system(cmd)

def get_build_version():
    versionStr = ""

    fileName = ProjRootPath + "src/HOUZZkitTester/SDTConfig.h"
    with open(fileName, "r",encoding='utf-8') as f:
        configFile = f.read()
        versionLeftIndex = configFile.find("#define FIRMWARE_VERSION")
        if versionLeftIndex == -1:
            return
        versionLeftIndex = versionLeftIndex + len(str("#define FIRMWARE_VERSION"))
        versionRightIndex = configFile.find("//",versionLeftIndex)
        oldVersion = configFile[versionLeftIndex:versionRightIndex]
        versionStr = oldVersion.strip()[2:]

    fileName = fileName = ProjRootPath + "src/main.cpp"
    with open(fileName, "r",encoding='utf-8') as f:
        configFile = f.read()

        versionLeftIndex = configFile.find("#define FIRMWARE_BUILD_DATETIME ")
        if versionLeftIndex == -1:
            return versionStr
        versionLeftIndex = versionLeftIndex + len(str("#define FIRMWARE_BUILD_DATETIME "))
        versionRightIndex = configFile.find("\n",versionLeftIndex)
        versionStr = versionStr + configFile[versionLeftIndex:versionRightIndex]
        versionStr = versionStr.replace('"','')
        versionLeftIndex = configFile.find("#define FIRMWARE_BUILD_VERSION ")
        if versionLeftIndex == -1:
            return versionStr
        versionLeftIndex = versionLeftIndex + len(str("#define FIRMWARE_BUILD_VERSION "))
        versionRightIndex = configFile.find("\n",versionLeftIndex)
        oldVersion = configFile[versionLeftIndex:versionRightIndex]
        versionStr = versionStr + hex(int(oldVersion))[2:]
    return versionStr

def copy_bin_and_elf_to(hardwareModel):
    fileName = get_build_version()
    targetPath = ProjRootPath + "tools/FirmwareTools/Firmware/PixForce"+hardwareModel+"_" + fileName + "/" 
    if not os.path.exists(targetPath):
        os.makedirs(targetPath) 
    
    binFilePath = ProjRootPath + ".pio/build/esp32dev/firmware.bin"
    if not os.path.exists(binFilePath):
        print("firmware.bin not exists")
    else:
        shutil.copy(binFilePath,targetPath + "PixForce"+hardwareModel+"_" + fileName+".bin")
        shutil.copy(binFilePath,ProjRootPath + "tools/FirmwareTools/firmware"+hardwareModel+".bin")
        with open(binFilePath, 'rb') as fp:
            data = fp.read()
            file_md5= hashlib.md5(data).hexdigest().upper()
            with open(targetPath + "PixForce"+hardwareModel+"_" + fileName+"_bin_md5.txt", "w") as mf:
                mf.write(file_md5)
            print("bin file md5 : " + file_md5)

    elfFilePath = ProjRootPath + ".pio/build/esp32dev/firmware.elf"
    if not os.path.exists(elfFilePath):
        print("firmware.elf not exists")
    else:
        shutil.copy(elfFilePath,targetPath + "PixForce"+hardwareModel+"_" + fileName+".elf")


def main():
    hardwareType = hardware_type_get()
    
    hardware_type_set(3)
    build_code()
    copy_bin_and_elf_to("C1")

    hardware_type_set(4)
    build_code()
    copy_bin_and_elf_to("A1")

    hardware_type_set(hardwareType)
    

if __name__=="__main__":
	main()