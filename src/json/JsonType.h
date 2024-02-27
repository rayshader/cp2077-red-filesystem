#ifndef REDFS_JSONTYPE_H
#define REDFS_JSONTYPE_H

#include <RedLib.hpp>

namespace RedFS {

enum class JsonType {
  Undefined,
  Null,
  Bool,
  Int64,
  Double,
  String,
  Array,
  Object
};

}

RTTI_DEFINE_ENUM(RedFS::JsonType);

#endif  //REDFS_JSONTYPE_H
