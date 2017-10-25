//
//  main.cpp
//
//  Created by miaoyou.gmy on 15/2/N.
//  Copyright (c) 2015年 miaoyou.gmy. All rights reserved.
//

/* base */
#include <string.h>
#include <math.h>
#include <time.h>
#include <algorithm>
#include <functional>
/* IO */
#include <fstream>
#include <iostream>
#include "Debug.h"
#include "LinkMapAnalyzer.h"
using namespace std;
const unsigned BUFMAXLEN(1024*10);      // IO 读入缓冲区大小
/*
 # Path: LinkMap文件所在的目录
 # Arch: 编译构建的 cpu架构(指令集)
 
 step 1:
 # Count Object Files:
 开始 记录 类(class - .o) 对应的文件编号 和 所属的模块(.a/.framework)
 
 step 2:
 # generate Module Map
 生成 target project中编译构建所引用的所有模块
 
 step 3:
 # analyze Symbols Address
 分析 符号地址 计算对应的符号文件大小
 
 step 4:
 # generate Module Size
 遍历生成 所有模块的大小 顺序输出
 */

int main(int argc,char *argv[]){
    LinkMapAnalyzer *analyazer = new LinkMapAnalyzer();
    
    clock_t beginTime = clock();

#ifndef _debug_
    if(argc < 2){
        cout<<"Please give a LinkMap file path.\n[ex. ./LinkMapAnalyzer ~/Library/xx-LinkMap-normal-x86_64.txt]"<<endl;
        return 0;
    }
    ifstream inFile(argv[1]);
#else
    //输入 重定向 load linkMap file
    const string inputFileName = "/Users/guomiaoyou/Documents/github/iOS-LinkMapAnalyzer/iOS-LinkMapAnalyzer/in.txt";
    ifstream inFile(inputFileName);
    //输出 重定向
    freopen("/Users/guomiaoyou/Documents/github/iOS-LinkMapAnalyzer/iOS-LinkMapAnalyzer/out.txt", "w", stdout);
#endif

    if(!inFile){
        cout<<"open linkMap file failed..."<<endl;
        return 0;
    }
    
    char buf[BUFMAXLEN] = {'\0'};
    string target = _unkown_module_name;
    LinkMapSessionType type = linkMapUndefine;
    _int64 lineCnt = 0;
    while(inFile.getline(buf, BUFMAXLEN - 1, '\n')){
        LinkMapSessionType currentType = analyazer->checkLinkMapSession(buf,type);
        if(currentType != linkMapUndefine) type = currentType;
        lineCnt++;
        
        switch (type) {
            case linkMapPath:{
#ifdef _debug_
                printf("%s\n",buf);
#endif
                char *p = rindex(buf, '/');
                ++p;
                target = p;
                break;
            }
            case linkMapArch:
#ifdef _debug_
                printf("%s\n",buf);
#endif
                break;
            case linkMapObjectFiles:
                if(currentType == linkMapUndefine){
                    analyazer->analyzeObjectFileFromBuf(buf,target);
                }
                break;
            case linkMapSections:{
#ifdef _debug_
                analyazer->ckeckUnkownObjectFile();
#endif
                analyazer->generateObjectModules();
                analyazer->SymoblCountSize = (_int64 *)malloc(sizeof(_int64) * analyazer->objectFiles.size() + 1);
                memset(analyazer->SymoblCountSize, 0, sizeof(_int64) * analyazer->objectFiles.size() + 1);
                break;
            }
            case linkMapSymbolAddress:
                if(currentType == linkMapUndefine){
                    analyazer->analyzeSymbolsAddressFromBuf(buf);
                }
                
                break;
            default:
                break;
        }
    }
#ifdef _debug_
    cout<<"[debug]: read file line count = "<<lineCnt<<endl;
#endif
    analyazer->output();
    delete analyazer;
    
    
    clock_t endTime = clock();
    cout<<"[-] Cast Time : "<<(endTime - beginTime)/1000<<endl;
    
    inFile.close();
    return 0;
}













