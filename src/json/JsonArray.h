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
  inline void set_item(uint32_t p_index,
                       const Red::Handle<JsonVariant>& p_item);
  [[nodiscard]] bool remove_item(uint32_t p_index);
  void add_item(const Red::Handle<JsonVariant>& p_item);
  inline void insert_item(uint32_t p_index,
                          const Red::Handle<JsonVariant>& p_item);

  [[nodiscard]] bool get_item_bool(uint32_t p_index) const;
  [[nodiscard]] int64_t get_item_int64(uint32_t p_index) const;
  [[nodiscard]] double get_item_double(uint32_t p_index) const;
  [[nodiscard]] Red::CString get_item_string(uint32_t p_index) const;

  void set_item_null(uint32_t p_index);
  void set_item_bool(uint32_t p_index, bool p_value);
  void set_item_int64(uint32_t p_index, int64_t p_value);
  void set_item_double(uint32_t p_index, double p_value);
  void set_item_string(uint32_t p_index, const Red::CString& p_value);

  void add_item_null();
  void add_item_bool(bool p_item);
  void add_item_int64(int64_t p_item);
  void add_item_double(double p_item);
  void add_item_string(const Red::CString& p_item);

  void insert_item_null(uint32_t p_index);
  void insert_item_bool(uint32_t p_index, bool p_item);
  void insert_item_int64(uint32_t p_index, int64_t p_item);
  void insert_item_double(uint32_t p_index, double p_item);
  void insert_item_string(uint32_t p_index, const Red::CString& p_item);

  void clear();

  [[nodiscard]] Red::CString to_string() const override;

  RTTI_IMPL_TYPEINFO(RedFS::JsonArray);
  RTTI_IMPL_ALLOCATOR();
};

}  // namespace RedFS

RTTI_DEFINE_CLASS(RedFS::JsonArray, {
  RTTI_ALIAS("RedFileSystem.Json.JsonArray");

  RTTI_PARENT(RedFS::JsonVariant);

  RTTI_METHOD(get_size, "GetSize");
  RTTI_METHOD(get_item, "GetItem");
  RTTI_METHOD(set_item, "SetItem");
  RTTI_METHOD(remove_item, "RemoveItem");
  RTTI_METHOD(add_item, "AddItem");
  RTTI_METHOD(insert_item, "InsertItem");

  RTTI_METHOD(get_item_bool, "GetItemBool");
  RTTI_METHOD(get_item_int64, "GetItemInt64");
  RTTI_METHOD(get_item_double, "GetItemDouble");
  RTTI_METHOD(get_item_string, "GetItemString");

  RTTI_METHOD(set_item_null, "SetItemNull");
  RTTI_METHOD(set_item_bool, "SetItemBool");
  RTTI_METHOD(set_item_int64, "SetItemInt64");
  RTTI_METHOD(set_item_double, "SetItemDouble");
  RTTI_METHOD(set_item_string, "SetItemString");

  RTTI_METHOD(add_item_null, "AddItemNull");
  RTTI_METHOD(add_item_bool, "AddItemBool");
  RTTI_METHOD(add_item_int64, "AddItemInt64");
  RTTI_METHOD(add_item_double, "AddItemDouble");
  RTTI_METHOD(add_item_string, "AddItemString");

  RTTI_METHOD(insert_item_null, "InsertItemNull");
  RTTI_METHOD(insert_item_bool, "InsertItemBool");
  RTTI_METHOD(insert_item_int64, "InsertItemInt64");
  RTTI_METHOD(insert_item_double, "InsertItemDouble");
  RTTI_METHOD(insert_item_string, "InsertItemString");

  RTTI_METHOD(clear, "Clear");

  RTTI_METHOD(to_string, "ToString");
});

#endif  //REDFS_JSONARRAY_H
