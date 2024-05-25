workspace(name = "custom_json_parser")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "com_google_googletest",
    urls = [
        "https://github.com/google/googletest/archive/release-1.11.0.tar.gz",
    ],
    strip_prefix = "googletest-release-1.11.0",
)

http_archive(
    name = "com_github_nlohmann_json",
    sha256 = "a22461d13119ac5c78f205d3df1db13403e58ce1bb1794edc9313677313f4a9d",
    urls = [ 
        "https://github.com/nlohmann/json/releases/download/v3.11.3/include.zip",
    ],
    build_file = "//external:nlohmann_json.BUILD",
)

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "com_github_nelhage_rules_boost",
    url = "https://github.com/nelhage/rules_boost/archive/5d04542e52164931841d06d5a6b3fd2f43c436d0.tar.gz",
    strip_prefix = "rules_boost-5d04542e52164931841d06d5a6b3fd2f43c436d0",
)

load("@com_github_nelhage_rules_boost//:boost/boost.bzl", "boost_deps")
boost_deps()