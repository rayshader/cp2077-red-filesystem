# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

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
[Unreleased]: https://github.com/rayshader/cp2077-red-filesystem/compare/v0.3.0...HEAD
[0.3.0]: https://github.com/rayshader/cp2077-red-filesystem/compare/v0.2.0...v0.3.0
[0.2.0]: https://github.com/rayshader/cp2077-red-filesystem/compare/v0.1.0...v0.2.0
[0.1.0]: https://github.com/rayshader/cp2077-red-filesystem/releases/tag/v0.1.0
