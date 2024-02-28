#ifndef REDFS_FILESYSTEM_H
#define REDFS_FILESYSTEM_H

#include <filesystem>

#include <RED4ext/RED4ext.hpp>
#include <RedLib.hpp>

#include "File.h"
#include "FileSystemPrefix.h"
#include "FileSystemStatus.h"

namespace RedFS {

class FileSystem : public Red::IScriptable {
 private:
  static std::filesystem::path game_path;
  static std::filesystem::path cet_path;
  static std::filesystem::path redscript_path;

  static std::filesystem::path restrict_path(const std::string& p_path,
                                             FileSystemPrefix p_prefix,
                                             std::error_code& p_error);

 public:
  static void load();

  static FileSystemStatus exists(
    const Red::CString& p_path,
    const Red::Optional<FileSystemPrefix>& p_prefix);
  static FileSystemStatus is_file(
    const Red::CString& p_path,
    const Red::Optional<FileSystemPrefix>& p_prefix);

  static Red::Handle<File> get_file(
    const Red::CString& p_path,
    const Red::Optional<FileSystemPrefix>& p_prefix);

  RTTI_IMPL_TYPEINFO(RedFS::FileSystem);
  RTTI_IMPL_ALLOCATOR();
};

}  // namespace RedFS

RTTI_DEFINE_CLASS(RedFS::FileSystem, {
  RTTI_ALIAS("RedFileSystem.FileSystem");

  RTTI_METHOD(exists, "Exists");
  RTTI_METHOD(is_file, "IsFile");

  RTTI_METHOD(get_file, "GetFile");
});

#endif  //REDFS_FILESYSTEM_H
