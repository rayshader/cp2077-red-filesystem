# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

------------------------

## [0.14.0] - 2025-07-18
### Changed
- support patch 2.3

------------------------

## [0.14.0] - 2025-07-18
### Changed
- support patch 2.3

------------------------

## [0.13.0] - 2025-03-31
### Added
- read binary file encoded to Base64 using `ReadAsBase64()`, in both `File` and
  `AsyncFile`. This is mainly intended to read PNG files (with a screenshot
  feature).

------------------------

## [0.12.0] - 2025-01-23
### Changed
- support patch 2.21

------------------------

## [0.11.0] - 2024-12-11
### Changed
- support patch 2.2

------------------------

## [0.10.0] - 2024-09-13
### Changed
- support patch 2.13

------------------------

## [0.9.4] - 2024-07-20
### Fixed
- use Unicode charset for path when resolving game's directory.

------------------------

## [0.9.3] - 2024-07-14
### Fixed
- `RedData.Json` is now optional when installing `RedFileSystem`.

------------------------

## [0.9.2] - 2024-06-28
### Fixed
- support any path of execution when resolving game's directory (Vortex / MO2).

------------------------

## [0.9.1] - 2024-06-04
### Added
- blacklist with file `__folder_managed_by_vortex` to hide it and prevent 
  access from `FileSystemStorage`.

------------------------

## [0.9.0] - 2024-06-02
### Added
- method to delete a file from storage with `DeleteFile`.

### Fixed
- support [MO2](https://github.com/ModOrganizer2/modorganizer/).

------------------------

## [0.8.0] - 2024-05-17
### Added
- asynchronous read/write operations with `AsyncFile`.
- callback handling with `FilePromise` when operation passes/fails.
- common storage in `<game>/r6/storages/shared/` to share files between mods.
  It can be accessed with `FileSystem.GetSharedStorage()` without restrictions.

### Changed
- migrate storages from `<game>/red4ext/plugins/RedFileSystem/storages/` to
  `<game>/r6/storages/` at startup.
- read/write operations with `File` are now thread safe.
- the name `shared` when using `FileSystem.GetStorage()` is now reserved and
  forbidden.

### Fixed
- storage is revoked when using case-insensitive names.

------------------------

## [0.7.1] - 2024-05-12
### Fixed
- issue when using files listed with method `GetFiles`.

------------------------

## [0.7.0] - 2024-05-08
### Added
- method `GetFiles` to list files present in a storage.

------------------------

## [0.6.0] - 2024-04-26
### Added
- optional indent argument when writing Json to format output.

### Changed
- now using `WriteJson` format output as minified Json by default.

------------------------

## [0.5.0] - 2024-04-16
### Added
- Json read/write functions are now undefined when 'RedData.Json' is not present.
- declaration of module 'RedFileSystem'.

------------------------

## [0.4.0] - 2024-04-09
### Added
- Json dependency with plugin [RedData](https://github.com/rayshader/cp2077-red-data).

### Removed
- Json classes and implementations, now powered by `RedData`.

------------------------

## [0.3.0] - 2024-03-04
### Changed
- use a storage per mod to read/write files.
- move features from `FileSystem` to `FileSystemStorage`.

### Security
- restrict all read/write operations within `red4ext\plugins\RedFileSystem\storages\`.
- revoke all operations of a storage when getting it multiple times.

This version is not compatible with v0.2.0 and below, see the README and 
`examples/` to migrate your code.

------------------------

## [0.2.0] - 2024-03-01
### Changed
- support patch v2.12

------------------------

## [0.1.0] - 2024-03-01
### Added
- test whether a file/directory exists or not.
- test whether a path points to a regular file.
- read text / lines / json from files.
- write text / lines / json to files (truncate or append).
- read / write / stringify Json data.

<!-- Table of releases -->
[Unreleased]: https://github.com/rayshader/cp2077-red-filesystem/compare/v0.14.0...HEAD
[0.14.0]: https://github.com/rayshader/cp2077-red-filesystem/compare/v0.13.0...v0.14.0
[0.13.0]: https://github.com/rayshader/cp2077-red-filesystem/compare/v0.12.0...v0.13.0
[0.12.0]: https://github.com/rayshader/cp2077-red-filesystem/compare/v0.11.0...v0.12.0
[0.11.0]: https://github.com/rayshader/cp2077-red-filesystem/compare/v0.10.0...v0.11.0
[0.10.0]: https://github.com/rayshader/cp2077-red-filesystem/compare/v0.9.4...v0.10.0
[0.9.4]: https://github.com/rayshader/cp2077-red-filesystem/compare/v0.9.3...v0.9.4
[0.9.3]: https://github.com/rayshader/cp2077-red-filesystem/compare/v0.9.2...v0.9.3
[0.9.2]: https://github.com/rayshader/cp2077-red-filesystem/compare/v0.9.1...v0.9.2
[0.9.1]: https://github.com/rayshader/cp2077-red-filesystem/compare/v0.9.0...v0.9.1
[0.9.0]: https://github.com/rayshader/cp2077-red-filesystem/compare/v0.8.0...v0.9.0
[0.8.0]: https://github.com/rayshader/cp2077-red-filesystem/compare/v0.7.1...v0.8.0
[0.7.1]: https://github.com/rayshader/cp2077-red-filesystem/compare/v0.7.0...v0.7.1
[0.7.0]: https://github.com/rayshader/cp2077-red-filesystem/compare/v0.6.0...v0.7.0
[0.6.0]: https://github.com/rayshader/cp2077-red-filesystem/compare/v0.5.0...v0.6.0
[0.5.0]: https://github.com/rayshader/cp2077-red-filesystem/compare/v0.4.0...v0.5.0
[0.4.0]: https://github.com/rayshader/cp2077-red-filesystem/compare/v0.3.0...v0.4.0
[0.3.0]: https://github.com/rayshader/cp2077-red-filesystem/compare/v0.2.0...v0.3.0
[0.2.0]: https://github.com/rayshader/cp2077-red-filesystem/compare/v0.1.0...v0.2.0
[0.1.0]: https://github.com/rayshader/cp2077-red-filesystem/releases/tag/v0.1.0
