#include "FileSystem.h"
#include "FileSystemStorage.h"

namespace RedFS {

RED4ext::PluginHandle FileSystem::handle = nullptr;
RED4ext::Logger* FileSystem::logger = nullptr;

std::filesystem::path FileSystem::game_path;
std::filesystem::path FileSystem::storages_path;

std::regex FileSystem::storage_name_rule("[A-Za-z]{3,24}");

FileSystem::StorageMap FileSystem::storages{};
bool FileSystem::has_error = true;

void FileSystem::load(RED4ext::PluginHandle p_handle,
                      RED4ext::Logger* p_logger) {
  handle = p_handle;
  logger = p_logger;
  auto path = std::filesystem::absolute(".");

  game_path = path.parent_path().parent_path();
  storages_path =
    game_path / "red4ext" / "plugins" / "RedFileSystem" / "storages";
  bool is_present = request_directory(storages_path);

  if (!is_present) {
    has_error = true;
    logger->ErrorF(handle, "Failed to create directory at \"%s\".",
                   storages_path.c_str());
    logger->Error(handle, "RedFileSystem has been disabled.");
    return;
  }
  has_error = false;
  logger->Info(handle, "RedFileSystem has been enabled.");
}

void FileSystem::unload() {
  storages.clear();
  has_error = true;
  logger->Info(handle, "RedFileSystem has been terminated.");
  handle = nullptr;
  logger = nullptr;
}

inline Red::Handle<FileSystemStorage> FileSystem::get_storage(
  const Red::CString& p_name) {
  if (has_error) {
    logger->Error(handle, "RedFileSystem is disabled for all mods.");
    return {};
  }
  std::string name = p_name.c_str();

  if (!regex_match(name, storage_name_rule)) {
    logger->ErrorF(handle, "Name of storage \"%s\" is not allowed.", name.c_str());
    logger->Error(handle, "See the documentation to fix this issue.");
    return {};
  }
  if (storages.contains(name)) {
    storages.at(name)->revoke_permission();
    logger->ErrorF(handle, "Attempt to access storage \"%s\" several times.",
                   name.c_str());
    logger->Error(handle,
                  "Only one mod can access its own storage with "
                  "RedFileSystem.");
    logger->Error(handle,
                  "Access to this storage has been permanently revoked "
                  "for this session.");
    return {};
  }
  auto path = storages_path / name;
  bool is_present = request_directory(path);

  if (!is_present) {
    logger->ErrorF(handle, "Failed to create storage \"%s\".",
                   name.c_str());
    return {};
  }
  auto storage = Red::MakeHandle<FileSystemStorage>(name, path);

  storages[name] = storage;
  logger->InfoF(handle, "Access to storage \"%s\" has been granted.",
                name.c_str());
  return storage;
}

bool FileSystem::request_directory(const std::filesystem::path& p_path) {
  std::error_code error;
  bool is_present = std::filesystem::exists(p_path, error);

  if (error) {
    return false;
  }
  if (is_present) {
    return true;
  }
  is_present = std::filesystem::create_directory(p_path, error);
  if (!is_present || error) {
    return false;
  }
  return true;
}

}  // namespace RedFS