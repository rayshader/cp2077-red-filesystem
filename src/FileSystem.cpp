#include "FileSystem.h"

#include <RED4ext/RED4ext.hpp>
#include <RedLib.hpp>

namespace RedFS {

std::filesystem::path FileSystem::game_path;
std::filesystem::path FileSystem::cet_path;
std::filesystem::path FileSystem::redscript_path;

void FileSystem::load() {
  auto path = std::filesystem::absolute(".");

  game_path = path.parent_path().parent_path();
  cet_path = path / "plugins" / "cyber_engine_tweaks" / "mods";
  redscript_path = game_path / "r6" / "scripts";
}

inline FileSystemStatus FileSystem::exists(
  const Red::CString& p_path, const Red::Optional<FileSystemPrefix>& p_prefix) {
  std::error_code error;
  auto path = restrict_path(p_path.c_str(), p_prefix.value, error);

  if (error) {
    return FileSystemStatus::Denied;
  }
  bool status = std::filesystem::exists(path, error);

  if (error) {
    return FileSystemStatus::Failure;
  }
  return (status) ? FileSystemStatus::True : FileSystemStatus::False;
}

inline FileSystemStatus FileSystem::is_file(
  const Red::CString& p_path, const Red::Optional<FileSystemPrefix>& p_prefix) {
  std::error_code error;
  auto path = restrict_path(p_path.c_str(), p_prefix.value, error);

  if (error) {
    return FileSystemStatus::Denied;
  }
  bool status = std::filesystem::is_regular_file(path, error);

  if (error) {
    return FileSystemStatus::Failure;
  }
  return (status) ? FileSystemStatus::True : FileSystemStatus::False;
}

inline Red::Handle<File> FileSystem::get_file(
  const Red::CString& p_path, const Red::Optional<FileSystemPrefix>& p_prefix) {
  std::error_code error;
  auto path = restrict_path(p_path.c_str(), p_prefix.value, error);

  if (error) {
    return {};
  }
  return Red::MakeHandle<File>(p_path.c_str(), path);
}

std::filesystem::path FileSystem::restrict_path(const std::string& p_path,
                                                FileSystemPrefix p_prefix,
                                                std::error_code& p_error) {
  std::filesystem::path prefix_path = game_path;

  if (p_prefix == FileSystemPrefix::CET) {
    prefix_path = cet_path;
  } else if (p_prefix == FileSystemPrefix::Redscript) {
    prefix_path = redscript_path;
  }
  std::filesystem::path path = prefix_path / p_path;
  std::filesystem::path real_path =
    std::filesystem::weakly_canonical(path, p_error);

  if (p_error) {
    return real_path;
  }
  if (real_path.string().find(prefix_path.string() + "\\") != 0) {
    p_error = std::make_error_code(std::errc::permission_denied);
  }
  return real_path;
}

}  // namespace RedFS