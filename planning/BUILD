
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

cc_library(
  name = "generate_combat_command",
  hdrs = ["generate_combat_command.hh"],
  srcs = ["generate_combat_command.cc"],
  visibility = ["//interface:__subpackages__"],
  deps = [
    ":turn_planner",
    "//interface:game_state",
    "//sim/cards:bash",
    "//sim/cards:defend",
    "//sim/cards:strike",
  ]
)