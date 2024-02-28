#ifndef REDFS_JSONOBJECT_H
#define REDFS_JSONOBJECT_H

#include <RED4ext/RED4ext.hpp>
#include <RedLib.hpp>

#include "JsonVariant.h"

namespace RedFS {

class JsonObject : public JsonVariant {
 private:
  std::unordered_map<std::string, Red::Handle<JsonVariant>> fields;

 public:
  JsonObject();

  [[nodiscard]] Red::DynArray<Red::CString> get_keys() const;
  [[nodiscard]] Red::DynArray<Red::Handle<JsonVariant>> get_values() const;

  [[nodiscard]] bool has_key(const Red::CString& p_key) const;
  [[nodiscard]] Red::Handle<JsonVariant> get_key(const Red::CString& p_key) const;
  [[nodiscard]] bool get_key_bool(const Red::CString& p_key) const;
  [[nodiscard]] int64_t get_key_int64(const Red::CString& p_key) const;
  [[nodiscard]] double get_key_double(const Red::CString& p_key) const;
  [[nodiscard]] Red::CString get_key_string(const Red::CString& p_key) const;

  void insert_field(const std::string& p_key,
                    const Red::Handle<JsonVariant>& p_value);

  RTTI_IMPL_TYPEINFO(RedFS::JsonObject);
  RTTI_IMPL_ALLOCATOR();
};

}  // namespace RedFS

RTTI_DEFINE_CLASS(RedFS::JsonObject, {
  RTTI_PARENT(RedFS::JsonVariant);

  RTTI_METHOD(get_keys, "GetKeys");
  RTTI_METHOD(get_values, "GetValues");

  RTTI_METHOD(has_key, "HasKey");
  RTTI_METHOD(get_key, "GetKey");
  RTTI_METHOD(get_key_bool, "GetKeyBool");
  RTTI_METHOD(get_key_int64, "GetKeyInt64");
  RTTI_METHOD(get_key_double, "GetKeyDouble");
  RTTI_METHOD(get_key_string, "GetKeyString");
});

#endif  //REDFS_JSONOBJECT_H
