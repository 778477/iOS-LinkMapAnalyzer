//
//  LinkMapSessionDefine.h
//  LinkMapAnalyzer
//
//  Created by miaoyou.gmy on 2016/12/30.
//  Copyright © 2016年 miaoyou.gmy. All rights reserved.
//

#ifndef LinkMapSessionDefine_h
#define LinkMapSessionDefine_h

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



#endif /* LinkMapSessionDefine_h */
