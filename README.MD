# Building

REQUIRED TOOLS:
- Conan v2 (e.g. v2.1.0)
- CMake >= 3.27 (e.g. v3.27.4)


We use Conan in order to manage (some of) our external dependencies

In order to build TransceiverTool, use the following commands

ONLY IF THIS MACHINE IS FRESH AND YOU HAVE NEVER USED CONAN BEFORE (QUICK START):
```
conan profile detect --force
```

Note that you might want to modify this profile, e.g. to make sure everything is built with Clang


```
conan config install conan-profiles
```
This will install the settings from the conan-profiles subfolder (last argument - we could also have named it "ILikeTrees") to your conan default directory in order that you can use the required profiles and settings


To install the external libraries:
```
conan install . --output-folder build --build=missing -pr:a default -pr:a TransceiverTool-<build_type>
```
The last two arguments are important (and their order!!)
`-pr:a default` instructs conan to use your default profile which specifies base stuff like compiler, OS, etc and is required
`-pr:a TransceiverTool-<build_type>` sets out build type

This will also automatically generate some files like `CMakeUserPresets.json` (which in turn includes `CMakePresets.json`)

We include `-DCMAKE_EXPORT_COMPILE_COMMANDS=1` by default. This will generate the `compile_commands.json` file needed by the Clangd language server to provide autocompletion in VSCode

Release Build:
```
conan install . --output-folder build --build=missing -pr:a default -pr:a TransceiverTool-release
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 --preset conan-release
cmake --build --preset conan-release
```

RelWithDebInfo Build:
```
conan install . --output-folder build --build=missing -pr:a default -pr:a TransceiverTool-relwithdebinfo
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 --preset conan-relwithdebinfo
cmake --build --preset conan-relwithdebinfo
```

Debug Build:
```
conan install . --output-folder build --build=missing -pr:a default -pr:a TransceiverTool-debug
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 --preset conan-debug
cmake --build --preset conan-debug
```
