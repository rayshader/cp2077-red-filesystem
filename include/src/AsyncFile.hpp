#ifndef REDFILESYSTEM_ASYNCFILE_HPP
#define REDFILESYSTEM_ASYNCFILE_HPP

#include <RED4ext/RED4ext.hpp>
#include <RedLib.hpp>

#ifdef HAS_REDDATA
#include <RedData.hpp>
#endif

#include "Enums.hpp"
#include "FilePromise.hpp"

namespace RedFileSystem {

class AsyncFile {
 private:
  Red::Handle<Red::IScriptable> wrapper;

 public:
  AsyncFile() = default;

  explicit AsyncFile(const Red::Handle<Red::IScriptable>& wrapper)
      : wrapper(wrapper) {}

  explicit operator bool() const noexcept { return wrapper; }

  inline Red::CString GetPath() const {
    Red::CString path;

    Red::CallVirtual(wrapper, "GetPath", path);
    return path;
  }

  inline Red::CString GetAbsolutePath() const {
    Red::CString path;

    Red::CallVirtual(wrapper, "GetAbsolutePath", path);
    return path;
  }

  inline Red::CString GetFilename() const {
    Red::CString filename;

    Red::CallVirtual(wrapper, "GetFilename", filename);
    return filename;
  }

  inline Red::CString GetExtension() const {
    Red::CString extension;

    Red::CallVirtual(wrapper, "GetExtension", extension);
    return extension;
  }

  inline uint64_t GetSize() const {
    uint64_t size;

    Red::CallVirtual(wrapper, "GetSize", size);
    return size;
  }

  inline void ReadAsText(const FilePromise& promise) {
    Red::CallVirtual(wrapper, "ReadAsText", promise);
  }

  inline void ReadAsLines(const FilePromise& promise) {
    Red::CallVirtual(wrapper, "ReadAsLines", promise);
  }

#ifdef HAS_REDDATA
  inline void ReadAsJson(const FilePromise& promise) {
    Red::CallVirtual(wrapper, "ReadAsJson", promise);
  }
#endif

  inline void WriteText(
    const FilePromise& promise, const Red::CString& text,
    FileSystemWriteMode mode = FileSystemWriteMode::Truncate) {
    Red::CallVirtual(wrapper, "WriteText", promise, text, mode);
  }

  inline void WriteLines(
    const FilePromise& promise, const Red::DynArray<Red::CString>& lines,
    FileSystemWriteMode mode = FileSystemWriteMode::Truncate) {
    Red::CallVirtual(wrapper, "WriteLines", promise, lines, mode);
  }

#ifdef HAS_REDDATA
  inline bool WriteJson(const FilePromise& promise,
                        const RedData::Json::JsonVariant& json,
                        const Red::CString& indent = "") {
    Red::CallVirtual(wrapper, "WriteJson", promise, json.GetHandle(), indent);
  }
#endif
};

}  // namespace RedFileSystem

#endif  //REDFILESYSTEM_ASYNCFILE_HPP
