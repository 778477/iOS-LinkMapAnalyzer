//
//  Common.h
//  LinkMapAnalyzer
//
//  Created by miaoyou.gmy on 2016/12/30.
//  Copyright © 2016年 miaoyou.gmy. All rights reserved.
//

#ifndef Common_h
#define Common_h

#define MAX(a, b)                                                                                  \
	({                                                                                             \
		__typeof__(a) _a = a;                                                                      \
		__typeof__(b) _b = b;                                                                      \
		_a > _b ? _a : _b;                                                                         \
	})

#define MIN(a, b)                                                                                  \
	({                                                                                             \
		__typeof__(a) _a = a;                                                                      \
		__typeof__(b) _b = b;                                                                      \
		_a < _b ? _a : _b;                                                                         \
	})

#include <string>
using namespace std;
typedef long long _int64;
string const _unkown_file_name = "unknownFile";
string const _unkown_module_name = "unknownModule";

#endif /* Common_h */
