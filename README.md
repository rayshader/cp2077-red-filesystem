# Red File System Api
![Cyberpunk 2077](https://img.shields.io/badge/Cyberpunk%202077-v2.12a-blue)
![GitHub License](https://img.shields.io/github/license/rayshader/cp2077-red-filesystem)
[![Donate](https://img.shields.io/badge/donate-buy%20me%20a%20coffee-yellow)](https://www.buymeacoffee.com/lpfreelance)

This header-only library provides an API wrapper for [RedFileSystem] plugin.

# Getting started

## Compatibility
- [RedFileSystem] v0.9.0

## Requirements
- [RED4ext.SDK]
- [RedLib]
- [RedData] is recommended

## Installation

1. Clone this branch as a submodule in your project:
```shell
git submodule add -b api https://github.com/rayshader/cp2077-red-filesystem deps/RedFileSystem
```
2. Add dependency in CMake:
```cmake
add_subdirectory(deps/RedFileSystem)
```

It will define the following macro when it is well configured:
```cpp
#ifdef HAS_REDFILESYSTEM
// ...
#endif
```

## Usage

Include this library using:
```cpp
#include <RedFileSystem.hpp>
```

API is defined in namespace `RedFileSystem`.

You can test whether plugin is installed/detected in-game using:
```cpp
bool isDetected = RedFileSystem::IsDetected();
```

This API is a mirror of redscript declarations.

You can test whether an instance is defined using `operator bool()`. It works 
for both `FileSystemStorage` and `File`:
```cpp
RedFileSystem::FileSystemStorage storage;

if (storage) {
  // Storage can be used.
} else {
  // Storage is denied / revoked.
}
```

# Development
Contributions are welcome, feel free to fill an issue or a PR.

<!-- Table of links -->
[RED4ext.SDK]: https://github.com/WopsS/RED4ext.SDK
[RedLib]: https://github.com/psiberx/cp2077-red-lib
[RedData]: https://github.com/rayshader/cp2077-red-data
[RedFileSystem]: https://github.com/rayshader/cp2077-red-filesystem
