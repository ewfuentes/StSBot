
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
  deps = [":constants"],
  linkopts=["-lpthread", "-lzmqpp", "-lzmq"],
)