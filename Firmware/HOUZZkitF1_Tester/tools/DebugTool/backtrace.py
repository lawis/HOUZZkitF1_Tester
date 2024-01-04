#!/usr/bin/env python3

import os,sys
import re

CurrentPath = sys.path[0] + "/"

def printWithColor(printStr,colorId,showType = 0):
    print(printWithCorlStr(printStr,colorId,showType))

def printWithCorlStr(printStr,colorId,showType = 0):
    return ("\033["+str(showType)+";"+ str(colorId) +"m" + printStr +"\033[0m")

def resPaint(res,findWord,colorId,showType = 0):
    pattern = re.compile(findWord)
    results = pattern.findall(res)
    for result in results:
        res = res.replace(result,printWithCorlStr(result,colorId,showType))
    return res

def main():
    backtraceStackStr = input("请输入Crack Backtrace:")
    # printWithColor(backtraceStackStr,93)
    with open(CurrentPath+'debug.txt', "w") as f:
        f.write(backtraceStackStr)
        f.write('\n')
        f.close()
    cmd = "cd " + CurrentPath + "\n "
    cmd += "./esp32-backtrace.sh "+ CurrentPath +"../../.pio/build/esp32s3/firmware.elf debug.txt"
    print(cmd)
    cmdRes = os.popen(cmd)
    res = cmdRes.read()
    os.remove(CurrentPath+'debug.txt')

    res = resPaint(res,u"(BT-\d+:)",93,7)
    res = resPaint(res,u"(0x\w+ is in)",92)
    print(res)
    



if __name__=="__main__":
    main()