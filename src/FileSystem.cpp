#include "FileSystem.h"
#include "FileSystemStorage.h"

#include "Utils.h"

namespace RedFS {

RED4ext::PluginHandle FileSystem::handle = nullptr;
RED4ext::Logger* FileSystem::logger = nullptr;

std::filesystem::path FileSystem::game_path;
std::filesystem::path FileSystem::storages_path;
bool FileSystem::has_mo2 = false;
std::vector<std::string> FileSystem::blacklist = {"__folder_managed_by_vortex"};

std::regex FileSystem::storage_name_rule("[A-Za-z]{3,24}");

FileSystem::StorageMap FileSystem::storages{};
bool FileSystem::has_error = true;

// Credits to https://github.com/jackhumbert/cyberpunk2077-input-loader/blob/a6eed8eda9ae97e92ff97e60158c08b945e61036/src/Utils.cpp#L26
std::filesystem::path get_game_path() {
  constexpr auto path_length = MAX_PATH + 1;
  std::string filename;

  do {
    filename.resize(filename.size() + path_length, '\0');
    auto length = GetModuleFileName(nullptr, filename.data(),
                                    static_cast<uint32_t>(filename.size()));

    if (length > 0) {
      filename.resize(length);
    }
  } while (GetLastError() == ERROR_INSUFFICIENT_BUFFER);
  return std::filesystem::path(filename)
    .parent_path()   // Resolve to "x64" directory.
    .parent_path()   // Resolve to "bin" directory.
    .parent_path();  // Resolve to game root directory.
}

void FileSystem::load(RED4ext::PluginHandle p_handle,
                      RED4ext::Logger* p_logger) {
  handle = p_handle;
  logger = p_logger;
  detect_mo2();
  game_path = get_game_path();
  storages_path = game_path / "r6" / "storages";
  bool is_present = request_directory(storages_path);

  if (!is_present) {
    has_error = true;
    logger->ErrorF(handle, "Failed to create directory at \"%s\".",
                   storages_path.string().c_str());
    logger->Error(handle, "RedFileSystem has been disabled.");
    return;
  }
  auto old_path =
    game_path / "red4ext" / "plugins" / "RedFileSystem" / "storages";

  if (!migrate_directory(old_path, storages_path)) {
    has_error = true;
    logger->WarnF(handle, R"(Failed to migrate directory from "%s" to "%s".)",
                  old_path.string().c_str(), storages_path.string().c_str());
    logger->Warn(handle, "You need to manually move content yourself.");
    return;
  }
  has_error = false;
  logger->Info(handle, "RedFileSystem has been enabled.");
}

void FileSystem::unload() {
  std::error_code error;
  auto old_path =
    game_path / "red4ext" / "plugins" / "RedFileSystem" / "storages";

  std::filesystem::remove_all(old_path, error);
  storages.clear();
  has_error = true;
  logger->Info(handle, "RedFileSystem has been terminated.");
  handle = nullptr;
  logger = nullptr;
}

Red::Handle<FileSystemStorage> FileSystem::get_storage(
  const Red::CString& p_name) {
  if (has_error) {
    logger->Error(handle, "RedFileSystem is disabled for all mods.");
    return {};
  }
  std::string name = p_name.c_str();

  if (!regex_match(name, storage_name_rule) ||
      equals_insensitive(name, "shared")) {
    logger->ErrorF(handle, "Name of storage \"%s\" is not allowed.",
                   name.c_str());
    logger->Error(handle, "See the documentation to fix this issue.");
    return {};
  }
  auto storage = find_storage(name);

  if (storage) {
    storage->revoke_permission();
    logger->ErrorF(handle,
                   "Attempt to access storage \"%s\" several times. "
                   "Only one mod can access its own storage with "
                   "RedFileSystem. Access to this storage has been permanently "
                   "revoked for this session.",
                   name.c_str());
    return {};
  }
  auto path = storages_path / name;
  bool is_present = request_directory(path);

  if (!is_present) {
    logger->ErrorF(handle, "Failed to create storage \"%s\".", name.c_str());
    return {};
  }
  storage = Red::MakeHandle<FileSystemStorage>(name, path);
  storages[name] = storage;
  logger->InfoF(handle, "Access to storage \"%s\" has been granted.",
                name.c_str());
  return storage;
}

Red::Handle<FileSystemStorage> FileSystem::get_shared_storage() {
  if (has_error) {
    logger->Error(handle, "RedFileSystem is disabled.");
    return {};
  }
  constexpr auto name = "shared";
  auto storage = find_storage(name);

  if (storage) {
    logger->Info(handle, "Access to shared storage has been granted.");
    return storage;
  }
  auto path = storages_path / name;
  bool is_present = request_directory(path);

  if (!is_present) {
    logger->Error(handle, "Failed to create shared storage.");
    return {};
  }
  storage = Red::MakeHandle<FileSystemStorage>(name, path);
  storages[name] = storage;
  logger->Info(handle, "Access to shared storage has been granted.");
  return storage;
}

bool FileSystem::is_mo2_detected() {
  return has_mo2;
}

bool FileSystem::is_blacklisted(const std::filesystem::path& p_path) {
  const std::string filename = p_path.filename().string();

  for (const auto& bl_filename : blacklist) {
    if (equals_insensitive(filename, bl_filename)) {
      return true;
    }
  }
  return false;
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
  if (error) {
    return false;
  }
  return is_present;
}

bool FileSystem::migrate_directory(const std::filesystem::path& p_old_path,
                                   const std::filesystem::path& p_new_path) {
  std::error_code error;
  bool has_old_path = std::filesystem::exists(p_old_path, error);

  if (error) {
    return false;
  }
  if (!has_old_path) {
    return true;
  }
  std::filesystem::copy_options options =
    std::filesystem::copy_options::overwrite_existing |
    std::filesystem::copy_options::recursive;

  std::filesystem::copy(p_old_path, p_new_path, options, error);
  if (error) {
    logger->ErrorF(handle, "Could not migrate \"storages\" due to: %s.",
                   error.message().c_str());
    return false;
  }
  // NOTE: remove old path in unload() to be backward compatible with
  //       RED4ext v1.24.3
  return true;
}

void FileSystem::detect_mo2() {
  has_mo2 = GetModuleHandle(TEXT("usvfs_x64.dll")) != nullptr;
  if (!has_mo2) {
    return;
  }
  logger->Info(handle, "RedFileSystem launched using MO2.");
}

Red::Handle<FileSystemStorage> FileSystem::find_storage(
  const std::string& p_name) {
  for (const auto& storage : storages) {
    if (equals_insensitive(storage.first, p_name)) {
      return storage.second;
    }
  }
  return {};
}

}  // namespace RedFS