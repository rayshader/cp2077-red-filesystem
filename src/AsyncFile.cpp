#include "AsyncFile.h"
#include "File.h"

#include <RedData.hpp>
#include <fstream>
#include <utility>

#include "Base64.h"

namespace RedFS {
AsyncFile::AsyncFile(SharedMutex p_mutex, std::filesystem::path p_path,
                     std::filesystem::path p_absolute_path)
    : mutex(std::move(p_mutex)),
      path(std::move(p_path)),
      absolute_path(std::move(p_absolute_path)) {}

Red::CString AsyncFile::get_path() const {
  return path.string();
}

Red::CString AsyncFile::get_absolute_path() const {
  return absolute_path.string();
}

Red::CString AsyncFile::get_filename() const {
  return path.filename().string();
}

Red::CString AsyncFile::get_extension() const {
  return path.extension().string();
}

uint64_t AsyncFile::get_size() const {
  std::error_code error;

  return std::filesystem::file_size(absolute_path, error);
}

void AsyncFile::read_as_text(const FilePromise& p_promise) {
  Red::JobQueue job_queue;

  job_queue.Dispatch([*this, p_promise]() -> void {
    mutex->lock();
    std::ifstream stream;

    stream.open(absolute_path);
    if (!stream.is_open()) {
      mutex->unlock();
      p_promise.reject();
      return;
    }
    std::stringstream data;

    data << stream.rdbuf();
    stream.close();
    mutex->unlock();
    Red::CString text = data.str();

    p_promise.resolve(text);
  });
}

void AsyncFile::read_as_base64(const FilePromise& p_promise) {
  Red::JobQueue job_queue;

  job_queue.Dispatch([*this, p_promise]() -> void {
    mutex->lock();
    std::ifstream stream(absolute_path, std::ios::binary);
    const std::string buffer(std::istreambuf_iterator(stream), {});

    mutex->unlock();
    const Red::CString data = base64::to_base64(buffer);

    p_promise.resolve(data);
  });
}

void AsyncFile::read_as_lines(const FilePromise& p_promise) {
  Red::JobQueue job_queue;

  job_queue.Dispatch([*this, p_promise]() -> void {
    mutex->lock();
    std::ifstream stream;

    stream.open(absolute_path);
    if (!stream.is_open()) {
      mutex->unlock();
      p_promise.reject();
      return;
    }
    Red::DynArray<Red::CString> lines;
    std::string line;

    while (std::getline(stream, line)) {
      if (stream.fail() || stream.bad()) {
        stream.close();
        mutex->unlock();
        p_promise.reject();
        return;
      }
      lines.EmplaceBack(line);
    }
    stream.close();
    mutex->unlock();
    p_promise.resolve(lines);
  });
}

void AsyncFile::read_as_json(const FilePromise& p_promise) {
  if (!RedData::IsDetected()) {
    p_promise.reject();
    return;
  }
  Red::JobQueue job_queue;

  job_queue.Dispatch([*this, p_promise]() -> void {
    mutex->lock();
    std::ifstream stream;

    stream.open(absolute_path);
    if (!stream.is_open()) {
      mutex->unlock();
      p_promise.reject();
      return;
    }
    std::stringstream data;

    data << stream.rdbuf();
    stream.close();
    mutex->unlock();
    Red::CString text = data.str();
    RedData::Json::JsonVariant json = RedData::Json::ParseJson(text);

    p_promise.resolve(json.GetHandle());
  });
}

void AsyncFile::write_text(const FilePromise& p_promise,
                           const Red::CString& p_text,
                           const Red::Optional<FileSystemWriteMode>& p_mode) {
  std::ios_base::openmode mode = File::get_mode(p_mode.value);
  Red::JobQueue job_queue;

  job_queue.Dispatch([*this, p_promise, p_text, mode]() -> void {
    mutex->lock();
    std::ofstream stream;

    stream.open(absolute_path, mode);
    if (!stream.is_open()) {
      mutex->unlock();
      p_promise.reject();
      return;
    }
    stream << p_text.c_str();
    stream.close();
    mutex->unlock();
    p_promise.resolve();
  });
}

void AsyncFile::write_lines(const FilePromise& p_promise,
                            const Red::DynArray<Red::CString>& p_lines,
                            const Red::Optional<FileSystemWriteMode>& p_mode) {
  std::ios_base::openmode mode = File::get_mode(p_mode.value);
  Red::JobQueue job_queue;

  job_queue.Dispatch([*this, p_promise, p_lines, mode]() -> void {
    mutex->lock();
    std::ofstream stream;

    stream.open(absolute_path, mode);
    if (!stream.is_open()) {
      mutex->unlock();
      p_promise.reject();
      return;
    }
    for (uint32_t i = 0; i < p_lines.size; i++) {
      stream << p_lines[i].c_str();
      if (i + 1 < p_lines.size) {
        stream << '\n';
      }
    }
    stream.close();
    mutex->unlock();
    p_promise.resolve();
  });
}

void AsyncFile::write_json(const FilePromise& p_promise,
                           const Red::Handle<Red::IScriptable>& p_json,
                           const Red::Optional<Red::CString>& p_indent) {
  if (!RedData::IsDetected()) {
    p_promise.reject();
    return;
  }
  RedData::Json::JsonVariant json = RedData::Json::JsonVariant(p_json);
  Red::CString text = json.ToString(p_indent.value);
  Red::Optional<FileSystemWriteMode> mode;

  mode.value = FileSystemWriteMode::Truncate;
  write_text(p_promise, text, mode);
}

}  // namespace RedFS