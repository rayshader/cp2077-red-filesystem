#ifndef REDFS_FILE_H
#define REDFS_FILE_H

#include <filesystem>

#include <simdjson.h>
#include <RED4ext/RED4ext.hpp>
#include <RedLib.hpp>

#include "FileSystemWriteMode.h"
#include "JsonArray.h"
#include "JsonObject.h"
#include "JsonVariant.h"

namespace RedFS {

class File : public Red::IScriptable {
 private:
  const std::filesystem::path path;
  const std::filesystem::path absolute_path;

  static void parse_object(const simdjson::dom::object& p_object,
                           Red::Handle<JsonObject>& p_root);
  static void parse_array(const simdjson::dom::array& p_array,
                          Red::Handle<JsonArray>& p_root);
  static std::ios_base::openmode get_mode(FileSystemWriteMode p_mode);

 public:
  File() = default;
  explicit File(std::filesystem::path p_path,
                std::filesystem::path p_absolute_path);

  [[nodiscard]] Red::CString get_path() const;
  [[nodiscard]] Red::CString get_absolute_path() const;
  [[nodiscard]] Red::CString get_filename() const;
  [[nodiscard]] Red::CString get_extension() const;
  [[nodiscard]] uint64_t get_size() const;

  Red::CString read_as_text();
  Red::DynArray<Red::CString> read_as_lines();
  Red::Handle<JsonVariant> read_as_json();

  bool write_text(const Red::CString& p_text,
                  const Red::Optional<FileSystemWriteMode>& p_mode);
  bool write_lines(const Red::DynArray<Red::CString>& p_lines,
                   const Red::Optional<FileSystemWriteMode>& p_mode);
  bool write_json(const Red::Handle<JsonVariant>& p_json);

  RTTI_IMPL_TYPEINFO(RedFS::File);
  RTTI_IMPL_ALLOCATOR();
};

}  // namespace RedFS

RTTI_DEFINE_CLASS(RedFS::File, {
  RTTI_ALIAS("RedFileSystem.File");

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

#endif  //REDFS_FILE_H
