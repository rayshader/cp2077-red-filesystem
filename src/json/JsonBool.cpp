#include "JsonBool.h"

namespace RedFS {

JsonBool::JsonBool() : value(false) {
  type = JsonType::Bool;
}

JsonBool::JsonBool(bool p_value) : value(p_value) {
  type = JsonType::Bool;
}

bool JsonBool::get_bool() const {
  return value;
}

Red::CString JsonBool::to_string() const {
  return value ? "true" : "false";
}

}  // namespace RedFS