#!/usr/bin/env python3

import os,sys

CurrentPath = sys.path[0]
ParentPath = os.path.abspath(os.path.dirname(CurrentPath))
TargetPath = ''

AppSizeArr = {'16016','808','12012','3208','32016'}

def readFile(fileName = ''):
	f = open(fileName)
	str = f.read()
	f.close()
	return str

def writeFile(fileName = '',fileStr = ''):
	f = open(fileName, "w")
	f.write(fileStr)
	f.close()

def createAppHFile(appName,appId):
    fileStr = readFile(CurrentPath + '/AppTemplate.ht')
    fileStr = fileStr.replace('<--Template-->',appName)
    fileStr = fileStr.replace('<--APPID-->',appId)
    writeFile(TargetPath + '/App'+ appName +'.h',fileStr)

def createAppCPPFile(appName):
    fileStr = readFile(CurrentPath + '/AppTemplate.cppt')
    fileStr = fileStr.replace('<--Template-->',appName)
    includeStr = ''
    for AppSizeStr in AppSizeArr:
        includeStr += '#include "App'+ appName + AppSizeStr + '.h"\n'
    fileStr = fileStr.replace('<--Include Subclass-->',includeStr)
    writeFile(TargetPath + '/App'+ appName +'.cpp',fileStr)

def createAppSizeHFile(appName,appSize):
    fileStr = readFile(CurrentPath + '/AppTemplateW0H.ht')
    fileStr = fileStr.replace('<--Template-->',appName)
    fileStr = fileStr.replace('<--W0H-->',appSize)
    writeFile(TargetPath + '/App'+ appName + appSize + '.h',fileStr)

def createAppSizeCPPFile(appName,appSize):
    fileStr = readFile(CurrentPath + '/AppTemplateW0H.cppt')
    fileStr = fileStr.replace('<--Template-->',appName)
    fileStr = fileStr.replace('<--W0H-->',appSize)
    writeFile(TargetPath + '/App'+ appName + appSize + '.cpp',fileStr)

def main():
    global TargetPath

    AppName = input("请输入App名称: ")
    AppID = input("请输入 APP ID: ")

    targetDir = "APP%05d" % int(AppID) + '_' + AppName
    TargetPath = ParentPath + '/' + targetDir
    if not os.path.exists(TargetPath):
        os.mkdir(TargetPath) 
    else:
        confirm = input("App文件夹已经存在，要覆盖吗(Y/N): ")
        if confirm != 'Y' :
            return 
    
    createAppHFile(AppName,AppID)
    createAppCPPFile(AppName)
    for AppSizeStr in AppSizeArr:
        createAppSizeHFile(AppName,AppSizeStr)
        createAppSizeCPPFile(AppName,AppSizeStr)


if __name__=="__main__":
	main()