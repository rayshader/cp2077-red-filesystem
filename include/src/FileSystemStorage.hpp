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
  FileSystemStorage() = default;

  explicit FileSystemStorage(const Red::Handle<Red::IScriptable>& wrapper)
      : wrapper(wrapper) {}

  explicit operator bool() const noexcept { return wrapper; }

  inline FileSystemStatus Exists(const Red::CString& path) const {
    FileSystemStatus status;

    Red::CallVirtual(wrapper, "Exists", status, path);
    return status;
  }

  inline FileSystemStatus IsFile(const Red::CString& path) const {
    FileSystemStatus status;

    Red::CallVirtual(wrapper, "IsFile", status, path);
    return status;
  }

  inline File GetFile(const Red::CString& path) const {
    Red::Handle<Red::IScriptable> file;

    Red::CallVirtual(wrapper, "GetFile", file, path);
    return File(file);
  }
};

}  // namespace RedFileSystem

#endif  //REDFILESYSTEM_FILESYSTEMSTORAGE_HPP
