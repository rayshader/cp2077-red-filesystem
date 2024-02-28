#ifndef REDFS_JSONFACTORY_H
#define REDFS_JSONFACTORY_H

#include <simdjson.h>
#include <RED4ext/RED4ext.hpp>
#include <RedLib.hpp>

#include "JsonArray.h"
#include "JsonBool.h"
#include "JsonDouble.h"
#include "JsonInt64.h"
#include "JsonNull.h"
#include "JsonObject.h"
#include "JsonString.h"
#include "JsonVariant.h"

namespace RedFS {

class JsonFactory {
 public:
  inline static Red::Handle<JsonUndefined> CreateUndefined() {
    return Red::MakeHandle<JsonUndefined>();
  }

  inline static Red::Handle<JsonNull> CreateNull() {
    return Red::MakeHandle<JsonNull>();
  }

  inline static Red::Handle<JsonBool> CreateBool(
    const simdjson::dom::element& p_value) {
    return Create<bool, JsonBool>(p_value);
  }

  inline static Red::Handle<JsonInt64> CreateInt64(
    const simdjson::dom::element& p_value) {
    return Create<int64_t, JsonInt64>(p_value);
  }

  inline static Red::Handle<JsonDouble> CreateDouble(
    const simdjson::dom::element& p_value) {
    return Create<double, JsonDouble>(p_value);
  }

  inline static Red::Handle<JsonString> CreateString(
    const simdjson::dom::element& p_value) {
    return Create<std::string, JsonString>(p_value);
  }

  inline static Red::Handle<JsonArray> CreateArray() {
    return Red::MakeHandle<JsonArray>();
  }

  inline static Red::Handle<JsonObject> CreateObject() {
    return Red::MakeHandle<JsonObject>();
  }

  template <typename T, class U>
  inline static Red::Handle<U> Create(const simdjson::dom::element& p_value) {
    return Red::MakeHandle<U>(T(p_value));
  }
};

}  // namespace RedFS

#endif  //REDFS_JSONFACTORY_H
