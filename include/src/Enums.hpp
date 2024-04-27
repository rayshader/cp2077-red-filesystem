#ifndef REDFILESYSTEM_ENUMS_HPP
#define REDFILESYSTEM_ENUMS_HPP

#include <RedLib.hpp>

namespace RedFileSystem {

enum class FileSystemStatus {
  True,
  False,
  Failure,
  Denied
};

enum class FileSystemWriteMode {
  Truncate,
  Append
};

}

RTTI_MAP_TYPE_NAME(RedFileSystem::FileSystemStatus, "RedFileSystem.FileSystemStatus")
RTTI_MAP_TYPE_NAME(RedFileSystem::FileSystemWriteMode, "RedFileSystem.FileSystemWriteMode")

#endif  //REDFILESYSTEM_ENUMS_HPP
