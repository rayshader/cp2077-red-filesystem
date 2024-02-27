#include "JsonDouble.h"

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

}  // namespace RedFS