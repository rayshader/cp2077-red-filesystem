# Red File System Api
![Cyberpunk 2077](https://img.shields.io/badge/Cyberpunk%202077-v2.12-blue)
![GitHub License](https://img.shields.io/github/license/rayshader/cp2077-red-filesystem)
[![Donate](https://img.shields.io/badge/donate-buy%20me%20a%20coffee-yellow)](https://www.buymeacoffee.com/lpfreelance)

This header-only library provides an API wrapper for [RedFileSystem] plugin.

# Getting started

## Compatibility
- [RedFileSystem] v0.5.0

## Requirements
- [RED4ext.SDK]
- [RedLib]

## Usage

API is defined in namespace `RedFileSystem`.

You can test whether plugin is installed/detected in-game using:
```cpp
bool isDetected = RedFileSystem::IsDetected();
```

This API is a mirror of redscript declarations. You can also directly test 
whether an instance is defined using `bool operator()` such as:
```cpp
RedFileSystem::FileSystemStorage storage;

if (!storage) {
    // Access to storage is denied or operation failed.
    return;
}
// Use it...
```

# Development
Contributions are welcome, feel free to fill an issue or a PR.

<!-- Table of links -->
[RED4ext.SDK]: https://github.com/WopsS/RED4ext.SDK
[RedLib]: https://github.com/psiberx/cp2077-red-lib
[RedFileSystem]: https://github.com/rayshader/cp2077-red-filesystem
