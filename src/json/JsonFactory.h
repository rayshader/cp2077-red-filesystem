#ifndef REDFS_JSONFACTORY_H
#define REDFS_JSONFACTORY_H

#include <simdjson.h>
#include <RED4ext/RED4ext.hpp>
#include <RedLib.hpp>

#include "JsonVariant.h"
#include "JsonNull.h"
#include "JsonArray.h"
#include "JsonObject.h"

namespace RedFS {

class JsonFactory {
 public:
  static Red::Handle<JsonUndefined> CreateUndefined() {
    return Red::MakeHandle<JsonUndefined>();
  }

  static Red::Handle<JsonNull> CreateNull() {
    return Red::MakeHandle<JsonNull>();
  }

  static Red::Handle<JsonArray> CreateArray() {
    return Red::MakeHandle<JsonArray>();
  }

  static Red::Handle<JsonObject> CreateObject() {
    return Red::MakeHandle<JsonObject>();
  }

  template<typename T, class U>
  static Red::Handle<U> Create(const simdjson::dom::element& p_value) {
    return Red::MakeHandle<U>(T(p_value));
  }
};

}

#endif  //REDFS_JSONFACTORY_H
