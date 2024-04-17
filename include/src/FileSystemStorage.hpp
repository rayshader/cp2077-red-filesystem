#ifndef REDFILESYSTEM_FILESYSTEMSTORAGE_HPP
#define REDFILESYSTEM_FILESYSTEMSTORAGE_HPP

#include <RED4ext/RED4ext.hpp>
#include <RedLib.hpp>

#include "Enums.hpp"
#include "File.hpp"

namespace RedFileSystem {

class FileSystemStorage {
 private:
  Red::Handle<Red::IScriptable> wrapper;

 public:
  explicit FileSystemStorage(const Red::Handle<Red::IScriptable>& acWrapper)
      : wrapper(acWrapper) {}

  inline bool operator() const { return wrapper; }

  inline FileSystemStatus Exists(const Red::CString& acPath) const {
    FileSystemStatus status;

    Red::CallVirtual(wrapper, "Exists", status, acPath);
    return status;
  }

  inline FileSystemStatus IsFile(const Red::CString& acPath) const {
    FileSystemStatus status;

    Red::CallVirtual(wrapper, "IsFile", status, acPath);
    return status;
  }

  inline File GetFile(const Red::CString& acPath) const {
    Red::Handle<Red::IScriptable> file;

    Red::CallVirtual(wrapper, "GetFile", file, acPath);
    return File(file);
  }
};

}  // namespace RedFileSystem

#endif  //REDFILESYSTEM_FILESYSTEMSTORAGE_HPP
