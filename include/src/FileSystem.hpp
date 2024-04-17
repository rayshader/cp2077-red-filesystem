#ifndef REDFILESYSTEM_FILESYSTEM_HPP
#define REDFILESYSTEM_FILESYSTEM_HPP

#include <RED4ext/RED4ext.hpp>
#include <RedLib.hpp>

#include "FileSystemStorage.hpp"

namespace RedFileSystem {

class FileSystem {
 public:
  inline static FileSystemStorage GetStorage(const Red::CString& acName) {
    Red::Handle<Red::IScriptable> storage;

    Red::CallStatic("RedFileSystem.FileSystem", "GetStorage", storage, acName);
    return FileSystemStorage(storage);
  }
};

}  // namespace RedFileSystem

#endif  //REDFILESYSTEM_FILESYSTEM_HPP
