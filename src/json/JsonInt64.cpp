#include "JsonInt64.h"

namespace RedFS {

JsonInt64::JsonInt64() : value(0) {
  type = JsonType::Int64;
}

JsonInt64::JsonInt64(int64_t p_value) : value(p_value) {
  type = JsonType::Int64;
}

int64_t JsonInt64::get_int64() const {
  return value;
}

Red::CString JsonInt64::to_string() const {
  return std::to_string(value);
}

}  // namespace RedFS