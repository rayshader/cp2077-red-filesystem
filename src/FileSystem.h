#ifndef REDFS_FILESYSTEM_H
#define REDFS_FILESYSTEM_H

#include <filesystem>
#include <regex>
#include <unordered_map>

#include <RED4ext/RED4ext.hpp>
#include <RedLib.hpp>

#include "File.h"
#include "FileSystemStatus.h"
#include "FileSystemStorage.h"

namespace RedFS {

class FileSystem : public Red::IScriptable {
  using StorageMap =
    std::unordered_map<std::string, Red::Handle<FileSystemStorage>>;

  static RED4ext::PluginHandle handle;
  static RED4ext::Logger* logger;

  static std::filesystem::path game_path;
  static std::filesystem::path storages_path;
  static bool has_mo2;
  static std::vector<std::string> blacklist;

  static std::regex storage_name_rule;

  static StorageMap storages;
  static bool has_error;

  static std::filesystem::path get_game_path();

  static bool request_directory(const std::filesystem::path& p_path);
  static bool migrate_directory(const std::filesystem::path& p_old_path,
                                const std::filesystem::path& p_new_path);
  static void detect_mo2();

  static Red::Handle<FileSystemStorage> find_storage(const std::string& p_name);

 public:
  static void load(RED4ext::PluginHandle p_handle, RED4ext::Logger* p_logger);
  static void unload();

  static Red::Handle<FileSystemStorage> get_storage(const Red::CString& p_name);
  static Red::Handle<FileSystemStorage> get_shared_storage();

  static bool is_mo2_detected();
  static bool is_blacklisted(const std::filesystem::path& p_path);

  template<class... Args>
  static void debug(const std::format_string<Args...>& p_fmt, Args&&... p_args) {
    const auto message = std::format(p_fmt, std::forward<Args>(p_args)...);
    logger->Debug(handle, message.c_str());
  }

  static std::string get_error_message(int p_errno);

  RTTI_IMPL_TYPEINFO(RedFS::FileSystem);
  RTTI_IMPL_ALLOCATOR();
};

}  // namespace RedFS

RTTI_DEFINE_CLASS(RedFS::FileSystem, {
  RTTI_ALIAS("RedFileSystem.FileSystem");

  RTTI_METHOD(get_storage, "GetStorage");
  RTTI_METHOD(get_shared_storage, "GetSharedStorage");
});

#endif  //REDFS_FILESYSTEM_H
