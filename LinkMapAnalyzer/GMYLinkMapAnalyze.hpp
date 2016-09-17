//
//  GMYLinkMapAnalyze.hpp
//  ACM
//
//  Created by miaoyou.gmy on 16/8/28.
//  Copyright © 2016年 miaoyou.gmy. All rights reserved.
//

#ifndef GMYLinkMapAnalyze_hpp
#define GMYLinkMapAnalyze_hpp


#include <string>
using namespace std;
/*
 # Path: LinkMap文件所在的目录
 
 # Arch: 编译构建的 cpu架构(指令集)
 
 step 1:
 # Object files:
 开始 记录 类(class - .o) 对应的文件编号 和 所属的模块(.a/.framework)
 */
class ObjectFile{
public:
    string fileName;    // 文件名字
    string belongModule; // 所属模块
    unsigned fileNo;// 文件编号
    
    ObjectFile(){
        fileName = "unkownFileName";
        belongModule = "unkownModuleName";
        fileNo = 0;
    }
    
    ObjectFile(string _fileName, string _belongModule, unsigned _no){
        fileName = _fileName;
        belongModule = _belongModule;
        fileNo = _no;
    }
};

typedef enum {
    linkMapUndefine = 0,        // 起始未定义
    linkMapPath = 10,           // 文件路径         # Path
    linkMapArch,                // cpu架构         # Arch
    linkMapObjectFiles,         // 编译文件         # Object files
    linkMapSections,            // 编译段落         # Sections
    linkMapSectionAddress,      // 段落地址,大小     # Address	Size    	Segment	Section
    linkMapSymbols,             // 编译符号         # Symbols
    linkMapSymbolAddress,       // 编译符号地址,大小  # Address	Size    	File  Name
}LinkMapType;

LinkMapType checkLinkMapSession(const char *buf);

#endif /* GMYLinkMapAnalyze_hpp */
