#include "JsonObject.h"

namespace RedFS {

JsonObject::JsonObject() {
  type = JsonType::Object;
}

Red::DynArray<Red::CString> JsonObject::get_keys() const {
  Red::DynArray<Red::CString> keys;

  for (const auto& field : fields) {
    Red::CString key = field.first.ToString();

    keys.PushBack(key);
  }
  return keys;
}

Red::DynArray<Red::Handle<JsonVariant>> JsonObject::get_values() const {
  Red::DynArray<Red::Handle<JsonVariant>> keys;

  for (const auto& field : fields) {
    keys.PushBack(field.second);
  }
  return keys;
}

bool JsonObject::has_key(const Red::CString& p_key) const {
  return fields.contains(Red::CName(p_key.c_str()));
}

Red::Handle<JsonVariant> JsonObject::get_key(const Red::CString& p_key) const {
  return fields.at(Red::CName(p_key.c_str()));
}

bool JsonObject::get_key_bool(const Red::CString& p_key) const {
  return fields.at(Red::CName(p_key.c_str()))->get_bool();
}

int64_t JsonObject::get_key_int64(const Red::CString& p_key) const {
  return fields.at(Red::CName(p_key.c_str()))->get_int64();
}

double JsonObject::get_key_double(const Red::CString& p_key) const {
  return fields.at(Red::CName(p_key.c_str()))->get_double();
}

Red::CString JsonObject::get_key_string(const Red::CString& p_key) const {
  return fields.at(Red::CName(p_key.c_str()))->get_string();
}

void JsonObject::insert_field(const Red::CName& p_key,
                              const Red::Handle<JsonVariant>& p_value) {
  fields[p_key] = p_value;
}

}  // namespace RedFS