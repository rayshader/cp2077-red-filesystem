#ifndef REDFILESYSTEM_REDFILESYSTEM_HPP
#define REDFILESYSTEM_REDFILESYSTEM_HPP

#include <RedLib.hpp>

#include "src/File.hpp"
#include "src/FileSystem.hpp"
#include "src/FileSystemStorage.hpp"

namespace RedFileSystem {

inline bool IsDetected() {
  return Red::GetClass("RedFileSystem.FileSystem") != nullptr;
}

}  // namespace RedFileSystem

#endif  //REDFILESYSTEM_REDFILESYSTEM_HPP
