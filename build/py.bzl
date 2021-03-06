
load("@rules_python//python:defs.bzl", native_bin = "py_binary", native_lib = "py_library", native_test = "py_test")

def py_test(name, **kwargs):
    native_test(name = name,
        **kwargs,
    )

    native.sh_binary(
        name = name + "_repl",
        srcs = ["//build:run_repl.sh"],
        args = ["$(location {})".format(name)],
        data = [name],
    )

def py_library(name, **kwargs):
    native_lib(name = name,
        **kwargs,
    )

    native.sh_binary(
        name = name + "_repl",
        srcs = ["//build:run_repl.sh"],
        args = ["$(location {})".format(name)],
        data = [name],
    )

def py_binary(name, **kwargs):
    native_bin(name = name,
        **kwargs,
    )

    native.sh_binary(
        name = name + "_repl",
        srcs = ["//build:run_repl.sh"],
        args = ["$(location {})".format(name)],
        data = [name],
    )