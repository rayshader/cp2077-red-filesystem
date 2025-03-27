// Copy file into r6\scripts\Awesome\
import Codeware.*

public class AwesomeSystem extends ScriptableSystem {
  private let m_storage: wref<FileSystemStorage>;
  private let m_callbackSystem: wref<CallbackSystem>;

  /// Lifecycle ///

  private func OnAttach() {
    this.m_storage = GetAwesomeService().GetStorage();
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
    FTLog(s"== RedFS - Examples ==");
    this.Example_Text();
    this.Example_Json();
    FTLog(s"== RedFS - Examples ==");
  }

  /// Examples ///

  public func Example_Text() {
    FTLog("");
    FTLog("== Text ==");
    let file = this.m_storage.GetFile("test.txt");

    if !IsDefined(file) {
      FTLogError("Access to file denied.");
      return;
    }
    let status = file.WriteText("Welcome to Night City!\n");

    if !status {
      FTLogError("Failed to write in file.");
      return;
    }
    let lines = [
      "Let's do this choom ;)",
      "",
      "Beware of flatlines..."
    ];

    status = file.WriteLines(lines, FileSystemWriteMode.Append);
    if !status {
      FTLogError("Failed to write in file.");
      return;
    }
    let lines = file.ReadAsLines();

    FTLog(s"File '\(file.GetFilename())' contains:");
    for line in lines {
      FTLog(line);
    }
  }

  public func Example_Json() {
    FTLog("");
    FTLog("== Json ==");
    let file = this.m_storage.GetFile("test.json");

    if !IsDefined(file) {
      FTLogError("Access to JSON file denied.");
      return;
    }
    let json = new JsonObject();

    json.SetKeyString("name", "Awesome");
    json.SetKeyInt64("version", 1);
    json.SetKeyBool("isEnabled", true);
    json.SetKeyNull("data");
    let status = file.WriteJson(json);

    if !status {
      FTLogError("Failed to write in JSON file.");
      return;
    }
    json = file.ReadAsJson() as JsonObject;
    if !IsDefined(json) {
      FTLogError("Cannot parse JSON file.");
      return;
    }
    FTLog(s"File '\(file.GetFilename())' contains:");
    FTLog(json.ToString());
  }

}