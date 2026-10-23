#include <RED4ext/RED4ext.hpp>

#include "Config.h"
#include "FileSystem.h"

namespace RedFS {

RED4EXT_C_EXPORT bool RED4EXT_CALL Main(RED4ext::v1::PluginHandle p_handle,
                                        RED4ext::v1::EMainReason p_reason,
                                        const RED4ext::v1::Sdk* p_sdk) {
  switch (p_reason) {
    case RED4ext::v1::EMainReason::Load: {
      Red::TypeInfoRegistrar::RegisterDiscovered();
      FileSystem::load(p_handle, p_sdk->logger);
      break;
    }
    case RED4ext::v1::EMainReason::Unload: {
      FileSystem::unload();
      break;
    }
  }
  return true;
}

RED4EXT_C_EXPORT void RED4EXT_CALL Query(RED4ext::v1::PluginInfo* p_info) {
  p_info->name = L"RedFileSystem";
  p_info->author = L"Rayshader";
  p_info->version = RED4EXT_V1_SEMVER(VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
  p_info->runtime = RED4EXT_V1_RUNTIME_VERSION_LATEST;
  p_info->sdk = RED4EXT_V1_SDK_VERSION_CURRENT;
}

RED4EXT_C_EXPORT uint32_t RED4EXT_CALL Supports() {
  return RED4EXT_API_VERSION_1;
}

}  // namespace RedFS
