
cc_library(
  name = "constants",
  hdrs = ["constants.hh"],
)

cc_binary(
  name = "bridge",
  srcs = ["bridge.cc"],
  deps = [":constants"],
  linkopts=["-lpthread", "-lzmqpp", "-lzmq"],
)

cc_binary(
  name = "bot_main",
  srcs = ["bot_main.cc"],
  deps = [
    ":constants",
    "@com_nlohmann_json//:json",
    ":game_state_from_json",
    ":generate_command",
  ],
  linkopts=["-lpthread", "-lzmqpp", "-lzmq"],
)

cc_library(
  name = "game_state",
  hdrs = ["game_state.hh"],
)

cc_library(
  name = "game_state_from_json",
  hdrs = ["game_state_from_json.hh"],
  srcs = ["game_state_from_json.cc"],
  deps = [
  ":game_state",
   "@com_nlohmann_json//:json",
  ]
)

cc_test(
  name = "game_state_from_json_test",
  srcs = ["game_state_from_json_test.cc"],
  deps = [
    ":game_state_from_json",
    "@com_google_googletest//:gtest_main",
  ]
)

cc_library(
  name = "generate_command",
  hdrs = ["generate_command.hh"],
  srcs = ["generate_command.cc"],
  deps = [
    ":game_state",
  ]
)