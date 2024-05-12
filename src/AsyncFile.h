#ifndef REDFILESYSTEM_ASYNCFILE_H
#define REDFILESYSTEM_ASYNCFILE_H

#include <filesystem>

#include <RED4ext/RED4ext.hpp>
#include <RedLib.hpp>

#include "FilePromise.h"
#include "FileSystemWriteMode.h"

namespace RedFS {

class AsyncFile : public Red::IScriptable {
 private:
  const std::filesystem::path path;
  const std::filesystem::path absolute_path;

 public:
  AsyncFile() = default;
  explicit AsyncFile(std::filesystem::path p_path,
                     std::filesystem::path p_absolute_path);

  [[nodiscard]] Red::CString get_path() const;
  [[nodiscard]] Red::CString get_absolute_path() const;
  [[nodiscard]] Red::CString get_filename() const;
  [[nodiscard]] Red::CString get_extension() const;
  [[nodiscard]] uint64_t get_size() const;

  void read_as_text(const FilePromise& p_promise);
  void read_as_lines(const FilePromise& p_promise);
  void read_as_json(const FilePromise& p_promise);

  void write_text(const FilePromise& p_promise, const Red::CString& p_text,
                  const Red::Optional<FileSystemWriteMode>& p_mode);
  void write_lines(const FilePromise& p_promise,
                   const Red::DynArray<Red::CString>& p_lines,
                   const Red::Optional<FileSystemWriteMode>& p_mode);
  void write_json(const FilePromise& p_promise,
                  const Red::Handle<Red::IScriptable>& p_json,
                  const Red::Optional<Red::CString>& p_indent);

  RTTI_IMPL_TYPEINFO(RedFS::AsyncFile);
  RTTI_IMPL_ALLOCATOR();
};

}  // namespace RedFS

RTTI_DEFINE_CLASS(RedFS::AsyncFile, {
  RTTI_ALIAS("RedFileSystem.AsyncFile");

  RTTI_METHOD(get_path, "GetPath");
  RTTI_METHOD(get_absolute_path, "GetAbsolutePath");
  RTTI_METHOD(get_filename, "GetFilename");
  RTTI_METHOD(get_extension, "GetExtension");
  RTTI_METHOD(get_size, "GetSize");

  RTTI_METHOD(read_as_text, "ReadAsText");
  RTTI_METHOD(read_as_lines, "ReadAsLines");
  RTTI_METHOD(read_as_json, "ReadAsJson");

  RTTI_METHOD(write_text, "WriteText");
  RTTI_METHOD(write_lines, "WriteLines");
  RTTI_METHOD(write_json, "WriteJson");
});

#endif  //REDFILESYSTEM_ASYNCFILE_H
