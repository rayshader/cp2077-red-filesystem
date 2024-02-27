#include "JsonString.h"

#include <utility>

namespace RedFS {

JsonString::JsonString() {
  type = JsonType::String;
}

JsonString::JsonString(std::string p_value) : value(std::move(p_value)) {
  type = JsonType::String;
}

Red::CString JsonString::get_string() const {
  return value;
}

}  // namespace RedFS