#include "JsonVariant.h"

namespace RedFS {

JsonVariant::JsonVariant() : type(JsonType::Undefined) {

}

bool JsonVariant::is_undefined() const {
  return type == JsonType::Undefined;
}

bool JsonVariant::is_null() const {
  return type == JsonType::Null;
}

bool JsonVariant::is_bool() const {
  return type == JsonType::Bool;
}

bool JsonVariant::is_int64() const {
  return type == JsonType::Int64;
}

bool JsonVariant::is_double() const {
  return type == JsonType::Double;
}

bool JsonVariant::is_string() const {
  return type == JsonType::String;
}

bool JsonVariant::is_array() const {
  return type == JsonType::Array;
}

bool JsonVariant::is_object() const {
  return type == JsonType::Object;
}

bool JsonVariant::get_bool() const {
  return false;
}

int64_t JsonVariant::get_int64() const {
  return 0;
}

double JsonVariant::get_double() const {
  return 0;
}

Red::CString JsonVariant::get_string() const {
  return {};
}

}