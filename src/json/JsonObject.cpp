#include "JsonObject.h"
#include "JsonFactory.h"

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

void JsonObject::set_key(const Red::CString& p_key,
                         const Red::Handle<JsonVariant>& p_value) {
  fields[p_key.c_str()] = p_value;
}

void JsonObject::remove_key(const Red::CString& p_key) {
  fields.erase(p_key.c_str());
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

void JsonObject::set_key_null(const Red::CString& p_key) {
  set_key(p_key, JsonFactory::CreateNull());
}

void JsonObject::set_key_bool(const Red::CString& p_key, bool p_value) {
  set_key(p_key, JsonFactory::CreateBool(p_value));
}

void JsonObject::set_key_int64(const Red::CString& p_key, int64_t p_value) {
  set_key(p_key, JsonFactory::CreateInt64(p_value));
}

void JsonObject::set_key_double(const Red::CString& p_key, double p_value) {
  set_key(p_key, JsonFactory::CreateDouble(p_value));
}

void JsonObject::set_key_string(const Red::CString& p_key,
                                const Red::CString& p_value) {
  set_key(p_key, JsonFactory::CreateString(p_value.c_str()));
}

void JsonObject::clear() {
  fields.clear();
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

}  // namespace RedFS