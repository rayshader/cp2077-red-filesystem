#include "FileSystemStorage.h"
#include "FileSystem.h"

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
  if (FileSystem::is_blacklisted(path)) {
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
  if (FileSystem::is_blacklisted(path)) {
    return FileSystemStatus::Denied;
  }
  bool status = std::filesystem::is_regular_file(path, error);

  if (error) {
    return FileSystemStatus::Failure;
  }
  return (status) ? FileSystemStatus::True : FileSystemStatus::False;
}

Red::Handle<File> FileSystemStorage::get_file(const Red::CString& p_path) {
  if (!rw_permission) {
    return {};
  }
  std::error_code error;
  auto path = restrict_path(p_path.c_str(), error);

  if (error) {
    return {};
  }
  if (FileSystem::is_blacklisted(path)) {
    return {};
  }
  SharedMutex mutex = get_mutex(path);

  return Red::MakeHandle<File>(mutex, p_path.c_str(), path);
}

Red::DynArray<Red::Handle<File>> FileSystemStorage::get_files() {
  return _get_files<File>();
}

FileSystemStatus FileSystemStorage::delete_file(
  const Red::CString& p_path) const {
  if (!rw_permission) {
    return FileSystemStatus::Denied;
  }
  std::error_code error;
  auto path = restrict_path(p_path.c_str(), error);

  if (error) {
    return FileSystemStatus::Denied;
  }
  if (FileSystem::is_blacklisted(path)) {
    return FileSystemStatus::Denied;
  }
  if (!std::filesystem::is_regular_file(path, error)) {
    return FileSystemStatus::Failure;
  }
  bool is_removed = std::filesystem::remove(path, error);

  if (error) {
    return FileSystemStatus::Failure;
  }
  return (is_removed) ? FileSystemStatus::True : FileSystemStatus::False;
}

Red::Handle<AsyncFile> FileSystemStorage::get_async_file(
  const Red::CString& p_path) {
  if (!rw_permission) {
    return {};
  }
  std::error_code error;
  auto path = restrict_path(p_path.c_str(), error);

  if (error) {
    return {};
  }
  if (FileSystem::is_blacklisted(path)) {
    return {};
  }
  SharedMutex mutex = get_mutex(path);

  return Red::MakeHandle<AsyncFile>(mutex, p_path.c_str(), path);
}

Red::DynArray<Red::Handle<AsyncFile>> FileSystemStorage::get_async_files() {
  return _get_files<AsyncFile>();
}

std::filesystem::path FileSystemStorage::restrict_path(
  const std::string& p_path, std::error_code& p_error) const {
  std::filesystem::path path = storage_path / p_path;
  std::filesystem::path real_path;

  // NOTE: See issue regarding usage of `std::weakly_canonical` with MO2:
  //       https://github.com/ModOrganizer2/modorganizer/issues/2039
  if (FileSystem::is_mo2_detected()) {
    if (p_path.find('/') != std::string::npos ||
        p_path.find('\\') != std::string::npos) {
      p_error = std::make_error_code(std::errc::permission_denied);
    }
    return path;
  } else {
    real_path = std::filesystem::weakly_canonical(path, p_error);
  }
  if (p_error) {
    return real_path;
  }
  if (real_path.string().find(storage_path.string() + "\\") != 0) {
    p_error = std::make_error_code(std::errc::permission_denied);
  }
  return real_path;
}

SharedMutex FileSystemStorage::get_mutex(const std::filesystem::path& p_path) {
  if (mutexes.contains(p_path)) {
    return mutexes[p_path];
  }
  SharedMutex mutex = std::make_shared<std::mutex>();

  mutexes[p_path] = mutex;
  return mutex;
}

template <class T>
Red::DynArray<Red::Handle<T>> FileSystemStorage::_get_files() {
  if (!rw_permission) {
    return {};
  }
  std::error_code error;
  auto entries = std::filesystem::directory_iterator(storage_path, error);

  if (error) {
    return {};
  }
  Red::DynArray<Red::Handle<T>> files;

  std::for_each(
    begin(entries), end(entries),
    [&files, this](const std::filesystem::directory_entry& entry) -> void {
      if (!entry.is_regular_file()) {
        return;
      }
      auto file_path = entry.path();

      if (FileSystem::is_blacklisted(file_path)) {
        return;
      }
      auto file_name = file_path.filename();
      auto file_mutex = get_mutex(file_path);
      auto file = Red::MakeHandle<T>(file_mutex, file_name, file_path);

      files.PushBack(file);
    });
  return files;
}

}  // namespace RedFS