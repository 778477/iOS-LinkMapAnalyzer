//
//  GMYLinkMapAnalyze.cpp
//  ACM
//
//  Created by miaoyou.gmy on 16/8/28.
//  Copyright © 2016年 miaoyou.gmy. All rights reserved.
//

#include "GMYLinkMapAnalyze.hpp"


LinkMapType checkLinkMapSession(const char *buf){
    
    if(buf[0] != '#') return linkMapUndefine;
    
    if(strstr(buf, "# Path") != NULL) return linkMapPath;
    if(strstr(buf, "# Arch") != NULL) return linkMapArch;
    
    
    return linkMapUndefine;
}