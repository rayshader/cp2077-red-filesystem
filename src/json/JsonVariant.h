#ifndef REDFS_JSONVARIANT_H
#define REDFS_JSONVARIANT_H

#include <RED4ext/RED4ext.hpp>
#include <RedLib.hpp>

#include "JsonType.h"

namespace RedFS {

class JsonVariant : public Red::IScriptable {
 protected:
  JsonType type;

 public:
  JsonVariant();

  static std::string to_json(const JsonVariant* p_json,
                             const std::string& p_indent);

  [[nodiscard]] bool is_undefined() const;
  [[nodiscard]] bool is_null() const;
  [[nodiscard]] bool is_bool() const;
  [[nodiscard]] bool is_int64() const;
  [[nodiscard]] bool is_double() const;
  [[nodiscard]] bool is_string() const;
  [[nodiscard]] bool is_array() const;
  [[nodiscard]] bool is_object() const;

  [[nodiscard]] virtual bool get_bool() const;
  [[nodiscard]] virtual int64_t get_int64() const;
  [[nodiscard]] virtual double get_double() const;
  [[nodiscard]] virtual Red::CString get_string() const;

  [[nodiscard]] virtual Red::CString to_string() const;

  RTTI_IMPL_TYPEINFO(RedFS::JsonVariant);
  RTTI_IMPL_ALLOCATOR();
};

using JsonUndefined = JsonVariant;

}  // namespace RedFS

RTTI_DEFINE_CLASS(RedFS::JsonVariant, {
  RTTI_ALIAS("RedFileSystem.Json.JsonVariant");

  RTTI_ABSTRACT();

  RTTI_METHOD(is_undefined, "IsUndefined");
  RTTI_METHOD(is_null, "IsNull");
  RTTI_METHOD(is_bool, "IsBool");
  RTTI_METHOD(is_int64, "IsInt64");
  RTTI_METHOD(is_double, "IsDouble");
  RTTI_METHOD(is_string, "IsString");
  RTTI_METHOD(is_array, "IsArray");
  RTTI_METHOD(is_object, "IsObject");

  RTTI_METHOD(get_bool, "GetBool");
  RTTI_METHOD(get_int64, "GetInt64");
  RTTI_METHOD(get_double, "GetDouble");
  RTTI_METHOD(get_string, "GetString");

  RTTI_METHOD(to_string, "ToString");
});

#endif  //REDFS_JSONVARIANT_H
