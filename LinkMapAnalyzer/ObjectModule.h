//
//  ObjectModule.h
//  LinkMapAnalyzer
//
//  Created by miaoyou.gmy on 2016/12/30.
//  Copyright © 2016年 miaoyou.gmy. All rights reserved.
//

#ifndef ObjectModule_h
#define ObjectModule_h

#include "Common.h"

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

#endif /* ObjectModule_h */
