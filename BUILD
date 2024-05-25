cc_library(
    name = "json_lib",
    srcs = ["src/json.cpp"],
    hdrs = ["include/json.h"],
    includes = ["include"],
    copts = ["-std=c++20"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "parser_lib",
    srcs = ["src/parser.cpp", "src/buffer_reader.cpp"],
    hdrs = ["include/parser.h", "include/buffer_reader.h"],
    deps = ["json_lib"],
    includes = ["include"],
    copts = ["-std=c++20"],
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "parser",
    srcs = ["src/main.cpp"],
    deps = [":parser_lib"],
    includes = ["include"],
    copts = ["-std=c++20"],
    # copts = select({
    #     "//conditions:default": [],
    #     "//build:debug": ["-g", "-O0"],
    #     "//build:release": ["-O3"],
    # }),
)