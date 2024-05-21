cc_library(
    name = "parser_lib",
    srcs = ["src/parser.cpp", "src/buffer_reader.cpp", "src/json.cpp"],
    hdrs = ["include/parser.h", "include/buffer_reader.h", "include/json.h"],
    includes = ["include"],
    copts = ["-std=c++20"],
)

cc_binary(
    name = "parser",
    srcs = ["src/main.cpp"],
    deps = [":parser_lib"],
    copts = ["-std=c++20"],
    # copts = select({
    #     "//conditions:default": [],
    #     "//build:debug": ["-g", "-O0"],
    #     "//build:release": ["-O3"],
    # }),
)

cc_test(
    name = "parser_test",
    srcs = ["tests/parser_test.cpp"],
    deps = [
        ":wc_lib",
        "@com_google_googletest//:gtest",
        "@com_google_googletest//:gtest_main",
        "@bazel_tools//tools/cpp/runfiles",
    ],
    data = ["//data:test_files",],
    copts = ["-std=c++20"],
)