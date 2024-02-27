#ifndef REDFS_PLUGIN_H
#define REDFS_PLUGIN_H

#include <RED4ext/RED4ext.hpp>
#include <RED4ext/Scripting/Natives/Generated/ent/IComponent.hpp>
#include <RED4ext/Scripting/Natives/Generated/game/Object.hpp>
#include <RedLib.hpp>

namespace RedFS {

class Plugin {
 private:
  static Plugin instance;

  RED4ext::PluginHandle handle;
  const RED4ext::Sdk* sdk;

 public:
  static Plugin& get_singleton();

  Plugin();

  void load(RED4ext::PluginHandle p_handle, const RED4ext::Sdk* p_sdk);
  void unload();
};

}  // namespace RedFS

#endif  //REDFS_PLUGIN_H
