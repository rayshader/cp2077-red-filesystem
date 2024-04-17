#ifndef REDFILESYSTEM_ENUMS_HPP
#define REDFILESYSTEM_ENUMS_HPP

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

#endif  //REDFILESYSTEM_ENUMS_HPP
