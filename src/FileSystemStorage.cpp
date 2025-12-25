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
    FileSystem::debug("Exists(\"{}\") failed because FileSystemStorage is used more than once.", p_path.c_str());
    return FileSystemStatus::Denied;
  }

  std::error_code error;
  const auto path = restrict_path(p_path.c_str(), error);
  if (error) {
    FileSystem::debug("Exists(\"{}\") failed because path is unsafe: {}", p_path.c_str(), error.message().c_str());
    return FileSystemStatus::Denied;
  }

  if (FileSystem::is_blacklisted(path)) {
    FileSystem::debug("Exists(\"{}\") failed because path is blacklisted.", p_path.c_str());
    return FileSystemStatus::Denied;
  }

  const bool status = std::filesystem::exists(path, error);
  if (error) {
    FileSystem::debug("Exists(\"{}\") failed at OS level: {}", p_path.c_str(),error.message().c_str());
    return FileSystemStatus::Failure;
  }

  return status ? FileSystemStatus::True : FileSystemStatus::False;
}

FileSystemStatus FileSystemStorage::is_file(const Red::CString& p_path) const {
  if (!rw_permission) {
    FileSystem::debug("IsFile(\"{}\") failed because FileSystemStorage is used more than once.", p_path.c_str());
    return FileSystemStatus::Denied;
  }

  std::error_code error;
  const auto path = restrict_path(p_path.c_str(), error);
  if (error) {
    FileSystem::debug("IsFile(\"{}\") failed because path is unsafe: {}", p_path.c_str(), error.message().c_str());
    return FileSystemStatus::Denied;
  }

  if (FileSystem::is_blacklisted(path)) {
    FileSystem::debug("IsFile(\"{}\") failed because path is blacklisted.", p_path.c_str());
    return FileSystemStatus::Denied;
  }

  const bool status = std::filesystem::is_regular_file(path, error);
  if (error) {
    FileSystem::debug("IsFile(\"{}\") failed at OS level: {}", p_path.c_str(), error.message().c_str());
    return FileSystemStatus::Failure;
  }

  return status ? FileSystemStatus::True : FileSystemStatus::False;
}

Red::Handle<File> FileSystemStorage::get_file(const Red::CString& p_path) {
  if (!rw_permission) {
    FileSystem::debug("GetFile(\"{}\") failed because FileSystemStorage is used more than once.", p_path.c_str());
    return {};
  }

  std::error_code error;
  const auto path = restrict_path(p_path.c_str(), error);
  if (error) {
    FileSystem::debug("GetFile(\"{}\") failed because path is unsafe: {}", p_path.c_str(), error.message().c_str());
    return {};
  }

  if (FileSystem::is_blacklisted(path)) {
    FileSystem::debug("GetFile(\"{}\") failed because path is blacklisted.", p_path.c_str());
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
    FileSystem::debug("DeleteFile(\"{}\") failed because FileSystemStorage is used more than once.", p_path.c_str());
    return FileSystemStatus::Denied;
  }

  std::error_code error;
  const auto path = restrict_path(p_path.c_str(), error);
  if (error) {
    FileSystem::debug("DeleteFile(\"{}\") failed because path is unsafe: {}", p_path.c_str(), error.message().c_str());
    return FileSystemStatus::Denied;
  }

  if (FileSystem::is_blacklisted(path)) {
    FileSystem::debug("DeleteFile(\"{}\") failed because path is blacklisted.", p_path.c_str());
    return FileSystemStatus::Denied;
  }

  if (!std::filesystem::is_regular_file(path, error)) {
    FileSystem::debug("DeleteFile(\"{}\") failed because path is not a file.", p_path.c_str());
    return FileSystemStatus::Failure;
  }

  const bool is_removed = std::filesystem::remove(path, error);
  if (error) {
    FileSystem::debug("DeleteFile(\"{}\") failed at OS level: {}", p_path.c_str(), error.message().c_str());
    return FileSystemStatus::Failure;
  }

  return is_removed ? FileSystemStatus::True : FileSystemStatus::False;
}

Red::Handle<AsyncFile> FileSystemStorage::get_async_file(
  const Red::CString& p_path) {
  if (!rw_permission) {
    FileSystem::debug("GetAsyncFile(\"{}\") failed because FileSystemStorage is used more than once.", p_path.c_str());
    return {};
  }

  std::error_code error;
  const auto path = restrict_path(p_path.c_str(), error);
  if (error) {
    FileSystem::debug("GetAsyncFile(\"{}\") failed because path is unsafe: {}", p_path.c_str(), error.message().c_str());
    return {};
  }

  if (FileSystem::is_blacklisted(path)) {
    FileSystem::debug("GetAsyncFile(\"{}\") failed because path is blacklisted.", p_path.c_str());
    return {};
  }

  const SharedMutex mutex = get_mutex(path);
  return Red::MakeHandle<AsyncFile>(mutex, p_path.c_str(), path);
}

Red::DynArray<Red::Handle<AsyncFile>> FileSystemStorage::get_async_files() {
  return _get_files<AsyncFile>();
}

std::filesystem::path FileSystemStorage::restrict_path(
  const std::string& p_path, std::error_code& p_error) const {
  std::filesystem::path path = storage_path / p_path;

  // NOTE: See issue regarding usage of `std::weakly_canonical` with MO2:
  //       https://github.com/ModOrganizer2/modorganizer/issues/2039
  if (FileSystem::is_mo2_detected()) {
    if (p_path.find('/') != std::string::npos ||
        p_path.find('\\') != std::string::npos) {
      p_error = std::make_error_code(std::errc::permission_denied);
    }
    return path;
  }

  const std::filesystem::path real_path = std::filesystem::weakly_canonical(path, p_error);
  if (p_error) {
    FileSystem::debug("Failed to get canonical path for \"{}\": {}", p_path.c_str(), p_error.message().c_str());
    return real_path;
  }

  if (real_path.string().find(storage_path.string() + "\\") != 0) {
    p_error = std::make_error_code(std::errc::permission_denied);
  }
  return real_path;
}

SharedMutex FileSystemStorage::get_mutex(const std::filesystem::path& p_path) {
  if (const auto it = mutexes.find(p_path); it != mutexes.end()) {
    return it->second;
  }

  const auto mutex = std::make_shared<std::mutex>();
  mutexes[p_path] = mutex;
  return mutex;
}

template <class T>
Red::DynArray<Red::Handle<T>> FileSystemStorage::_get_files() {
  if (!rw_permission) {
    if constexpr (std::is_same_v<T, File>) {
      FileSystem::debug("GetFiles() failed because FileSystemStorage is used more than once.");
    } else if constexpr (std::is_same_v<T, AsyncFile>) {
      FileSystem::debug("GetASyncFiles() failed because FileSystemStorage is used more than once.");
    }
    return {};
  }

  std::error_code error;
  auto entries = std::filesystem::directory_iterator(storage_path, error);
  if (error) {
    if constexpr (std::is_same_v<T, File>) {
      FileSystem::debug("GetFiles() failed at OS level: {}", error.message().c_str());
    } else if constexpr (std::is_same_v<T, AsyncFile>) {
      FileSystem::debug("GetAsyncFiles() failed at OS level: {}", error.message().c_str());
    }
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