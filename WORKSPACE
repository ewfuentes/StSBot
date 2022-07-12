load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "bazel_skylib",
    urls = [
        "https://mirror.bazel.build/github.com/bazelbuild/bazel-skylib/releases/download/1.2.1/bazel-skylib-1.2.1.tar.gz",
        "https://github.com/bazelbuild/bazel-skylib/releases/download/1.2.1/bazel-skylib-1.2.1.tar.gz",
    ],
    sha256 = "f7be3474d42aae265405a592bb7da8e171919d74c16f082a5457840f06054728",
)
load("@bazel_skylib//:workspace.bzl", "bazel_skylib_workspace")
bazel_skylib_workspace()

http_archive(
    name = "rules_cc",
    strip_prefix = "rules_cc-8bb0eb5c5ccd96b91753bb112096bb6993d16d13",
    sha256 = "8cd53b3563a3f66daa1f06f97b78a35600986a547bb27bebcafb8b1f8f54efb0",
    urls = ["https://github.com/bazelbuild/rules_cc/archive/8bb0eb5c5ccd96b91753bb112096bb6993d16d13.zip"],
)
load("@rules_cc//cc:repositories.bzl", "rules_cc_dependencies", "rules_cc_toolchains")
rules_cc_dependencies()
rules_cc_toolchains()

http_archive(
    name = "rules_python",
    sha256 = "56dc7569e5dd149e576941bdb67a57e19cd2a7a63cc352b62ac047732008d7e1",
    strip_prefix = "rules_python-0.10.0",
    url = "https://github.com/bazelbuild/rules_python/archive/refs/tags/0.10.0.tar.gz",
)

load("@rules_python//python:repositories.bzl", "python_register_toolchains")
python_register_toolchains(name="python3_10", python_version="3.10.4")


http_archive(
    name = "rules_proto",
    sha256 = "e017528fd1c91c5a33f15493e3a398181a9e821a804eb7ff5acdd1d2d6c2b18d",
    strip_prefix = "rules_proto-4.0.0-3.20.0",
    urls = [
        "https://github.com/bazelbuild/rules_proto/archive/refs/tags/4.0.0-3.20.0.tar.gz",
    ],
)
load("@rules_proto//proto:repositories.bzl", "rules_proto_dependencies", "rules_proto_toolchains")
rules_proto_dependencies()
rules_proto_toolchains()

http_archive(
  name = "com_google_googletest",
  urls = ["https://github.com/google/googletest/archive/5f467ec04df33024e3c6760fa403b5cd5d8e9ace.zip"],
  strip_prefix = "googletest-5f467ec04df33024e3c6760fa403b5cd5d8e9ace",
  sha256 = "aff2e98fd8fb11becd00d3820f33839eb16370c82693bb8552118c93d963773a"
)

http_archive(
  name = "com_nlohmann_json",
  urls = ["https://github.com/nlohmann/json/archive/refs/tags/v3.10.5.zip"],
  strip_prefix = "json-3.10.5",
  sha256 = "ea4b0084709fb934f92ca0a68669daa0fe6f2a2c6400bf353454993a834bb0bb",
  build_file="@//third_party:json.BUILD",
)

