#ifndef REDFS_FILESYSTEMSTATUS_H
#define REDFS_FILESYSTEMSTATUS_H

#include <RedLib.hpp>

namespace RedFS {

enum class FileSystemStatus {
  True,
  False,
  Failure,
  Denied
};

}

RTTI_DEFINE_ENUM(RedFS::FileSystemStatus)

#endif  //REDFS_FILESYSTEMSTATUS_H
