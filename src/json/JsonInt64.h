#ifndef REDFS_JSONINT64_H
#define REDFS_JSONINT64_H

#include <cstdint>

#include <RED4ext/RED4ext.hpp>
#include <RedLib.hpp>

#include "JsonVariant.h"

namespace RedFS {

class JsonInt64 : public JsonVariant {
 private:
  int64_t value;

 public:
  JsonInt64();
  explicit JsonInt64(int64_t p_value);

  [[nodiscard]] int64_t get_int64() const override;

  [[nodiscard]] Red::CString to_string() const override;

  RTTI_IMPL_TYPEINFO(RedFS::JsonInt64);
  RTTI_IMPL_ALLOCATOR();
};

}  // namespace RedFS

RTTI_DEFINE_CLASS(RedFS::JsonInt64, {
  RTTI_PARENT(RedFS::JsonVariant);

  RTTI_METHOD(get_int64, "GetInt64");

  RTTI_METHOD(to_string, "ToString");
});

#endif  //REDFS_JSONINT64_H
