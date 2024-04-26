#ifndef REDFILESYSTEM_FILE_HPP
#define REDFILESYSTEM_FILE_HPP

#include <RED4ext/RED4ext.hpp>
#include <RedLib.hpp>

#ifdef HAS_REDDATA
#include <RedData.hpp>
#endif

#include "Enums.hpp"

namespace RedFileSystem {

class File {
 private:
  Red::Handle<Red::IScriptable> wrapper;

 public:
  File() = default;

  explicit File(const Red::Handle<Red::IScriptable>& wrapper)
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

  inline Red::CString ReadAsText() {
    Red::CString text;

    Red::CallVirtual(wrapper, "ReadAsText", text);
    return text;
  }

  inline Red::DynArray<Red::CString> ReadAsLines() {
    Red::DynArray<Red::CString> lines;

    Red::CallVirtual(wrapper, "ReadAsLines", lines);
    return lines;
  }

#ifdef HAS_REDDATA
  inline RedData::Json::JsonVariant ReadAsJson() {
    Red::Handle<Red::IScriptable> json;

    Red::CallVirtual(wrapper, "ReadAsJson", json);
    return json;
  }
#endif

  inline bool WriteText(
    const Red::CString& text,
    FileSystemWriteMode mode = FileSystemWriteMode::Truncate) {
    bool status;

    Red::CallVirtual(wrapper, "WriteText", status, text, mode);
    return status;
  }

  inline bool WriteLines(
    const Red::DynArray<Red::CString>& lines,
    FileSystemWriteMode mode = FileSystemWriteMode::Truncate) {
    bool status;

    Red::CallVirtual(wrapper, "WriteLines", status, lines, mode);
    return status;
  }

#ifdef HAS_REDDATA
  inline bool WriteJson(const RedData::Json::JsonVariant& json,
                        const Red::CString& indent = "") {
    bool status;

    Red::CallVirtual(wrapper, "WriteJson", status, json.GetHandle(), indent);
    return status;
  }
#endif
};

}  // namespace RedFileSystem

#endif  //REDFILESYSTEM_FILE_HPP
