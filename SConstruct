#!/usr/bin/env python
from glob import glob
from pathlib import Path
import SCons
import shutil
import os
import subprocess
import sys

# class CustomCacheDir(SCons.CacheDir.CacheDir):
#     @classmethod
#     def copy_to_cache(cls, env, src, dst) -> str:
#         fn = str(src)
#         if fn.startswith("godot-cpp"):
#             if os.path.splitext(fn)[1] not in [".lib", ".dll", ".exp", ".a", ".so"]:
#                 return super().copy_to_cache(env, src, dst)
#         return None
#
# env = SConscript("godot-cpp/SConstruct")
# env.CacheDir("scons_cache", CustomCacheDir)
#
# windows = env["platform"] == "windows"
# #linux = env["platform"] == "linux"
# config = "release" if env["target"] == "template_release" else "debug"
#
# env.Append(CPPPATH=["src/", "include/"])
# env.Replace(CXXFLAGS=str(env["CXXFLAGS"]).replace("c++17", "c++20"))
# env.Append(CXXSTANDARD="c++20")
# if windows:
#     env.Append(CXXFLAGS=["/EHsc"])
#
# # Nice to know
# # env.Append(CPPDEFINES=['IMGUI_USER_CONFIG="\\"imconfig-godot.h\\""', "IGN_EXPORT"])
#
# if config == "release":
#     if windows:
#         env.Append(CPPDEFINES=["NDEBUG"])
#
# # Copy assets to addons folder
# shutil.copy("gdex_toml.gdextension", "addons/gdex_toml/")
#
# # ...
# #sources = Glob("src/*.cpp") + Glob("imgui/*.cpp")
# sources = Glob("src/*.cpp")
#
# libpath = env.get("LIBPATH", [])
# libs = [env["LIBS"]]
#
# (extension_path,) = glob("addons/gdex_toml/*.gdextension")
# addon_path = Path(extension_path).parent
# project_name = Path(extension_path).stem
#
# if env["platform"] == "macos":
#     library = env.SharedLibrary(
#         "{0}/bin/{1}.{2}.{3}.framework/{1}.{2}.{3}".format(
#             addon_path,
#             project_name,
#             env["platform"],
#             config,
#         ),
#         source=sources,
#         LIBPATH=libpath,
#         LIBS=libs,
#     )
# else:
#     library = env.SharedLibrary(
#         "{}/bin/{}.{}.{}.{}{}".format(
#             addon_path,
#             project_name,
#             env["platform"],
#             env["arch"],
#             config,
#             env["SHLIBSUFFIX"],
#         ),
#         source=sources,
#         LIBPATH=libpath,
#         LIBS=libs,
#     )
#
# Default(library)


# -------------------

env = SConscript("godot-cpp/SConstruct")

# For the reference:
# - CCFLAGS are compilation flags shared between C and C++
# - CFLAGS are for C-specific compilation flags
# - CXXFLAGS are for C++-specific compilation flags
# - CPPFLAGS are for pre-processor flags
# - CPPDEFINES are for pre-processor defines
# - LINKFLAGS are for linking flags


def add_sources(sources, dir, extension):
    for f in os.listdir(dir):
        if f.endswith("." + extension):
            sources.append(dir + "/" + f)

windows = env["platform"] == "windows"

env.Replace(CXXFLAGS=str(env["CXXFLAGS"]).replace("c++17", "c++20"))
env.Append(CXXSTANDARD="c++20")

if windows:
    env.Append(CXXFLAGS=['/EHsc', '/std:c++20'])


sources = []
add_sources(sources, "src", "cpp")
add_sources(sources, "include", "cpp")

# tweak this if you want to use different folders, or more folders, to store your source code in.
env.Append(CPPPATH=["src/", "include/"])

if env["platform"] == "macos":
    library = env.SharedLibrary(
        "game/bin/libgdflecs.{}.{}.framework/libgdflecs.{}.{}".format(
            env["platform"], env["target"], env["platform"], env["target"]
        ),
        source=sources,
    )
else:
    library = env.SharedLibrary(
        "game/bin/libgdflecs{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=sources,
    )

Default(library)
