[iOS 包瘦身浅析](http://778477.github.io/2016/09/01/2016-09-01-iOS%20%E5%8C%85%E7%98%A6%E8%BA%AB%E7%A0%94%E7%A9%B6/)


```
typedef enum {
    linkMapUndefine = 0,        // 起始未定义
    linkMapPath = 10,           // 文件路径         # Path
    linkMapArch,                // cpu架构         # Arch
    linkMapObjectFiles,         // 编译文件         # Object files
    linkMapSections,            // 编译段落         # Sections
    linkMapSectionAddress,      // 段落地址,大小     # Address    Size        Segment Section
    linkMapSymbols,             // 编译符号          # Symbols
    linkMapSymbolAddress,       // 编译符号地址,大小  # Address	Size    	File  Name
}LinkMapSessionType;

```


 
 
 * step 1
 Count Object Files
 开始 记录 类(class - .o) 对应的文件编号 和 所属的模块(.a/.framework)
 
 * step 2
 generate Module Map
 生成 target project中编译构建所引用的所有模块
 
 * step 3
 analyze Symbols Address
 分析 符号地址 计算对应的符号文件大小
 
 * step 4
 generate Module Size
 遍历生成 所有模块的大小 顺序输出

## How to use

使用编译之后产物 `LinkMapAnalyzer`目录下 在命令行中运行。 后面指明要分析的linkMap文件地址

`./LinkMapAnalyzer /Users/guomiaoyou/Library/Developer/Xcode/DerivedData/GAG-focyaelrlonzsogsgfnrfxjywimo/Build/Intermediates/GAG.build/Debug-iphonesimulator/GAG.build/GAG-LinkMap-normal-x86_64.txt`


## Example output：

```
libAFNetworking.a                                                       317.37KB
libSDWebImage.a                                                         198.61KB
GAG                                                                      68.55KB
libSVPullToRefresh.a                                                     43.76KB
libarclite_iphonesimulator.a                                              7.38KB
unknownModule                                                             2.18KB
libPods-GAG.a                                                             0.25KB
```