
package(default_visibility=["//sim:__subpackages__"])

cc_library(
  name = "combat",
  visibility = ["//visibility:public"],
  hdrs = ["combat.hh"],
  srcs = ["combat.cc"],
)

cc_library(
  name = "combat_updater",
  hdrs = ["combat_updater.hh"],
  srcs = ["combat_updater.cc"],
  deps = [
    ":combat",
  ]
)

cc_test(
  name = "combat_updater_test",
  srcs = ["combat_updater_test.cc"],
  deps = [
    ":combat_updater",
    "@com_google_googletest//:gtest_main",
  ]
)