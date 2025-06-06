# FakPlugins

Small package providing Plugins manager for smooth modern development.
## Requirements
```sudo apt install cmake git clang-format clang-tidy```
```pip install conan```

## How to build
```git clone <project>```
```cd FakPlugins```
```
conan install . --output-folder=build --build=missing -s build_type=<Debug/Release>
```
```cd build```

```
cmake .. \
  -DCMAKE_TOOLCHAIN_FILE=build/<Debug/Release>/generators/conan_toolchain.cmake \
  -DCMAKE_BUILD_TYPE=<Debug/Release>
```
```make```

## How to install package
```
conan create . --build=missing -s build_type=<Debug/Release>
```

## Other
```make tidy```
