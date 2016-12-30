//
//  linkMapAnalyzer.cpp
//  LinkMapAnalyzer
//
//  Created by miaoyou.gmy on 2016/12/30.
//  Copyright © 2016年 miaoyou.gmy. All rights reserved.
//

#include "LinkMapAnalyzer.h"

#include <iostream>
using namespace std;


/**
 *  IO 按行读入buf，分析当前行所在段落意义
 *
 *  @param buf     当前行 内容
 *  @param preType 上一段落意义
 *
 *  @return 当前行段落意义
 */
LinkMapSessionType LinkMapAnalyzer::checkLinkMapSession(const char *buf, const LinkMapSessionType preType){
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

/**
 *  IO 按行读入buf，分析当前行包含的符号文件信息
 *
 *  @param buf        当前行 内容
 *  @param targetName 编译项目名称
 *
 *  @return 符号文件信息
 */
ObjectFile* LinkMapAnalyzer::analyzeObjectFileFromBuf(const char *buf, const string targetName){
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
    
    objectFiles.push_back(ofile);
    return ofile;
}
/**
 *  生成 符号文件所在的模块
 */
void LinkMapAnalyzer::generateObjectModules(){
    for_each(objectFiles.begin(), objectFiles.end(), [&](const ObjectFile *file){
        if(objectModulesMap.find(file->belongModule) == objectModulesMap.end()){
            objectModules.push_back(new ObjectModule(file->belongModule,0));
            objectModulesMap[file->belongModule] = objectModules.size() - 1;
        }
    });
    
    
}
/**
 *  按行读入buf，分析当前行包含的符号地址信息 - 计算符号文件大小
 *
 *  @param buf 当前行 内容
 */
void LinkMapAnalyzer::analyzeSymbolsAddressFromBuf(const char *buf){
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

/**
 *  输出 分析结果
 */
void LinkMapAnalyzer::output(){
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

/**
 *  格式化 输出
 */
void LinkMapAnalyzer::formatoutput(const string name,double size,const string sizeUnit,const double maxSize){
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

#pragma mark - Debug output
/**
 *  检查是否存在未知模块
 */
void LinkMapAnalyzer::ckeckUnkownObjectFile(){
    for_each(objectFiles.begin(), objectFiles.end(), [&](const ObjectFile *file){
        if(file->fileName == _unkown_file_name || file->belongModule == _unkown_module_name){
            cout<<"[debug]: "<<file->fileNo<<"-"<<file->fileName<<"-"<<file->belongModule<<endl;
        }
    });
}
