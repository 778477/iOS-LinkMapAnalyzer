//
//  linkMapAnalyzer.hpp
//  LinkMapAnalyzer
//
//  Created by miaoyou.gmy on 2016/12/30.
//  Copyright © 2016年 miaoyou.gmy. All rights reserved.
//

#ifndef LinkMapAnalyzer_h
#define LinkMapAnalyzer_h

#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
/* STL */
#include <vector>
#include <queue>
#include <set>
#include <list>
#include <stack>
#include <map>
#include <set>

#include "ObjectFile.h"
#include "ObjectModule.h"
#include "LinkMapSessionDefine.h"
using namespace std;

class LinkMapAnalyzer{
public:
    vector<ObjectFile *> objectFiles{4096}; // 装载 符号文件(.o file) 容器
    _int64 *SymoblCountSize;                // 符号文件size大小 index: file's number val: file's Size
    map<string,_int64> objectModulesMap;    // 所属模块map  hashKey: module's name hashVal: module' index
    vector<ObjectModule *> objectModules{}; // 装载 符号文件(.o file)对应的所属模块 容器
    
    LinkMapAnalyzer(){
        objectFiles.clear();
    }
    ~LinkMapAnalyzer(){
        free(SymoblCountSize);
    }
    
    LinkMapSessionType checkLinkMapSession(const char *buf, const LinkMapSessionType preType);
    ObjectFile *analyzeObjectFileFromBuf(const char *buf, const string targetName);
    void ckeckUnkownObjectFile();
    void generateObjectModules();
    void analyzeSymbolsAddressFromBuf(const char *buf);
    void output();
private:
    void formatoutput(const string name,double size,const string sizeUnit);

};




#endif /* linkMapAnalyzer_h */
