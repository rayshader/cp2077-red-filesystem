#ifndef REDFS_JSONARRAY_H
#define REDFS_JSONARRAY_H

#include <RED4ext/RED4ext.hpp>
#include <RedLib.hpp>

#include "JsonVariant.h"

namespace RedFS {

class JsonArray : public JsonVariant {
 private:
  Red::DynArray<Red::Handle<JsonVariant>> items;

 public:
  JsonArray();

  static std::string to_json(const JsonArray* p_array,
                             const std::string& p_indent = "");

  [[nodiscard]] uint32_t get_size() const;
  [[nodiscard]] Red::Handle<JsonVariant> get_item(uint32_t p_index) const;

  [[nodiscard]] bool get_item_bool(uint32_t p_index) const;
  [[nodiscard]] int64_t get_item_int64(uint32_t p_index) const;
  [[nodiscard]] double get_item_double(uint32_t p_index) const;
  [[nodiscard]] Red::CString get_item_string(uint32_t p_index) const;

  [[nodiscard]] Red::CString to_string() const override;

  void push_back(const Red::Handle<JsonVariant>& p_item);

  RTTI_IMPL_TYPEINFO(RedFS::JsonArray);
  RTTI_IMPL_ALLOCATOR();
};

}  // namespace RedFS

RTTI_DEFINE_CLASS(RedFS::JsonArray, {
  RTTI_PARENT(RedFS::JsonVariant);

  RTTI_METHOD(get_size, "GetSize");
  RTTI_METHOD(get_item, "GetItem");

  RTTI_METHOD(get_item_bool, "GetItemBool");
  RTTI_METHOD(get_item_int64, "GetItemInt64");
  RTTI_METHOD(get_item_double, "GetItemDouble");
  RTTI_METHOD(get_item_string, "GetItemString");

  RTTI_METHOD(to_string, "ToString");
});

#endif  //REDFS_JSONARRAY_H
