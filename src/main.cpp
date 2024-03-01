#include <RED4ext/RED4ext.hpp>

#include "Config.h"
#include "FileSystem.h"

namespace RedFS {

RED4EXT_C_EXPORT bool RED4EXT_CALL Main(RED4ext::PluginHandle p_handle,
                                        RED4ext::EMainReason p_reason,
                                        const RED4ext::Sdk* p_sdk) {
  RED4EXT_UNUSED_PARAMETER(p_handle);
  RED4EXT_UNUSED_PARAMETER(p_sdk);

  switch (p_reason) {
    case RED4ext::EMainReason::Load: {
      Red::TypeInfoRegistrar::RegisterDiscovered();
      FileSystem::load();
      break;
    }
    case RED4ext::EMainReason::Unload: {
      break;
    }
  }
  return true;
}

RED4EXT_C_EXPORT void RED4EXT_CALL Query(RED4ext::PluginInfo* p_info) {
  p_info->name = L"RedFileSystem";
  p_info->author = L"Rayshader";
  p_info->version = RED4EXT_SEMVER(VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
  p_info->runtime = RED4EXT_RUNTIME_INDEPENDENT;
  p_info->sdk = RED4EXT_SDK_LATEST;
}

RED4EXT_C_EXPORT uint32_t RED4EXT_CALL Supports() {
  return RED4EXT_API_VERSION_LATEST;
}

}  // namespace RedFS
