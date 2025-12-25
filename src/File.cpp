#include "File.h"

#include <fstream>
#include <utility>

#include <RED4ext/RED4ext.hpp>
#include <RedData.hpp>
#include <RedLib.hpp>

#include "Base64.h"
#include "FileSystem.h"

namespace RedFS {

File::File(SharedMutex p_mutex, std::filesystem::path p_path,
           std::filesystem::path p_absolute_path)
    : mutex(std::move(p_mutex)),
      path(std::move(p_path)),
      absolute_path(std::move(p_absolute_path)) {}

Red::CString File::get_path() const {
  return path.string();
}

Red::CString File::get_absolute_path() const {
  return absolute_path.string();
}

Red::CString File::get_filename() const {
  return path.filename().string();
}

Red::CString File::get_extension() const {
  return path.extension().string();
}

uint64_t File::get_size() const {
  std::error_code error;

  return std::filesystem::file_size(absolute_path, error);
}

Red::CString File::read_as_text() {
  mutex->lock();

  std::ifstream stream(absolute_path);
  if (!stream) {
    mutex->unlock();

    const std::string error = FileSystem::get_error_message(errno);
    FileSystem::debug("Failed to read text from \"{}\": {}", path.string().c_str(), error.c_str());
    return {};
  }

  std::stringstream data;
  data << stream.rdbuf();
  stream.close();

  mutex->unlock();

  return data.str();
}

Red::CString File::read_as_base64() {
  mutex->lock();

  std::ifstream stream(absolute_path, std::ios::binary);
  if (!stream) {
    mutex->unlock();

    const std::string error = FileSystem::get_error_message(errno);
    FileSystem::debug("Failed to read as base64 from \"{}\": {}", path.string().c_str(), error.c_str());
    return {};
  }

  const std::string buffer(std::istreambuf_iterator(stream), {});

  mutex->unlock();

  return base64::to_base64(buffer);
}

Red::DynArray<Red::CString> File::read_as_lines() {
  mutex->lock();

  std::ifstream stream(absolute_path);
  if (!stream) {
    mutex->unlock();

    const std::string error = FileSystem::get_error_message(errno);
    FileSystem::debug("Failed to read lines from \"{}\": {}", path.string().c_str(), error.c_str());
    return {};
  }

  Red::DynArray<Red::CString> lines;
  std::string line;
  while (std::getline(stream, line)) {
    if (!stream) {
      break;
    }
    lines.EmplaceBack(line);
  }
  stream.close();

  mutex->unlock();

  return lines;
}

Red::Handle<Red::IScriptable> File::read_as_json() {
  if (!RedData::IsDetected()) {
    return {};
  }

  const Red::CString text = read_as_text();
  const RedData::Json::JsonVariant json = RedData::Json::ParseJson(text);
  return json.GetHandle();
}

bool File::write_text(const Red::CString& p_text,
                      const Red::Optional<FileSystemWriteMode>& p_mode) {
  mutex->lock();

  const std::ios_base::openmode mode = get_mode(p_mode.value);
  std::ofstream stream(absolute_path, mode);
  if (!stream) {
    mutex->unlock();

    const std::string error = FileSystem::get_error_message(errno);
    FileSystem::debug("Failed to write text in \"{}\": {}", path.string().c_str(), error.c_str());
    return false;
  }

  stream << p_text.c_str();
  stream.close();

  mutex->unlock();

  return true;
}

bool File::write_lines(const Red::DynArray<Red::CString>& p_lines,
                       const Red::Optional<FileSystemWriteMode>& p_mode) {
  mutex->lock();

  const std::ios_base::openmode mode = get_mode(p_mode.value);
  std::ofstream stream(absolute_path, mode);
  if (!stream) {
    mutex->unlock();

    const std::string error = FileSystem::get_error_message(errno);
    FileSystem::debug("Failed to write lines in \"{}\": {}", path.string().c_str(), error.c_str());
    return false;
  }

  for (uint32_t i = 0; i < p_lines.size; i++) {
    stream << p_lines[i].c_str();
    if (i + 1 < p_lines.size) {
      stream << '\n';
    }
  }
  stream.close();

  mutex->unlock();

  return true;
}

bool File::write_json(const Red::Handle<Red::IScriptable>& p_json,
                      const Red::Optional<Red::CString>& p_indent) {
  if (!RedData::IsDetected()) {
    return false;
  }

  const RedData::Json::JsonVariant json(p_json);
  const Red::CString text = json.ToString(p_indent.value);
  Red::Optional<FileSystemWriteMode> mode;
  mode.value = FileSystemWriteMode::Truncate;
  return write_text(text, mode);
}

std::ios_base::openmode File::get_mode(const FileSystemWriteMode p_mode) {
  std::ios_base::openmode mode = std::ios_base::trunc;
  if (p_mode == FileSystemWriteMode::Append) {
    mode = std::ios_base::app;
  }
  return mode;
}

}  // namespace RedFS