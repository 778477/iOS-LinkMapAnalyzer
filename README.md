
## 为什么要做包瘦身？

[Submitting the App to App Review](https://developer.apple.com/library/content/documentation/LanguagesUtilities/Conceptual/iTunesConnect_Guide/Chapters/SubmittingTheApp.html)

>Your app’s total uncompressed size must be less than 4GB. Each Mach-O executable file (for example, > app_name.app/app_name) must not exceed these limits:

>For apps whose MinimumOSVersion is less than 7.0: maximum of 80 MB for the total of all __TEXT sections in the binary.

>For apps whose MinimumOSVersion is 7.x through 8.x: maximum of 60 MB per slice for the __TEXT section of each architecture slice in the binary.

>For apps whose MinimumOSVersion is 9.0 or greater: maximum of 500 MB for the total of all __TEXT sections in the binary.

[iOS 包瘦身浅析](http://778477.github.io/2016/09/01/2016-09-01-iOS%20%E5%8C%85%E7%98%A6%E8%BA%AB%E7%A0%94%E7%A9%B6/)


## How to use

使用项目目录里`LinkMapAnalyzer`可执行文件，在命令行中运行。
后面指明要分析的linkMap文件地址

`./LinkMapAnalyzer ./GAG-LinkMap-normal-x86_64.txt`

也可以将输出重定向保存为其他文件 如：

`./LinkMapAnalyzer ./GAG-LinkMap-normal-x86_64.txt > out.txt`


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
