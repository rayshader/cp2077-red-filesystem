#include "JsonObject.h"

namespace RedFS {

JsonObject::JsonObject() {
  type = JsonType::Object;
}

std::string JsonObject::to_json(const JsonObject* p_object,
                                const std::string& p_indent) {
  auto keys = p_object->get_string_keys();
  std::string json = "{";

  for (int i = 0; i < keys.size(); i++) {
    auto key = keys[i];
    auto value = p_object->get_key(key);

    json.append("\n");
    json.append(p_indent);
    json.append("  ");
    json.append("\"");
    json.append(key);
    json.append("\": ");
    json.append(JsonVariant::to_json(value, p_indent + "  "));
    if (i + 1 < keys.size()) {
      json.append(",");
    }
  }
  if (!keys.empty()) {
    json.append("\n");
  }
  json.append(p_indent);
  json.append("}");
  return json;
}

Red::DynArray<Red::CString> JsonObject::get_keys() const {
  Red::DynArray<Red::CString> keys;

  for (const auto& field : fields) {
    Red::CString key = field.first;

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
  return fields.contains(p_key.c_str());
}

Red::Handle<JsonVariant> JsonObject::get_key(const Red::CString& p_key) const {
  std::string key = p_key.c_str();

  if (!fields.contains(key)) {
    return {};
  }
  return fields.at(key);
}

bool JsonObject::get_key_bool(const Red::CString& p_key) const {
  std::string key = p_key.c_str();

  if (!fields.contains(key)) {
    return {};
  }
  return fields.at(key)->get_bool();
}

int64_t JsonObject::get_key_int64(const Red::CString& p_key) const {
  std::string key = p_key.c_str();

  if (!fields.contains(key)) {
    return {};
  }
  return fields.at(key)->get_int64();
}

double JsonObject::get_key_double(const Red::CString& p_key) const {
  std::string key = p_key.c_str();

  if (!fields.contains(key)) {
    return {};
  }
  return fields.at(key)->get_double();
}

Red::CString JsonObject::get_key_string(const Red::CString& p_key) const {
  std::string key = p_key.c_str();

  if (!fields.contains(key)) {
    return {};
  }
  return fields.at(key)->get_string();
}

Red::CString JsonObject::to_string() const {
  return to_json(this);
}

std::vector<std::string> JsonObject::get_string_keys() const {
  std::vector<std::string> keys;

  for (const auto& field : fields) {
    keys.push_back(field.first);
  }
  return keys;
}

void JsonObject::insert_field(const std::string& p_key,
                              const Red::Handle<JsonVariant>& p_value) {
  fields[p_key] = p_value;
}

}  // namespace RedFS