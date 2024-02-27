#include "Plugin.h"
#include "FileSystem.h"

#include <RED4ext/RED4ext.hpp>
#include <RedLib.hpp>

namespace RedFS {

Plugin Plugin::instance;

Plugin& Plugin::get_singleton() {
  return instance;
}

Plugin::Plugin() : handle(nullptr), sdk(nullptr) {}

void Plugin::load(RED4ext::PluginHandle p_handle, const RED4ext::Sdk* p_sdk) {
  handle = p_handle;
  sdk = p_sdk;
  Red::TypeInfoRegistrar::RegisterDiscovered();
  FileSystem::load();
}

void Plugin::unload() {}

}  // namespace RedFS