#!/usr/bin/env python3

# Import("env")
import os,sys
import time
# CurrentPath = sys.path[0] + "/"

# def after_build(source, target, env): 
#     print("[From Script] Finished building!!")

# env.AddPreAction("buildprog", after_build)


def version_check():
    fileName = sys.path[0] + "/" + "../HOUZZkitTester/SDTConfig.h"
    with open(fileName, "r",encoding='utf-8') as f:
        configFile = f.read()
        versionLeftIndex = configFile.find("#define FIRMWARE_VERSION")
        if versionLeftIndex == -1:
            return
        versionLeftIndex = versionLeftIndex + len(str("#define FIRMWARE_VERSION"))
        versionRightIndex = configFile.find("//",versionLeftIndex)
        oldVersion = configFile[versionLeftIndex:versionRightIndex]
        oldVersion = oldVersion.strip()
        version_record_file = sys.path[0] + "/" + "version_record"
        with open(version_record_file, "r") as of:
            version_record = of.read()
            print(version_record)
            if version_record != oldVersion:
                with open(version_record_file, "w") as of:
                    of.write(oldVersion)
                fileName = sys.path[0] + "/" + "../main.cpp"
                with open(fileName, "r") as mf:
                    configFile = mf.read()
                    versionLeftIndex = configFile.find("#define FIRMWARE_BUILD_VERSION ")
                    if versionLeftIndex == -1:
                        return
                    versionLeftIndex = versionLeftIndex + len(str("#define FIRMWARE_BUILD_VERSION "))
                    versionRightIndex = configFile.find("\n",versionLeftIndex)
                    configFile = configFile[:versionLeftIndex] + str(0) + configFile[versionRightIndex:]
                    with open(fileName, "w") as mf:
                        mf.write(configFile)

def version_add_one():
    fileName = sys.path[0] + "/" + "../main.cpp"
    with open(fileName, "r",encoding='utf-8') as f:
        configFile = f.read()
        versionLeftIndex = configFile.find("#define FIRMWARE_BUILD_VERSION ")
        if versionLeftIndex == -1:
            return
        versionLeftIndex = versionLeftIndex + len(str("#define FIRMWARE_BUILD_VERSION "))
        versionRightIndex = configFile.find("\n",versionLeftIndex)
        oldVersion = configFile[versionLeftIndex:versionRightIndex]
        oldVersion = int(oldVersion)
        newVersion = oldVersion + 1
        configFile = configFile[:versionLeftIndex] + str(newVersion) + configFile[versionRightIndex:]
        versionLeftIndex = configFile.find("#define FIRMWARE_BUILD_DATETIME ")
        if versionLeftIndex == -1:
            return
        versionLeftIndex = versionLeftIndex + len(str("#define FIRMWARE_BUILD_DATETIME "))
        versionRightIndex = configFile.find("\n",versionLeftIndex)
        
        timeStr = time.strftime("%y%m%d", time.localtime())
        timeStr = hex(int(timeStr))
        configFile = configFile[:versionLeftIndex] + '"'+ str(timeStr)[2:] + '"' + configFile[versionRightIndex:]
        with open(fileName, "w") as f:
            f.write(configFile)
            

version_check()
version_add_one()