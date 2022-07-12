
package(default_visibility=["//planning:__subpackages__"])
load("@pip_deps//:requirements.bzl", "requirement")
load("@//build:proto.bzl", "multi_proto_library")
load("@//build:py.bzl", "py_test")

filegroup(
  name = "cards_pbtxt",
  srcs = ["ironclad.pbtxt"],
)

cc_library(
  name = "get_unique_id",
  hdrs = ["get_unique_id.hh"],
  srcs = ["get_unique_id.cc"],
)

cc_library(
  name = "defend",
  hdrs = ["defend.hh"],
  deps = [
  "//sim:combat",
  "//sim:combat_updater",
  ":get_unique_id",
  ],
)

cc_test(
  name = "defend_test",
  srcs = ["defend_test.cc"],
  deps = [
    ":defend",
    "@com_google_googletest//:gtest_main",
  ],
)

cc_library(
  name = "strike",
  hdrs = ["strike.hh"],
  deps = [
    "//sim:combat",
    "//sim:combat_updater",
    ":get_unique_id",
  ],
)

cc_test(
  name = "strike_test",
  srcs = ["strike_test.cc"],
  deps = [
    ":strike",
    "@com_google_googletest//:gtest_main",
  ],
)

cc_library(
  name = "bash",
  hdrs = ["bash.hh"],
  deps = [
    "//sim:combat",
    "//sim:combat_updater",
    ":get_unique_id",
  ],
)

multi_proto_library(
    name = "card_proto",
    srcs = ["card.proto",],
    deps = [":effect_proto",],
)

multi_proto_library(
    name = "effect_proto",
    srcs = ["effect.proto",],
)

py_test(
    name = "cards_proto_test",
    srcs = ["cards_proto_test.py"],
    data = [":cards_pbtxt"],
    deps = [
      ":card_proto_py",
      ":effect_proto_py",
      requirement("protobuf"),
    ]
)