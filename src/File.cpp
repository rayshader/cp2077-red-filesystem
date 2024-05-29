#include "File.h"

#include <fstream>
#include <utility>

#include <RED4ext/RED4ext.hpp>
#include <RedData.hpp>
#include <RedLib.hpp>

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
  std::ifstream stream;

  stream.open(absolute_path);
  if (!stream.is_open()) {
    mutex->unlock();
    return {};
  }
  std::stringstream data;

  data << stream.rdbuf();
  stream.close();
  mutex->unlock();
  return data.str();
}

Red::DynArray<Red::CString> File::read_as_lines() {
  mutex->lock();
  std::ifstream stream;

  stream.open(absolute_path);
  if (!stream.is_open()) {
    mutex->unlock();
    return {};
  }
  Red::DynArray<Red::CString> lines;
  std::string line;

  while (std::getline(stream, line)) {
    if (stream.fail() || stream.bad()) {
      stream.close();
      mutex->unlock();
      return lines;
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
  Red::CString text = read_as_text();
  RedData::Json::JsonVariant json = RedData::Json::ParseJson(text);

  return json.GetHandle();
}

bool File::write_text(const Red::CString& p_text,
                      const Red::Optional<FileSystemWriteMode>& p_mode) {
  mutex->lock();
  std::ios_base::openmode mode = get_mode(p_mode.value);
  std::ofstream stream;

  // create directories if they don't exist
  std::filesystem::create_directories(absolute_path.parent_path());

  stream.open(absolute_path, mode);
  if (!stream.is_open()) {
    mutex->unlock();
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
  std::ios_base::openmode mode = get_mode(p_mode.value);
  std::ofstream stream;

  // create directories if they don't exist
  std::filesystem::create_directories(absolute_path.parent_path());

  stream.open(absolute_path, mode);
  if (!stream.is_open()) {
    mutex->unlock();
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
  RedData::Json::JsonVariant json(p_json);
  Red::CString text = json.ToString(p_indent.value);
  Red::Optional<FileSystemWriteMode> mode;

  mode.value = FileSystemWriteMode::Truncate;
  return write_text(text, mode);
}

std::ios_base::openmode File::get_mode(FileSystemWriteMode p_mode) {
  std::ios_base::openmode mode = std::ios_base::trunc;

  if (p_mode == FileSystemWriteMode::Append) {
    mode = std::ios_base::app;
  }
  return mode;
}

}  // namespace RedFS