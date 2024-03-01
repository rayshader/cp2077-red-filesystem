#ifndef REDFS_JSONOBJECT_H
#define REDFS_JSONOBJECT_H

#include <RED4ext/RED4ext.hpp>
#include <RedLib.hpp>

#include "JsonArray.h"
#include "JsonVariant.h"

namespace RedFS {

class JsonObject : public JsonVariant {
 private:
  std::unordered_map<std::string, Red::Handle<JsonVariant>> fields;

 public:
  JsonObject();

  static std::string to_json(const JsonObject* p_object,
                             const std::string& p_indent = "");

  [[nodiscard]] Red::DynArray<Red::CString> get_keys() const;
  [[nodiscard]] Red::DynArray<Red::Handle<JsonVariant>> get_values() const;

  [[nodiscard]] bool has_key(const Red::CString& p_key) const;
  [[nodiscard]] Red::Handle<JsonVariant> get_key(
    const Red::CString& p_key) const;
  inline void set_key(const Red::CString& p_key,
                      const Red::Handle<JsonVariant>& p_value);
  void remove_key(const Red::CString& p_key);

  [[nodiscard]] bool get_key_bool(const Red::CString& p_key) const;
  [[nodiscard]] int64_t get_key_int64(const Red::CString& p_key) const;
  [[nodiscard]] double get_key_double(const Red::CString& p_key) const;
  [[nodiscard]] Red::CString get_key_string(const Red::CString& p_key) const;

  void set_key_null(const Red::CString& p_key);
  void set_key_bool(const Red::CString& p_key, bool p_value);
  void set_key_int64(const Red::CString& p_key, int64_t p_value);
  void set_key_double(const Red::CString& p_key, double p_value);
  void set_key_string(const Red::CString& p_key, const Red::CString& p_value);

  void clear();

  [[nodiscard]] Red::CString to_string() const override;

  [[nodiscard]] std::vector<std::string> get_string_keys() const;

  RTTI_IMPL_TYPEINFO(RedFS::JsonObject);
  RTTI_IMPL_ALLOCATOR();
};

}  // namespace RedFS

RTTI_DEFINE_CLASS(RedFS::JsonObject, {
  RTTI_ALIAS("RedFileSystem.Json.JsonObject");

  RTTI_PARENT(RedFS::JsonVariant);

  RTTI_METHOD(get_keys, "GetKeys");
  RTTI_METHOD(get_values, "GetValues");

  RTTI_METHOD(has_key, "HasKey");
  RTTI_METHOD(get_key, "GetKey");
  RTTI_METHOD(set_key, "SetKey");
  RTTI_METHOD(remove_key, "RemoveKey");

  RTTI_METHOD(get_key_bool, "GetKeyBool");
  RTTI_METHOD(get_key_int64, "GetKeyInt64");
  RTTI_METHOD(get_key_double, "GetKeyDouble");
  RTTI_METHOD(get_key_string, "GetKeyString");

  RTTI_METHOD(set_key_null, "SetKeyNull");
  RTTI_METHOD(set_key_bool, "SetKeyBool");
  RTTI_METHOD(set_key_int64, "SetKeyInt64");
  RTTI_METHOD(set_key_double, "SetKeyDouble");
  RTTI_METHOD(set_key_string, "SetKeyString");

  RTTI_METHOD(clear, "Clear");

  RTTI_METHOD(to_string, "ToString");
});

#endif  //REDFS_JSONOBJECT_H
