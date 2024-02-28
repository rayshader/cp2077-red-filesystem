#ifndef REDFS_JSONDOUBLE_H
#define REDFS_JSONDOUBLE_H

#include <cstdint>

#include <RED4ext/RED4ext.hpp>
#include <RedLib.hpp>

#include "JsonVariant.h"

namespace RedFS {

class JsonDouble : public JsonVariant {
 private:
  double value;

 public:
  JsonDouble();
  explicit JsonDouble(double p_value);

  [[nodiscard]] double get_double() const override;

  [[nodiscard]] Red::CString to_string() const override;

  RTTI_IMPL_TYPEINFO(RedFS::JsonDouble);
  RTTI_IMPL_ALLOCATOR();
};

}  // namespace RedFS

RTTI_DEFINE_CLASS(RedFS::JsonDouble, {
  RTTI_ALIAS("RedFileSystem.Json.JsonDouble");

  RTTI_PARENT(RedFS::JsonVariant);

  RTTI_METHOD(get_double, "GetDouble");

  RTTI_METHOD(to_string, "ToString");
});

#endif  //REDFS_JSONDOUBLE_H
