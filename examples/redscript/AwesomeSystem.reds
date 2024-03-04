// Copy file into r6\scripts\Awesome\
import Codeware.*

public class AwesomeSystem extends ScriptableSystem {
  private let m_storage: wref<FileSystemStorage>;
  private let m_callbackSystem: wref<CallbackSystem>;

  /// Lifecycle ///

  private func OnAttach() {
    this.m_storage = GetAwesomeEnv().GetStorage();
    this.m_callbackSystem = GameInstance.GetCallbackSystem();
    this.m_callbackSystem.RegisterCallback(n"Session/Ready", this, n"OnSessionReady");
  }

  private func OnDetach() {
    this.m_callbackSystem.UnregisterCallback(n"Session/Ready", this, n"OnSessionReady");
    this.m_callbackSystem = null;
    this.m_storage = null;
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
    let file = this.m_storage.GetFile("test.txt");

    if !IsDefined(file) {
      LogChannel(n"Error", "Access to file denied.");
      return;
    }
    let status = file.WriteText("Welcome to Night City!\n");

    if !status {
      LogChannel(n"Error", "Failed to write in file.");
      return;
    }
    let lines = [
      "Let's do this choom ;)",
      "",
      "Beware of flatlines..."
    ];

    status = file.WriteLines(lines, FileSystemWriteMode.Append);
    if !status {
      LogChannel(n"Error", "Failed to write in file.");
      return;
    }
    let lines = file.ReadAsLines();

    LogChannel(n"Info", s"File '\(file.GetFilename())' contains:");
    for line in lines {
      LogChannel(n"Info", line);
    }
  }

  public func Example_Json() {
    LogChannel(n"Info", "");
    LogChannel(n"Info", "== Json ==");
    let file = this.m_storage.GetFile("test.json");

    if !IsDefined(file) {
      LogChannel(n"Error", "Access to JSON file denied.");
      return;
    }
    let json = new JsonObject();

    json.SetKeyString("name", "Awesome");
    json.SetKeyInt64("version", 1);
    json.SetKeyBool("isEnabled", true);
    json.SetKeyNull("data");
    let status = file.WriteJson(json);

    if !status {
      LogChannel(n"Error", "Failed to write in JSON file.");
      return;
    }
    json = file.ReadAsJson() as JsonObject;
    if !IsDefined(json) {
      LogChannel(n"Error", "Cannot parse JSON file.");
      return;
    }
    LogChannel(n"Info", s"File '\(file.GetFilename())' contains:");
    LogChannel(n"Info", json.ToString());
  }

}