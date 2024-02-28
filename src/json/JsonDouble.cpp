#include "JsonDouble.h"

#include <sstream>

namespace RedFS {

JsonDouble::JsonDouble() : value(0) {
  type = JsonType::Double;
}

JsonDouble::JsonDouble(double p_value) : value(p_value) {
  type = JsonType::Double;
}

double JsonDouble::get_double() const {
  return value;
}

Red::CString JsonDouble::to_string() const {
  std::stringstream stream;

  stream << value;
  return stream.str();
}

}  // namespace RedFS