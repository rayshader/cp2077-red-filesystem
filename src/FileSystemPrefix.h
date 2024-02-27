#ifndef REDFS_FILESYSTEMPREFIX_H
#define REDFS_FILESYSTEMPREFIX_H

#include <RedLib.hpp>

namespace RedFS {

enum class FileSystemPrefix {
  None,
  CET,
  Redscript
};

}

RTTI_DEFINE_ENUM(RedFS::FileSystemPrefix)

#endif  //REDFS_FILESYSTEMPREFIX_H
