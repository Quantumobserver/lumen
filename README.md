# lumen

## Build

Basic (using ninja)
```cmake
cmake -G"Ninja" -S. -B./bin -DCMAKE_BUILD_TYPE=Release
cmake --build ./bin
```
Build Options (Use -D to define the options you need):
1. CMAKE_BUILD_TYPE: this is a CMake built-in option, it can be Debug, Release, RelWithDebInfo and MinSizeRel.
2. CMAKE_TOOLCHAIN_FILE: this is a CMake built-in option, the default setting will be use if not specify.
3. LUMEN_BUILD_TESTS: this option is for lumen project, the tests will be build if defined.
