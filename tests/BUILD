cc_test(
    name = "json_test",
    srcs = ["//tests:json_test.cpp"],
    deps = [
        "//:json_lib",
        "@com_google_googletest//:gtest",
        "@com_google_googletest//:gtest_main",
        "@bazel_tools//tools/cpp/runfiles",
        "@com_github_nlohmann_json//:nlohmann_json",
    ],
    copts = ["-std=c++20", "-g", "-fno-omit-frame-pointer", "-fno-inline"],
)

cc_test(
    name = "parser_test",
    srcs = ["//tests:parser_test.cpp"],
    deps = [
        "//:json_lib",
        "//:parser_lib",
        "@com_google_googletest//:gtest",
        "@com_google_googletest//:gtest_main",
        "@bazel_tools//tools/cpp/runfiles",
    ],
    copts = ["-std=c++20", "-g", "-fno-omit-frame-pointer", "-fno-inline"],
)