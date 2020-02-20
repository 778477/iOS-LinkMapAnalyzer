//
//  ObjectFile.h
//  LinkMapAnalyzer
//
//  Created by miaoyou.gmy on 2016/12/30.
//  Copyright © 2016年 miaoyou.gmy. All rights reserved.
//

#ifndef ObjectFile_h
#define ObjectFile_h

#include "Common.h"

class ObjectFile {
  public:
	string fileName;	   // 文件名字
	string belongModule;   // 所属模块
	unsigned fileNo;	   // 文件编号

	ObjectFile() {
		fileName = _unkown_file_name;
		belongModule = _unkown_module_name;
		fileNo = 0;
	}

	ObjectFile(string _fileName, string _belongModule, unsigned _no) {
		fileName = _fileName;
		belongModule = _belongModule;
		fileNo = _no;
	}
};

#endif /* ObjectFile_h */
