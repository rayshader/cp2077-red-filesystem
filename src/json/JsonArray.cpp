#include "JsonArray.h"

namespace RedFS {

JsonArray::JsonArray() {
  type = JsonType::Array;
}

std::string JsonArray::to_json(const JsonArray* p_array,
                               const std::string& p_indent) {
  uint32_t size = p_array->get_size();
  std::string json = "[";

  for (uint32_t i = 0; i < size; i++) {
    auto item = p_array->get_item(i);

    json.append("\n");
    json.append(p_indent);
    json.append("  ");
    json += JsonVariant::to_json(item, p_indent + "  ");
    if (i + 1 < size) {
      json.append(",");
    }
  }
  if (size > 0) {
    json.append("\n");
  }
  json.append(p_indent);
  json.append("]");
  return json;
}

uint32_t JsonArray::get_size() const {
  return items.size;
}

Red::Handle<JsonVariant> JsonArray::get_item(uint32_t p_index) const {
  if (p_index >= items.size) {
    return {};
  }
  return items[p_index];
}

bool JsonArray::get_item_bool(uint32_t p_index) const {
  if (p_index >= items.size) {
    return {};
  }
  return items[p_index]->get_bool();
}

int64_t JsonArray::get_item_int64(uint32_t p_index) const {
  if (p_index >= items.size) {
    return {};
  }
  return items[p_index]->get_int64();
}

double JsonArray::get_item_double(uint32_t p_index) const {
  if (p_index >= items.size) {
    return {};
  }
  return items[p_index]->get_double();
}

Red::CString JsonArray::get_item_string(uint32_t p_index) const {
  if (p_index >= items.size) {
    return {};
  }
  return items[p_index]->get_string();
}

Red::CString JsonArray::to_string() const {
  return to_json(this);
}

void JsonArray::push_back(const Red::Handle<JsonVariant>& p_item) {
  items.PushBack(p_item);
}

}  // namespace RedFS