#ifndef REDFS_JSONBOOL_H
#define REDFS_JSONBOOL_H

#include <RED4ext/RED4ext.hpp>
#include <RedLib.hpp>

#include "JsonVariant.h"

namespace RedFS {

class JsonBool : public JsonVariant {
 private:
  bool value;

 public:
  JsonBool();
  explicit JsonBool(bool p_value);

  [[nodiscard]] bool get_bool() const override;

  RTTI_IMPL_TYPEINFO(RedFS::JsonBool);
  RTTI_IMPL_ALLOCATOR();
};

}  // namespace RedFS

RTTI_DEFINE_CLASS(RedFS::JsonBool, {
  RTTI_PARENT(RedFS::JsonVariant);

  RTTI_METHOD(get_bool, "GetBool");
});

#endif  //REDFS_JSONBOOL_H
