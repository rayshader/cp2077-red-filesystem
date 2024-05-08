#ifndef REDFILESYSTEM_FILESYSTEMSTORAGE_H
#define REDFILESYSTEM_FILESYSTEMSTORAGE_H

#include <filesystem>

#include <RED4ext/RED4ext.hpp>
#include <RedLib.hpp>

#include "File.h"
#include "FileSystemStatus.h"

namespace RedFS {

class FileSystemStorage : public Red::IScriptable {
 private:
  const std::string name;
  const std::filesystem::path storage_path;

  bool rw_permission;

  std::filesystem::path restrict_path(const std::string& p_path,
                                      std::error_code& p_error) const;

 public:
  FileSystemStorage();
  explicit FileSystemStorage(std::string p_name, std::filesystem::path p_path);

  void revoke_permission();

  [[nodiscard]] FileSystemStatus exists(const Red::CString& p_path) const;
  [[nodiscard]] FileSystemStatus is_file(const Red::CString& p_path) const;

  [[nodiscard]] Red::Handle<File> get_file(const Red::CString& p_path) const;

  [[nodiscard]] Red::DynArray<Red::Handle<File>> get_files() const;

  RTTI_IMPL_TYPEINFO(RedFS::FileSystemStorage);
  RTTI_IMPL_ALLOCATOR();
};

}  // namespace RedFS

RTTI_DEFINE_CLASS(RedFS::FileSystemStorage, {
  RTTI_ALIAS("RedFileSystem.FileSystemStorage");

  RTTI_METHOD(exists, "Exists");
  RTTI_METHOD(is_file, "IsFile");

  RTTI_METHOD(get_file, "GetFile");
  RTTI_METHOD(get_files, "GetFiles");
});

#endif  //REDFILESYSTEM_FILESYSTEMSTORAGE_H
