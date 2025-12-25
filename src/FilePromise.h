#ifndef REDFILESYSTEM_FILEPROMISE_H
#define REDFILESYSTEM_FILEPROMISE_H

#include <RED4ext/RED4ext.hpp>
#include <RedLib.hpp>

namespace RedFS {

struct FilePromise {
  Red::WeakHandle<Red::IScriptable> target;
  Red::CName fn_resolve;
  Red::CName fn_reject;
  Red::DynArray<Red::Variant> data;

  template <typename T>
  void resolve(const T& p_buffer) const {
    if (target.Expired()) {
      return;
    }
    if (data.size == 0) {
      Red::CallVirtual(target.Lock(), fn_resolve, p_buffer);
    } else {
      Red::CallVirtual(target.Lock(), fn_resolve, p_buffer, data);
    }
  }

  void resolve() const {
    if (target.Expired()) {
      return;
    }
    if (data.size == 0) {
      Red::CallVirtual(target.Lock(), fn_resolve);
    } else {
      Red::CallVirtual(target.Lock(), fn_resolve, data);
    }
  }

  void reject() const {
    if (target.Expired() || fn_reject.IsNone()) {
      return;
    }
    if (data.size == 0) {
      Red::CallVirtual(target.Lock(), fn_reject);
    } else {
      Red::CallVirtual(target.Lock(), fn_reject, data);
    }
  }
};

}  // namespace RedFS

RTTI_DEFINE_CLASS(RedFS::FilePromise, {
  RTTI_ALIAS("RedFileSystem.FilePromise");

  RTTI_PROPERTY(target);
  RTTI_PROPERTY(fn_resolve, "resolve");
  RTTI_PROPERTY(fn_reject, "reject");
  RTTI_PROPERTY(data);
});

#endif  //REDFILESYSTEM_FILEPROMISE_H
