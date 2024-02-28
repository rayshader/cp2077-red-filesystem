import Codeware.*
import RedFileSystem.*
import RedFileSystem.Enums.*
import RedFileSystem.Json.*

public class FileSystemExample extends ScriptableSystem {
  private let m_callbackSystem: wref<CallbackSystem>;

  /// Lifecycle ///

  private func OnAttach() {
    this.m_callbackSystem = GameInstance.GetCallbackSystem();
    this.m_callbackSystem.RegisterCallback(n"Session/Ready", this, n"OnSessionReady");
  }

  private func OnDetach() {
    this.m_callbackSystem.UnregisterCallback(n"Session/Ready", this, n"OnSessionReady");
    this.m_callbackSystem = null;
  }

  /// Game events ///

  private cb func OnSessionReady(event: ref<GameSessionEvent>) {
    let isPreGame = event.IsPreGame();

    if !isPreGame {
      return;
    }
    LogChannel(n"Info", s"== RedFS - Examples ==");
    this.Example_Text();
    this.Example_Json();
    LogChannel(n"Info", s"== RedFS - Examples ==");
  }

  /// Examples ///

  public func Example_Text() {
    LogChannel(n"Info", "");
    LogChannel(n"Info", "== Text ==");
    let path = "engine\\config\\base\\multiplayer.ini";
    let status = FileSystem.Exists(path);

    if !Equals(status, FileSystemStatus.True) {
      LogChannel(n"Error", "Cannot find INI file.");
      return;
    }
    let file = FileSystem.GetFile(path);

    if !IsDefined(file) {
      LogChannel(n"Error", "Access to INI file denied.");
      return;
    }
    let lines = file.ReadAsLines();

    LogChannel(n"Info", s"INI file '\(file.GetFilename())':");
    for line in lines {
      LogChannel(n"Info", line);
    }
  }

  public func Example_Json() {
    LogChannel(n"Info", "");
    LogChannel(n"Info", "== Json ==");
    let path = "r6\\config\\settings\\platform\\pc\\options.json";
    let status = FileSystem.Exists(path);

    if !Equals(status, FileSystemStatus.True) {
      LogChannel(n"Error", "Cannot find JSON file.");
      return;
    }
    let file = FileSystem.GetFile(path);

    if !IsDefined(file) {
      LogChannel(n"Error", "Access to JSON file denied.");
      return;
    }
    let json = file.ReadAsJson();

    if !IsDefined(json) {
      LogChannel(n"Error", "Cannot parse JSON file.");
      return;
    }
    let root = json as JsonObject;

    if !root.HasKey("version") || !root.HasKey("groups") || !root.HasKey("options") {
      LogChannel(n"Error", "JSON schema is unknown.");
      return;
    }
    let options = root.GetKey("options") as JsonArray;
    let names: array<String> = [];
    let i = 0u;

    while i < options.GetSize() {
      let option = options.GetItem(i) as JsonObject;
      let name = option.GetKeyString("group_name");

      ArrayPush(names, name);
      i += 1u;
    }
    LogChannel(n"Info", s"List of groups in game options:");
    for name in names {
      LogChannel(n"Info", name);
    }
  }

}