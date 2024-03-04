#include "FileSystemStorage.h"

#include <utility>

namespace RedFS {

FileSystemStorage::FileSystemStorage() : rw_permission(false) {}

FileSystemStorage::FileSystemStorage(std::string p_name,
                                     std::filesystem::path p_path)
    : name(std::move(p_name)),
      storage_path(std::move(p_path)),
      rw_permission(true) {}

void FileSystemStorage::revoke_permission() {
  rw_permission = false;
}

FileSystemStatus FileSystemStorage::exists(const Red::CString& p_path) const {
  if (!rw_permission) {
    return FileSystemStatus::Denied;
  }
  std::error_code error;
  auto path = restrict_path(p_path.c_str(), error);

  if (error) {
    return FileSystemStatus::Denied;
  }
  bool status = std::filesystem::exists(path, error);

  if (error) {
    return FileSystemStatus::Failure;
  }
  return (status) ? FileSystemStatus::True : FileSystemStatus::False;
}

FileSystemStatus FileSystemStorage::is_file(const Red::CString& p_path) const {
  if (!rw_permission) {
    return FileSystemStatus::Denied;
  }
  std::error_code error;
  auto path = restrict_path(p_path.c_str(), error);

  if (error) {
    return FileSystemStatus::Denied;
  }
  bool status = std::filesystem::is_regular_file(path, error);

  if (error) {
    return FileSystemStatus::Failure;
  }
  return (status) ? FileSystemStatus::True : FileSystemStatus::False;
}

Red::Handle<File> FileSystemStorage::get_file(
  const Red::CString& p_path) const {
  if (!rw_permission) {
    return {};
  }
  std::error_code error;
  auto path = restrict_path(p_path.c_str(), error);

  if (error) {
    return {};
  }
  return Red::MakeHandle<File>(p_path.c_str(), path);
}

std::filesystem::path FileSystemStorage::restrict_path(
  const std::string& p_path, std::error_code& p_error) const {
  std::filesystem::path path = storage_path / p_path;
  std::filesystem::path real_path =
    std::filesystem::weakly_canonical(path, p_error);

  if (p_error) {
    return real_path;
  }
  if (real_path.string().find(storage_path.string() + "\\") != 0) {
    p_error = std::make_error_code(std::errc::permission_denied);
  }
  return real_path;
}

}  // namespace RedFS