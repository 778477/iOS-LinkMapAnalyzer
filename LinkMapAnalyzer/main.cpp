//
//  main.cpp
//
//  Created by miaoyou.gmy on 15/2/N.
//  Copyright (c) 2015年 miaoyou.gmy. All rights reserved.
//

/* base */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string>
#include <algorithm>
#include <functional>


/* STL */
#include <vector>
#include <queue>
#include <set>
#include <list>
#include <stack>
#include <map>
#include <set>


/* IO */
#include <fstream>
#include <iostream>
using namespace std;

#define MAX(a,b)                          \
    ({ __typeof__ (a) _a = a;             \
       __typeof__ (b) _b = b;             \
       _a > _b ? _a : _b;})



#define MIN(a,b)                          \
    ({ __typeof__ (a) _a = a;             \
       __typeof__ (b) _b = b;             \
       _a < _b ? _a : _b;})


// debug 开关 取消注释将会打印debug信息
//#define _debug_ 1

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
class ObjectFile;
class ObjectModule;

string const _unkown_file_name = "unknownFile";
string const _unkown_module_name = "unknownModule";

typedef long long _int64;
const unsigned BUFMAXLEN(1024*10);      // IO 读入缓冲区大小
vector<ObjectFile *> objectFiles{4096}; // 装载 符号文件(.o file) 容器
_int64 *SymoblCountSize;                // 符号文件size大小 index: file's number val: file's Size
map<string,_int64> objectModulesMap;    // 所属模块map  hashKey: module's name hashVal: module' index
vector<ObjectModule *> objectModules{}; // 装载 符号文件(.o file)对应的所属模块 容器



class ObjectFile{
public:
    string fileName;    // 文件名字
    string belongModule; // 所属模块
    unsigned fileNo;// 文件编号
    
    ObjectFile(){
        fileName = _unkown_file_name;
        belongModule = _unkown_module_name;
        fileNo = 0;
    }
    
    ObjectFile(string _fileName, string _belongModule, unsigned _no){
        fileName = _fileName;
        belongModule = _belongModule;
        fileNo = _no;
    }
};

class ObjectModule{
public:
    string moduleName;
    _int64 moduleSize;
    ObjectModule(){
        moduleName = _unkown_module_name;
        moduleSize = 0;
    }
    
    ObjectModule(string _moduleName, _int64 size = 0){
        moduleName = _moduleName;
        moduleSize = size;
    }
};



typedef enum {
    linkMapUndefine = 0,        // 起始未定义
    linkMapPath = 10,           // 文件路径         # Path
    linkMapArch,                // cpu架构         # Arch
    linkMapObjectFiles,         // 编译文件         # Object files
    linkMapSections,            // 编译段落         # Sections
    linkMapSectionAddress,      // 段落地址,大小     # Address    Size        Segment Section
    linkMapSymbols,             // 编译符号         # Symbols
    linkMapSymbolAddress,       // 编译符号地址,大小  # Address	Size    	File  Name
}LinkMapSessionType;




/**
 *  IO 按行读入buf，分析当前行所在段落意义
 *
 *  @param buf     当前行 内容
 *  @param preType 上一段落意义
 *
 *  @return 当前行段落意义
 */
LinkMapSessionType checkLinkMapSession(const char *buf, const LinkMapSessionType preType);
/**
 *  IO 按行读入buf，分析当前行包含的符号文件信息
 *
 *  @param buf        当前行 内容
 *  @param targetName 编译项目名称
 *
 *  @return 符号文件信息
 */
ObjectFile *analyzeObjectFileFromBuf(const char *buf, const string targetName);

/**
 *  检查是否存在未知模块
 */
void ckeckUnkownObjectFile();
/**
 *  生成 符号文件所在的模块
 */
void generateObjectModules();
/**
 *  按行读入buf，分析当前行包含的符号地址信息 - 计算符号文件大小
 *
 *  @param buf 当前行 内容
 */
void analyzeSymbolsAddressFromBuf(const char *buf);

/**
 *  输出 分析结果
 */
void output();

/**
 *  格式化 输出
 */
void formatoutput(const string name,double size,const string sizeUnit,const double maxSize);

int main(int argc,char *argv[]){
    clock_t beginTime = clock();

#ifndef _debug_
    if(argc < 2){
        cout<<"Please give a LinkMap file path.\n[ex. ./LinkMapAnalyzer ~/Library/xx-LinkMap-normal-x86_64.txt]"<<endl;
        return 0;
    }
    ifstream inFile(argv[1]);
#else
    //输入 重定向 load linkMap file
    const string inputFileName = "/Users/guomiaoyou/Documents/778477/LinkMapAnalyzer/LinkMapAnalyzer/in.txt";
    ifstream inFile(inputFileName);
    //输出 重定向
    freopen("/Users/guomiaoyou/Documents/778477/LinkMapAnalyzer/LinkMapAnalyzer/output.txt", "w", stdout);
#endif

    if(!inFile){
        cout<<"open linkMap file failed..."<<endl;
        return 0;
    }
    
    char buf[BUFMAXLEN] = {'\0'};
    objectFiles.clear();
    
    string target = _unkown_module_name;
    
    
    LinkMapSessionType type = linkMapUndefine;
    _int64 lineCnt = 0;
    while(inFile.getline(buf, BUFMAXLEN - 1, '\n')){
        LinkMapSessionType currentType = checkLinkMapSession(buf,type);
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
                    objectFiles.push_back(analyzeObjectFileFromBuf(buf,target));
                }
                break;
            case linkMapSections:{
#ifdef _debug_
                ckeckUnkownObjectFile();
#endif
                generateObjectModules();
                SymoblCountSize = (_int64 *)malloc(sizeof(_int64) * objectFiles.size() + 1);
                memset(SymoblCountSize, 0, sizeof(_int64) * objectFiles.size() + 1);
                break;
            }
            case linkMapSymbolAddress:
                if(currentType == linkMapUndefine){
                    analyzeSymbolsAddressFromBuf(buf);
                }
                
                break;
            default:
                break;
        }
    }
#ifdef _debug_
    cout<<"[debug]: read file line count = "<<lineCnt<<endl;
#endif
    output();
    free(SymoblCountSize);
    
    
    clock_t endTime = clock();
    cout<<"[-] Cast Time : "<<(endTime - beginTime)/1000<<endl;
    
    inFile.close();
    return 0;
}


LinkMapSessionType checkLinkMapSession(const char *buf, const LinkMapSessionType preType){
    
    if(buf[0] != '#') return linkMapUndefine;
    if(strstr(buf, "# Path") != NULL) return linkMapPath;
    if(strstr(buf, "# Arch") != NULL) return linkMapArch;
    if(strstr(buf, "# Object") != NULL) return linkMapObjectFiles;
    if(strstr(buf, "# Sections") != NULL) return linkMapSections;
    if(strstr(buf, "# Symbols") != NULL) return linkMapSymbols;
    if(strstr(buf, "# Address") != NULL){
        if(preType == linkMapSections) return linkMapSectionAddress;
        
        if(preType == linkMapSymbols) return linkMapSymbolAddress;
    }
    
    return linkMapUndefine;
}

ObjectFile *analyzeObjectFileFromBuf(const char *buf, const string targetName){
    ObjectFile *ofile = new ObjectFile();
    size_t len = strlen(buf);
    unsigned no = 0;
    bool hasNo = false;
    
    // 1.analyze get the file's number
    for(unsigned i = 1; i < len ; i++){
        if(buf[i]>='0' && buf[i]<='9'){
            no = no*10 + (buf[i] - '0');
            hasNo = true;
        }
        else if(hasNo){
            break;
        }
    }
    ofile->fileNo = no;
    
    // 2. find the file's name
    if(buf[len - 1] == ')'){
        char *p = index(buf,'(');
        size_t pLen = strlen(p);
        ++p,p[pLen - 2] = '\0';
        ofile->fileName = p;
        
        // 3. find the file's module name
        char *s = rindex(buf, '/');
        size_t sLen = strlen(s);
        ++s,s[sLen - pLen] = '\0';
        ofile->belongModule = s;
    }
    else{
        char *p = rindex(buf, '/');
        if(p != NULL){
            ofile->fileName = ++p;
            ofile->belongModule = targetName;  // 3. set the default module name(build target name)
        }
    }
    
    return ofile;
}

void ckeckUnkownObjectFile(){
    for_each(objectFiles.begin(), objectFiles.end(), [](const ObjectFile *file){
        if(file->fileName == _unkown_file_name || file->belongModule == _unkown_module_name){
            cout<<"[debug]: "<<file->fileNo<<"-"<<file->fileName<<"-"<<file->belongModule<<endl;
        }
    });
}

void generateObjectModules(){
    for_each(objectFiles.begin(), objectFiles.end(), [](const ObjectFile *file){
        if(objectModulesMap.find(file->belongModule) == objectModulesMap.end()){
            objectModules.push_back(new ObjectModule(file->belongModule,0));
            objectModulesMap[file->belongModule] = objectModules.size() - 1;
        }
    });
}

void analyzeSymbolsAddressFromBuf(const char *buf){
    _int64 starAdd = 0, symSize = 0, no = 0;
    sscanf(buf, "%llx\t%llx\t",&starAdd,&symSize);
    char *p = index(buf, '[');
    bool hasno = false;
    
    while(p++){
        if(*p >= '0' && *p <= '9'){
            hasno = true;
            no = no * 10 + (*p - '0');
        }
        else if(hasno){
            break;
        }
    }
    
    SymoblCountSize[no] += symSize;
}


void output(){
    _int64 totalSize = 0;
    for_each(objectFiles.begin(), objectFiles.end(), [&](const ObjectFile *file){
        if(SymoblCountSize[file->fileNo]){
            totalSize += SymoblCountSize[file->fileNo];
            objectModules[objectModulesMap[file->belongModule]]->moduleSize += SymoblCountSize[file->fileNo];
        }
    });
    
    
    sort(objectModules.begin(), objectModules.end(), [](const ObjectModule *a,const ObjectModule *b){
        return a->moduleSize > b->moduleSize;
    });
    
    totalSize = 0;
    double maxSize = objectModules.size() >= 1 ? objectModules[0]->moduleSize*1.0/1024 : 0.0;
    for_each(objectModules.begin(), objectModules.end(), [&](const ObjectModule *module){
        formatoutput(module->moduleName, module->moduleSize*1.0/1024, "KB",maxSize);
        totalSize+=module->moduleSize;
    });
    
    cout<<"\n\n\n"<<"[-] totalSize : "<<totalSize*1.0/(1024*1024)<<"MB"<<endl;
}


void formatoutput(const string name,double size,const string sizeUnit,const double maxSize){
    const long limLen = 80;
    char *sizeStr = (char *)malloc(sizeof(char) * 1024);
    memset(sizeStr, '\0', sizeof(char) * 1024);
    sprintf(sizeStr, "%.2lf",size);
    long spaceLen = limLen - name.size() - strlen(sizeStr) - sizeUnit.size();
    
    cout<<name;
    for(int i=0;i<spaceLen;i++){
        cout<<" ";
    }
    cout<<sizeStr<<sizeUnit<<endl;
    
    free(sizeStr);
}
