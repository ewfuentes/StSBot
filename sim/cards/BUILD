
package(default_visibility=["//planning:__subpackages__"])
load("@rules_cc//cc:defs.bzl", "cc_library", "cc_proto_library")

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

proto_library(
    name = "card_proto",
    srcs = ["card.proto"],
    deps = [":effect_proto"],
)

proto_library(
    name = "effect_proto",
    srcs = ["effect.proto"],
)

cc_proto_library(
    name = "effect_cc_proto",
    deps = [":effect_proto"],
)

cc_proto_library(
    name = "card_cc_proto",
    deps = [":card_proto"],
)
