#!/usr/bin/env python
import os
import sys
import SCons
import pathlib

#Example Invokations:
#    scons use_msvc=yes godot_cpp=../../../godot-cpp/
#    scons use_mingw=yes godot_cpp=../../../godot-cpp/

print(os.getcwd())
CWD = os.getcwd()

env = Environment()
GODOT_CPP = ARGUMENTS.get('godot_cpp', "")
USE_MINGW = ARGUMENTS.get('use_mingw', "")
USE_MSVC = ARGUMENTS.get('use_msvc', "")
USE_CLANG_CL = ARGUMENTS.get('use_clang_cl', "")

if len(GODOT_CPP) == 0:
    print("Provide the path to your godot-cpp. Add 'godot_cpp=Path/To/godot-cpp/' to your SCons command.")
    exit(1)
BIN_DIR = "project/bin/"
OUTPUT_LIB_NAME = "libBsExampleModule"

env = SConscript(GODOT_CPP + "SConstruct")

if len(USE_MSVC) != 0:
    print("Compiling for msvc..")
    env.Append(CXXFLAGS=["/std:c++20","/EHsc","/permissive","/Zc:preprocessor","/Zc:lambda"])
elif len(USE_MINGW) != 0:
    print("Compiling for mingw..")
    env.Append(CXXFLAGS=["-std=c++20","-fexceptions"])
elif len(USE_CLANG_CL) != 0:
    print("Compiling for clang..")
    env.Append(CXXFLAGS=["-std=c++20","-fexceptions"])
else:
    print("Must specify a compiler by adding one of the following to your SCons command:")
    print("    MSVC Users - 'use_msvc=yes'")
    print("    GCC Users - 'use_mingw=yes'")
    print("    CLANG Users - 'use_clang_cl=yes'")

env.Append(CPPPATH=[".",".."])

library = env.SharedLibrary(
        BIN_DIR + OUTPUT_LIB_NAME + "{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=Glob("*.cpp"),
)

Default(library)