#!/usr/bin/env python
import os
import sys

env = SConscript("../godot-cpp/SConstruct")
env.Append(CPPPATH=["src/"])
_sources = Glob("src/*.cpp")

sources = [
        Glob("src/Utils/*.cpp"),
        Glob("src/Interfaces/*.cpp"),

        Glob("src/Mods/*.cpp"),
        
        Glob("src/World/*.cpp"),
        Glob("src/Inventory/*.cpp"),

        Glob("src/Items/*.cpp"),
        Glob("src/Machines/*.cpp"),
        Glob("src/Recipes/*.cpp"),
        Glob("src/Storages/*.cpp"),
        Glob("src/ConveyorBelts/*.cpp"),

        Glob("src/Buildings/*.cpp"),

        Glob("src/Analytics/*.cpp"),
    ]

for source in sources:
    _sources += source

if env["platform"] == "macos":
    library = env.SharedLibrary(
        "demo/bin/AG.{}.{}.framework/AG.{}.{}".format(
            env["platform"], env["target"], env["platform"], env["target"]
        ),
        source=_sources,
    )
else:
    library = env.SharedLibrary(
        "../AG/bin/AG{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=_sources,
    )

Default(library)
