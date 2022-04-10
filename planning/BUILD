
cc_library(
  name = "turn_planner",
  hdrs = ["turn_planner.hh"],
  srcs = ["turn_planner.cc"],
  deps = [
    "//sim:combat",
  ]
)

cc_test(
  name = "turn_planner_test",
  srcs = ["turn_planner_test.cc"],
  deps = [
    ":turn_planner",
    "//sim/cards:strike",
    "//sim/cards:defend",
    "@com_google_googletest//:gtest_main",
  ]
)