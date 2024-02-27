#include "JsonArray.h"

namespace RedFS {

JsonArray::JsonArray() {
  type = JsonType::Array;
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

void JsonArray::push_back(const Red::Handle<JsonVariant>& p_item) {
  items.PushBack(p_item);
}

}  // namespace RedFS