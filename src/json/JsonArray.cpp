#include "JsonArray.h"
#include "JsonFactory.h"

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

void JsonArray::set_item(uint32_t p_index,
                         const Red::Handle<JsonVariant>& p_item) {
  if (p_index >= items.size) {
    return;
  }
  items[p_index] = p_item;
}

bool JsonArray::remove_item(uint32_t p_index) {
  return items.RemoveAt(p_index);
}

void JsonArray::insert_item(uint32_t p_index,
                            const Red::Handle<JsonVariant>& p_item) {
  if (p_index >= items.size) {
    add_item(p_item);
    return;
  }
  items.PushBack(items[items.size - 1]);
  for (uint32_t i = items.size - 1; i > p_index; i--) {
    items[i] = items[i - 1];
  }
  items[p_index] = p_item;
}

void JsonArray::add_item(const Red::Handle<JsonVariant>& p_item) {
  items.PushBack(p_item);
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

void JsonArray::set_item_null(uint32_t p_index) {
  set_item(p_index, JsonFactory::CreateNull());
}

void JsonArray::set_item_bool(uint32_t p_index, bool p_value) {
  set_item(p_index, JsonFactory::CreateBool(p_value));
}

void JsonArray::set_item_int64(uint32_t p_index, int64_t p_value) {
  set_item(p_index, JsonFactory::CreateInt64(p_value));
}

void JsonArray::set_item_double(uint32_t p_index, double p_value) {
  set_item(p_index, JsonFactory::CreateDouble(p_value));
}

void JsonArray::set_item_string(uint32_t p_index, const Red::CString& p_value) {
  set_item(p_index, JsonFactory::CreateString(p_value.c_str()));
}

void JsonArray::add_item_null() {
  items.PushBack(JsonFactory::CreateNull());
}

void JsonArray::add_item_bool(bool p_item) {
  items.PushBack(JsonFactory::CreateBool(p_item));
}

void JsonArray::add_item_int64(int64_t p_item) {
  items.PushBack(JsonFactory::CreateInt64(p_item));
}

void JsonArray::add_item_double(double p_item) {
  items.PushBack(JsonFactory::CreateDouble(p_item));
}

void JsonArray::add_item_string(const Red::CString& p_item) {
  items.PushBack(JsonFactory::CreateString(p_item.c_str()));
}

void JsonArray::insert_item_null(uint32_t p_index) {
  insert_item(p_index, JsonFactory::CreateNull());
}

void JsonArray::insert_item_bool(uint32_t p_index, bool p_item) {
  insert_item(p_index, JsonFactory::CreateBool(p_item));
}

void JsonArray::insert_item_int64(uint32_t p_index, int64_t p_item) {
  insert_item(p_index, JsonFactory::CreateInt64(p_item));
}

void JsonArray::insert_item_double(uint32_t p_index, double p_item) {
  insert_item(p_index, JsonFactory::CreateDouble(p_item));
}

void JsonArray::insert_item_string(uint32_t p_index,
                                   const Red::CString& p_item) {
  insert_item(p_index, JsonFactory::CreateString(p_item.c_str()));
}

void JsonArray::clear() {
  items.Clear();
}

Red::CString JsonArray::to_string() const {
  return to_json(this);
}

}  // namespace RedFS