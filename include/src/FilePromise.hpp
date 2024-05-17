#ifndef REDFILESYSTEM_FILEPROMISE_HPP
#define REDFILESYSTEM_FILEPROMISE_HPP

#include <RED4ext/RED4ext.hpp>
#include <RedLib.hpp>

namespace RedFileSystem {

struct FilePromise {
 public:
  FilePromise(const Red::WeakHandle<Red::IScriptable>& target,
              const Red::CName& resolve, const Red::CName& reject = {},
              const Red::DynArray<Red::Variant>& data = {})
      : target(target), fn_resolve(resolve), fn_reject(reject), data(data) {}

  Red::WeakHandle<Red::IScriptable> target;
  Red::CName fn_resolve;
  Red::CName fn_reject;
  Red::DynArray<Red::Variant> data;
};

}  // namespace RedFileSystem

RTTI_MAP_TYPE_NAME(RedFileSystem::FilePromise, "RedFileSystem.FilePromise")

#endif  //REDFILESYSTEM_FILEPROMISE_HPP
