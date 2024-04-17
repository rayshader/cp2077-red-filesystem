#ifndef REDFILESYSTEM_FILE_HPP
#define REDFILESYSTEM_FILE_HPP

#include <RED4ext/RED4ext.hpp>
#include <RedLib.hpp>

namespace RedFileSystem {

class File {
 private:
  Red::Handle<Red::IScriptable> wrapper;

 public:
  explicit File(const Red::Handle<Red::IScriptable>& acWrapper)
      : wrapper(acWrapper) {}

  bool operator() const { return wrapper; }

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

  inline Red::Handle<Red::IScriptable> ReadAsJson() {
    Red::Handle<Red::IScriptable> json;

    Red::CallVirtual(wrapper, "ReadAsJson", json);
    return json;
  }

  inline bool WriteText(
    const Red::CString& acText,
    FileSystemWriteMode aMode = FileSystemWriteMode::Truncate) {
    bool status;

    Red::CallVirtual(wrapper, "WriteText", status, acText, aMode);
    return status;
  }

  inline bool WriteLines(
    const Red::DynArray<Red::CString>& acLines,
    FileSystemWriteMode aMode = FileSystemWriteMode::Truncate) {
    bool status;

    Red::CallVirtual(wrapper, "WriteLines", status, acLines, aMode);
    return status;
  }

  inline bool WriteJson(const Red::Handle<Red::IScriptable>& acJson) {
    bool status;

    Red::CallVirtual(wrapper, "WriteJson", status, acJson);
    return status;
  }
};

}  // namespace RedFileSystem

#endif  //REDFILESYSTEM_FILE_HPP
