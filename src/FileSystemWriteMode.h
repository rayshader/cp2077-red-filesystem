#ifndef REDFS_FILESYSTEMWRITEMODE_H
#define REDFS_FILESYSTEMWRITEMODE_H

#include <RedLib.hpp>

namespace RedFS {

enum class FileSystemWriteMode {
  Truncate,
  Append
};

}

RTTI_DEFINE_ENUM(RedFS::FileSystemWriteMode)

#endif  //REDFS_FILESYSTEMWRITEMODE_H
