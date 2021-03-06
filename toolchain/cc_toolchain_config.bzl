
load("@bazel_tools//tools/build_defs/cc:action_names.bzl", "ACTION_NAMES")
load("@bazel_tools//tools/cpp:cc_toolchain_config_lib.bzl", "feature", "flag_group", "flag_set", "tool_path")

def _impl(ctx):
    tool_paths = [
      tool_path(
        name = "gcc",
        path = "/usr/bin/clang-14",
      ),
      tool_path(
        name = "ar",
        path = "/usr/bin/ar",
      ),
      tool_path(
        name = "ld",
        path = "/usr/bin/ld",
      ),
      tool_path(
        name = "cpp",
        path = "/bin/false",
      ),
      tool_path(
        name = "gcov",
        path = "/bin/false",
      ),
      tool_path(
        name = "nm",
        path = "/bin/false",
      ),
      tool_path(
        name = "objdump",
        path = "/bin/false",
      ),
      tool_path(
        name = "strip",
        path = "/bin/false",
      ),
    ]
    all_link_actions = [
        ACTION_NAMES.cpp_link_executable,
        ACTION_NAMES.cpp_link_dynamic_library,
        ACTION_NAMES.cpp_link_nodeps_dynamic_library,
    ]
    all_compile_actions = [
        ACTION_NAMES.c_compile,
        ACTION_NAMES.cpp_compile,
    ]
    features = [
      feature(
        name = "default_linker_flags",
        enabled=True,
        flag_sets = [
          flag_set(
            actions = all_link_actions,
            flag_groups = [
              flag_group(
                flags=["-lstdc++", "-lm"],
              ),
            ],
          ),
        ],
      ),
      feature(
          name="warning_compile_flags",
          enabled=True,
          flag_sets = [
            flag_set(
              actions = all_compile_actions,
              flag_groups = [
                flag_group(
                  flags=["-Wall", "-Wextra", "-Wpedantic", "-Werror"]
                )
              ]
            )
          ]
      ),
      feature(
          name="cpp_compile_flags",
          enabled=True,
          flag_sets = [
            flag_set(
              actions = [ACTION_NAMES.cpp_compile],
              flag_groups = [
                flag_group(
                  flags=["-std=c++20"]
                )
              ]
            )
          ]
      ),
    ]

    return cc_common.create_cc_toolchain_config_info(
      ctx=ctx,
      features = features,
      cxx_builtin_include_directories = [
        "/usr/lib/llvm-14/lib/clang/14.0.0/include",
        "/usr/include",
      ],
      toolchain_identifier="k8-clang-toolchain",
      host_system_name="local",
      target_system_name="local",
      target_cpu="k8",
      target_libc="unknown",
      compiler="clang",
      abi_version="unknown",
      abi_libc_version="unknown",
      tool_paths = tool_paths
    )

cc_toolchain_config = rule(
    implementation = _impl,
    attrs = {},
    provides = [CcToolchainConfigInfo]
)