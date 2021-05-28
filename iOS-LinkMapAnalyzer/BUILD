load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_binary(
    name = "LinkMapAnalyzer",
    srcs = ["main.cpp"],
    deps = [
        ":LinkMapAnalyzerModule",
    ]
)

cc_library(
    name = "LinkMapAnalyzerModule",
    srcs = ["LinkMapAnalyzer.cpp"],
    hdrs = ["ObjectFile.h", "ObjectModule.h", "LinkMapSessionDefine.h", "LinkMapAnalyzer.h", "Debug.h", "Common.h"],
)