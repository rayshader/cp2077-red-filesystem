#ifndef REDFS_JSONNULL_H
#define REDFS_JSONNULL_H

#include <RED4ext/RED4ext.hpp>
#include <RedLib.hpp>

#include "JsonVariant.h"

namespace RedFS {

class JsonNull : public JsonVariant {
 public:
  JsonNull();

  RTTI_IMPL_TYPEINFO(RedFS::JsonNull);
  RTTI_IMPL_ALLOCATOR();
};

}  // namespace RedFS

RTTI_DEFINE_CLASS(RedFS::JsonNull, {
  RTTI_PARENT(RedFS::JsonVariant);
});

#endif  //REDFS_JSONNULL_H
