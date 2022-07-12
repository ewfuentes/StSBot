load("@rules_proto//proto:defs.bzl", "proto_library")
load("@rules_cc//cc:defs.bzl", "cc_library", "cc_proto_library")
load("@com_google_protobuf//:protobuf.bzl", "py_proto_library")

def multi_proto_library(name, **kwargs):
    deps = kwargs.get("deps", {})

    raw_proto_name = lambda x: x + "__raw"
    py_proto_name = lambda x: x + "_py"
    proto_library(
      name = raw_proto_name(name),
      srcs = kwargs["srcs"],
      deps = [raw_proto_name(d) for d in deps],
    )

    cc_proto_library(
      name = name,
      deps = [raw_proto_name(name)],
    )
    
    py_proto_library(
      name = name + "_py",
      srcs = kwargs["srcs"],
      deps = [py_proto_name(d) for d in deps]
    )