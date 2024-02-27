#ifndef REDFS_JSONSTRING_H
#define REDFS_JSONSTRING_H

#include <string>

#include <RED4ext/RED4ext.hpp>
#include <RedLib.hpp>

#include "JsonVariant.h"

namespace RedFS {

class JsonString : public JsonVariant {
 private:
  std::string value;

 public:
  JsonString();
  explicit JsonString(std::string p_value);

  [[nodiscard]] Red::CString get_string() const override;

  RTTI_IMPL_TYPEINFO(RedFS::JsonString);
  RTTI_IMPL_ALLOCATOR();
};

}  // namespace RedFS

RTTI_DEFINE_CLASS(RedFS::JsonString, {
  RTTI_PARENT(RedFS::JsonVariant);

  RTTI_METHOD(get_string, "GetString");
});

#endif  //REDFS_JSONSTRING_H
