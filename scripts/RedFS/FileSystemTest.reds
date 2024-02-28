/// DO NOT RELEASE THIS FILE ///
import Codeware.*

class FileSystemTest extends ScriptableSystem {
  private let m_callbackSystem: wref<CallbackSystem>;

  private let m_isLoaded: Bool;
  private let m_failCount: Int32;
  private let m_passCount: Int32;
  private let m_totalCount: Int32;
  
  /// Lifecycle ///

  private func OnAttach() {
    this.m_isLoaded = false;
    this.m_failCount = 0;
    this.m_passCount = 0;
    this.m_totalCount = 0;
    this.m_callbackSystem = GameInstance.GetCallbackSystem();
    this.m_callbackSystem.RegisterCallback(n"Session/Ready", this, n"OnSessionReady");
  }

  private func OnDetach() {
    this.m_isLoaded = false;
    this.m_callbackSystem.UnregisterCallback(n"Session/Ready", this, n"OnSessionReady");
    this.m_callbackSystem = null;
  }

  /// Game events ///

  private cb func OnSessionReady(event: ref<GameSessionEvent>) {
    let isPreGame = event.IsPreGame();

    if !isPreGame || this.m_isLoaded {
      return;
    }
    LogChannel(n"Info", s"== RedFS - Tests ==");
    let scriptPath = "RedFS\\";

    this.Test_RootPathDenied();
    this.Test_RootRelativePathDenied();

    this.Test_PrefixNone();
    this.Test_PrefixCET();
    this.Test_PrefixRedscript();

    this.Test_ReadText(scriptPath, "test.txt");
    this.Test_ReadLines(scriptPath, "test.txt");
    this.Test_ReadNotFoundText(scriptPath, "unknown.txt");
    this.Test_ReadJson(scriptPath, "test.json");
    this.Test_ReadInvalidJson(scriptPath, "test.txt");

    this.Test_WriteTextTruncate(scriptPath, "test-write.txt");
    this.Test_WriteTextAppend(scriptPath, "test-write.txt");
    this.Test_WriteLinesTruncate(scriptPath, "test-write-lines.txt");
    this.Test_WriteLinesAppend(scriptPath, "test-write-lines.txt");
    LogChannel(n"Info", "");
    LogChannel(n"Info", s"Tests: \(this.m_failCount) failed, \(this.m_passCount) passed, \(this.m_totalCount) total");
    LogChannel(n"Info", s"Time:  ? s");
    LogChannel(n"Info", "");
    LogChannel(n"Info", s"== RedFS - Tests ==");
    this.m_isLoaded = true;
  }

  /// Tests ///

  private func Test_RootPathDenied() {
    LogChannel(n"Info", "");
    LogChannel(n"Info", s"== RootPathDenied ==");
    let path = "C:\\Fake\\file.txt";

    LogChannel(n"Info", s"Absolute path: '\(path)'");
    let status = FileSystem.Exists(path);

    this.ExpectString("Exists call denied", s"\(status)", "Denied");
    status = FileSystem.IsFile(path);

    this.ExpectString("IsFile call denied", s"\(status)", "Denied");
    let file = FileSystem.GetFile(path);

    this.ExpectBool("GetFile call return undefined", IsDefined(file), false);
  }

  private func Test_RootRelativePathDenied() {
    LogChannel(n"Info", "");
    LogChannel(n"Info", s"== RootRelativePathDenied ==");
    // Should resolve to "C:\passwd.txt" with Steam default installation.
    let path = "..\\..\\..\\..\\..\\denied.txt";

    LogChannel(n"Info", s"Relative path: '\(path)'");
    let status = FileSystem.Exists(path);

    this.ExpectString("Exists call denied", s"\(status)", "Denied");
    status = FileSystem.IsFile(path);

    this.ExpectString("IsFile call denied", s"\(status)", "Denied");
    let file = FileSystem.GetFile(path);

    this.ExpectBool("GetFile call return undefined", IsDefined(file), false);
  }

  private func Test_PrefixNone() {
    LogChannel(n"Info", "");
    LogChannel(n"Info", s"== PrefixNone ==");
    let path = "launcher-configuration.json";

    LogChannel(n"Info", s"Path: '\(path)'");
    let status = FileSystem.Exists(path, FileSystemPrefix.None);

    this.ExpectString("file exists", s"\(status)", "True");
    status = FileSystem.IsFile(path);

    this.ExpectString("is a file", s"\(status)", "True");
    let file = FileSystem.GetFile(path);

    this.ExpectBool("GetFile is defined", IsDefined(file), true);
  }

  private func Test_PrefixCET() {
    LogChannel(n"Info", "");
    LogChannel(n"Info", s"== PrefixCET ==");
    let path = "TestMod\\test.txt";

    LogChannel(n"Info", s"Path: '\(path)'");
    let status = FileSystem.Exists(path, FileSystemPrefix.CET);

    this.ExpectString("file doesn't exist", s"\(status)", "False");
    let file = FileSystem.GetFile(path);

    this.ExpectBool("GetFile is defined", IsDefined(file), true);
  }

  private func Test_PrefixRedscript() {
    LogChannel(n"Info", "");
    LogChannel(n"Info", s"== PrefixRedscript ==");
    let path = "TestMod\\test.reds";

    LogChannel(n"Info", s"Path: '\(path)'");
    let status = FileSystem.Exists(path, FileSystemPrefix.Redscript);

    this.ExpectString("file doesn't exist", s"\(status)", "False");
    let file = FileSystem.GetFile(path);

    this.ExpectBool("GetFile is defined", IsDefined(file), true);
  }

  private func Test_ReadText(scriptPath: String, fileName: String) {
    LogChannel(n"Info", "");
    LogChannel(n"Info", s"== ReadText ==");
    let path = s"\(scriptPath)\(fileName)";
    let status = FileSystem.Exists(path, FileSystemPrefix.Redscript);

    if !Equals(status, FileSystemStatus.True) {
      LogChannel(n"Error", s"FAIL: text file '\(fileName)' not found!");
      this.m_failCount += 1;
      this.m_totalCount += 1;
      return;
    }
    let file = FileSystem.GetFile(path, FileSystemPrefix.Redscript);

    this.ExpectFileStat(file, fileName, ".txt", 878ul);
    let expectedText = "صِف خَلقَ خَودِ كَمِثلِ الشَمسِ إِذ بَزَغَت — يَحظى الضَجيعُ بِها نَجلاءَ مِعطارِ (A poem by Al Farāhīdi)\n" +
      "ऋषियों को सताने वाले दुष्ट राक्षसों के राजा रावण का सर्वनाश करने वाले विष्णुवतार भगवान श्रीराम, अयोध्या के महाराज दशरथ के बड़े सपुत्र थे।\n" +
      "いろはにほへと ちりぬるを わかよたれそ つねならむ うゐのおくやま けふこえて あさきゆめみし ゑひもせす（ん）\n" +
      "Sic fugiens, dux, zelotypos, quam Karus haberis.\n" +
      "\n" +
      "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.";
    let text = file.ReadAsText();
    let textSize = StrLen(text);
    let expectedSize = StrLen(expectedText);

    if textSize != expectedSize {
      this.LogFail("read text", s"\(textSize)", s"\(expectedSize)");
    }
    if !UnicodeStringEqual(text, expectedText) {
      this.LogFail(s"read text", text, expectedText);
    } else {
      this.LogPass(s"read text: \(text)");
    }
  }

  private func Test_ReadLines(scriptPath: String, fileName: String) {
    LogChannel(n"Info", "");
    LogChannel(n"Info", s"== ReadLines ==");
    let path = s"\(scriptPath)\(fileName)";
    let status = FileSystem.Exists(path, FileSystemPrefix.Redscript);

    if !Equals(status, FileSystemStatus.True) {
      LogChannel(n"Error", s"FAIL: text file '\(fileName)' not found!");
      this.m_failCount += 1;
      this.m_totalCount += 1;
      return;
    }
    let file = FileSystem.GetFile(path, FileSystemPrefix.Redscript);

    this.ExpectFileStat(file, fileName, ".txt", 878ul);
    let expectedLines = [
      "صِف خَلقَ خَودِ كَمِثلِ الشَمسِ إِذ بَزَغَت — يَحظى الضَجيعُ بِها نَجلاءَ مِعطارِ (A poem by Al Farāhīdi)",
      "ऋषियों को सताने वाले दुष्ट राक्षसों के राजा रावण का सर्वनाश करने वाले विष्णुवतार भगवान श्रीराम, अयोध्या के महाराज दशरथ के बड़े सपुत्र थे।",
      "いろはにほへと ちりぬるを わかよたれそ つねならむ うゐのおくやま けふこえて あさきゆめみし ゑひもせす（ん）",
      "Sic fugiens, dux, zelotypos, quam Karus haberis.",
      "",
      "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua."
    ];
    let lines = file.ReadAsLines();
    let linesSize = ArraySize(lines);
    let expectedSize = ArraySize(expectedLines);

    if linesSize != expectedSize {
      this.LogFail("read lines", s"\(linesSize)", s"\(expectedSize)");
      return;
    }
    let i = 0;

    while i < linesSize {
      if !Equals(lines[i], expectedLines[i]) {
        this.LogFail(s"line \(i)", lines[i], expectedLines[i]);
      } else {
        this.LogPass(s"line \(i): \(expectedLines[i])");
      }
      i += 1;
    }
  }

  private func Test_ReadNotFoundText(scriptPath: String, fileName: String) {
    LogChannel(n"Info", "");
    LogChannel(n"Info", s"== ReadNotFoundText ==");
    let path = s"\(scriptPath)\(fileName)";
    let status = FileSystem.Exists(path, FileSystemPrefix.Redscript);

    this.ExpectString(s"text file '\(fileName)' not found", s"\(status)", "False");
  }

  private func Test_ReadJson(scriptPath: String, fileName: String) {
    LogChannel(n"Info", "");
    LogChannel(n"Info", s"== ReadJson ==");
    let path = s"\(scriptPath)\(fileName)";
    let status = FileSystem.Exists(path, FileSystemPrefix.Redscript);

    if !Equals(status, FileSystemStatus.True) {
      LogChannel(n"Error", s"FAIL: json file '\(fileName)' not found!");
      this.m_failCount += 1;
      this.m_totalCount += 1;
      return;
    }
    let file = FileSystem.GetFile(path, FileSystemPrefix.Redscript);

    this.ExpectFileStat(file, fileName, ".json", 1102ul);
    let json = file.ReadAsJson();

    if !json.IsObject() {
      LogChannel(n"Error", s"FAIL: root of json document is not an object!");
      this.m_failCount += 1;
      this.m_totalCount += 1;
      return;
    }
    LogChannel(n"Info", "");
    LogChannel(n"Info", json.ToString());
    LogChannel(n"Info", "");
    let root = json as JsonObject;

    this.ExpectJsonKeyString(root, "name", "FileSystemTest");
    let authors = root.GetKey("authors") as JsonArray;

    if !authors.IsArray() {
      this.LogFail("obj['authors']", "?", "[...]");
    } else if authors.GetSize() != 1u {
      this.ExpectSize("obj['authors'].size", authors.GetSize(), 1u);
    } else {
      this.ExpectString("obj['authors'][0]", authors.GetItemString(0u), "Rayshader");
    }
    this.ExpectJsonKeyInt64(root, "version", 1l);
    this.ExpectJsonKeyBool(root, "isJson", true);
    this.ExpectJsonKeyDouble(root, "delta", 0.016);
    let pangrams = root.GetKey("pangrams") as JsonObject;

    this.ExpectJsonKeyString(pangrams, "Arabic", "صِف خَلقَ خَودِ كَمِثلِ الشَمسِ إِذ بَزَغَت — يَحظى الضَجيعُ بِها نَجلاءَ مِعطارِ (A poem by Al Farāhīdi)");
    this.ExpectJsonKeyString(pangrams, "Hindi", "ऋषियों को सताने वाले दुष्ट राक्षसों के राजा रावण का सर्वनाश करने वाले विष्णुवतार भगवान श्रीराम, अयोध्या के महाराज दशरथ के बड़े सपुत्र थे।");
    this.ExpectJsonKeyString(pangrams, "Japanese", "いろはにほへと ちりぬるを わかよたれそ つねならむ うゐのおくやま けふこえて あさきゆめみし ゑひもせす（ん）");
    this.ExpectJsonKeyString(pangrams, "Latin", "Sic fugiens, dux, zelotypos, quam Karus haberis.");
    let random = root.GetKey("random") as JsonArray;

    LogChannel(n"Info", "");
    LogChannel(n"Info", random.ToString());
    LogChannel(n"Info", "");
    if !random.IsArray() {
      this.LogFail("obj['random']", "?", "[...]");
    } else if random.GetSize() != 6u {
      this.ExpectSize("obj['random'].size", random.GetSize(), 6u);
    } else {
      this.ExpectBool("obj['random'][0]", random.GetItemBool(0u), true);
      this.ExpectInt64("obj['random'][1]", random.GetItemInt64(1u), 42l);
      this.ExpectDouble("obj['random'][2]", random.GetItemDouble(2u), 13.37);
      this.ExpectString("obj['random'][3]", random.GetItemString(3u), "Weird");
      let object = random.GetItem(4u) as JsonObject;

      if !object.IsObject() {
        this.LogFail("obj['random'][4]", "?", "{...}");
      } else {
        this.ExpectJsonKeyNull(object, "object");
      }
      let array = random.GetItem(5u) as JsonArray;

      if !array.IsArray() {
        this.LogFail("obj['random'][5]", "?", "[...]");
      } else {
        this.ExpectString("obj['random'][5][0]", array.GetItemString(0u), "array");
      }
    }
  }

  private func Test_ReadInvalidJson(scriptPath: String, fileName: String) {
    LogChannel(n"Info", "");
    LogChannel(n"Info", s"== ReadInvalidJson ==");
    let path = s"\(scriptPath)\(fileName)";
    let status = FileSystem.Exists(path, FileSystemPrefix.Redscript);

    if !Equals(status, FileSystemStatus.True) {
      LogChannel(n"Error", s"FAIL: json file '\(fileName)' not found!");
      this.m_failCount += 1;
      this.m_totalCount += 1;
      return;
    }
    let file = FileSystem.GetFile(path, FileSystemPrefix.Redscript);

    this.ExpectFileStat(file, fileName, ".txt", 878ul);
    let json = file.ReadAsJson();

    this.ExpectBool(s"invalid json file '\(fileName)'", IsDefined(json), false);
  }

  private func Test_WriteTextTruncate(scriptPath: String, fileName: String) {
    LogChannel(n"Info", "");
    LogChannel(n"Info", s"== WriteTextTruncate ==");
    let path = s"\(scriptPath)\(fileName)";

    LogChannel(n"Info", s"Relative path: '\(path)'");
    let file = FileSystem.GetFile(path, FileSystemPrefix.Redscript);
    let status = FileSystem.Exists(path, FileSystemPrefix.Redscript);

    if Equals(status, FileSystemStatus.True) {
      this.ExpectFileStat(file, fileName, ".txt", 0ul);
    }
    let text = "";

    text += "Hello world!\n";
    text += "Hola el mundo!\n";
    text += "Bonjour le monde!";
    let status = file.WriteText(text);

    if !status {
      this.LogFail("write text (truncate mode)", s"\(status)", "true");
    } else {
      this.LogPass("write text (truncate mode)");
    }
  }

  private func Test_WriteTextAppend(scriptPath: String, fileName: String) {
    LogChannel(n"Info", "");
    LogChannel(n"Info", s"== WriteTextAppend ==");
    let path = s"\(scriptPath)\(fileName)";
    let file = FileSystem.GetFile(path, FileSystemPrefix.Redscript);
    let status = FileSystem.Exists(path, FileSystemPrefix.Redscript);

    if Equals(status, FileSystemStatus.True) {
      this.ExpectFileStat(file, fileName, ".txt", 47ul);
    }
    let text = "";

    text += "\n\n";
    text += "صِف خَلقَ خَودِ كَمِثلِ الشَمسِ إِذ بَزَغَت — يَحظى الضَجيعُ بِها نَجلاءَ مِعطارِ (A poem by Al Farāhīdi)\n";
    text += "いろはにほへと ちりぬるを わかよたれそ つねならむ うゐのおくやま けふこえて あさきゆめみし ゑひもせす（ん）";
    let status = file.WriteText(text, FileSystemWriteMode.Append);

    if !status {
      this.LogFail("write text (append mode)", s"\(status)", "true");
    } else {
      this.LogPass("write text (append mode)");
    }
  }

  private func Test_WriteLinesTruncate(scriptPath: String, fileName: String) {
    LogChannel(n"Info", "");
    LogChannel(n"Info", s"== WriteLinesTruncate ==");
    let path = s"\(scriptPath)\(fileName)";
    let file = FileSystem.GetFile(path, FileSystemPrefix.Redscript);
    let status = FileSystem.Exists(path, FileSystemPrefix.Redscript);

    if Equals(status, FileSystemStatus.True) {
      this.ExpectFileStat(file, fileName, ".txt", 0ul);
    }
    let lines = [
      "Hello world!",
      "Hola el mundo!",
      "Bonjour le monde!"
    ];
    let status = file.WriteLines(lines);

    if !status {
      this.LogFail("write lines (truncate mode)", s"\(status)", "true");
    } else {
      this.LogPass("write lines (truncate mode)");
    }
  }

  private func Test_WriteLinesAppend(scriptPath: String, fileName: String) {
    LogChannel(n"Info", "");
    LogChannel(n"Info", s"== WriteLinesAppend ==");
    let path = s"\(scriptPath)\(fileName)";
    let file = FileSystem.GetFile(path, FileSystemPrefix.Redscript);
    let status = FileSystem.Exists(path, FileSystemPrefix.Redscript);

    if Equals(status, FileSystemStatus.True) {
      this.ExpectFileStat(file, fileName, ".txt", 47ul);
    }
    let lines = [
      "",
      "",
      "صِف خَلقَ خَودِ كَمِثلِ الشَمسِ إِذ بَزَغَت — يَحظى الضَجيعُ بِها نَجلاءَ مِعطارِ (A poem by Al Farāhīdi)",
      "いろはにほへと ちりぬるを わかよたれそ つねならむ うゐのおくやま けふこえて あさきゆめみし ゑひもせす（ん）"
    ];
    let status = file.WriteLines(lines, FileSystemWriteMode.Append);

    if !status {
      this.LogFail("write lines (append mode)", s"\(status)", "true");
    } else {
      this.LogPass("write lines (append mode)");
    }
  }

  /// Helpers ///

  private func ExpectBool(from: String, actual: Bool, expected: Bool) {
    if !Equals(actual, expected) {
      this.LogFail(from, s"\(actual)", s"\(expected)");
    } else {
      this.LogPass(from);
    }
  }

  private func ExpectInt64(from: String, actual: Int64, expected: Int64) {
    if !Equals(actual, expected) {
      this.LogFail(from, s"\(actual)", s"\(expected)");
    } else {
      this.LogPass(from);
    }
  }

  private func ExpectUint64(from: String, actual: Uint64, expected: Uint64) {
    if !Equals(actual, expected) {
      this.LogFail(from, s"\(actual)", s"\(expected)");
    } else {
      this.LogPass(from);
    }
  }

  private func ExpectDouble(from: String, actual: Double, expected: Double) {
    // Use formatted string to compare, raw type comparison is not precise.
    if !Equals(s"\(actual)", s"\(expected)") {
      this.LogFail(from, s"\(actual)", s"\(expected)");
    } else {
      this.LogPass(from);
    }
  }

  private func ExpectString(from: String, actual: String, expected: String) {
    if !Equals(actual, expected) {
      this.LogFail(from, s"'\(actual)'", s"'\(expected)'");
    } else {
      this.LogPass(from);
    }
  }

  private func ExpectSize(from: String, actual: Uint32, expected: Uint32) {
    if !Equals(actual, expected) {
      this.LogFail(from, s"\(actual)", s"\(expected)");
    }/* else {
      this.LogPass(from);
    }*/
  }

  private func ExpectJsonKeyNull(obj: wref<JsonObject>, key: String) -> Void {
    let from = s"obj['\(key)']";

    if !obj.HasKey(key) {
      this.LogFail(from, "<undefined>", "<null>");
      return;
    }
    let actual = obj.GetKey(key);

    if !actual.IsNull() {
      this.LogFail(from, "?", "<null>");
    } else {
      this.LogPass(from);
    }
  }

  private func ExpectJsonKeyBool(obj: wref<JsonObject>, key: String, expected: Bool) -> Void {
    let from = s"obj['\(key)']";

    if !obj.HasKey(key) {
      this.LogFail(from, "<undefined>", s"\(expected)");
      return;
    }
    let actual = obj.GetKeyBool(key);

    if !Equals(actual, expected) {
      this.LogFail(from, s"\(actual)", s"\(expected)");
    } else {
      this.LogPass(from);
    }
  }

  private func ExpectJsonKeyInt64(obj: wref<JsonObject>, key: String, expected: Int64) -> Void {
    let from = s"obj['\(key)']";

    if !obj.HasKey(key) {
      this.LogFail(from, "<undefined>", s"\(expected)");
      return;
    }
    let actual = obj.GetKeyInt64(key);

    if !Equals(actual, expected) {
      this.LogFail(from, s"\(actual)", s"\(expected)");
    } else {
      this.LogPass(from);
    }
  }

  private func ExpectJsonKeyDouble(obj: wref<JsonObject>, key: String, expected: Double) -> Void {
    let from = s"obj['\(key)']";

    if !obj.HasKey(key) {
      this.LogFail(from, "<undefined>", s"\(expected)");
      return;
    }
    let actual = obj.GetKeyDouble(key);

    // Use formatted string to compare, raw type comparison is not precise.
    if !Equals(s"\(actual)", s"\(expected)") {
      this.LogFail(from, s"\(actual)", s"\(expected)");
    } else {
      this.LogPass(from);
    }
  }

  private func ExpectJsonKeyString(obj: wref<JsonObject>, key: String, expected: String) -> Void {
    let from = s"obj['\(key)']";

    if !obj.HasKey(key) {
      this.LogFail(from, "<undefined>", expected);
      return;
    }
    let actual = obj.GetKeyString(key);

    if !Equals(actual, expected) {
      this.LogFail(from, s"'\(actual)'", s"'\(expected)'");
    } else {
      this.LogPass(from);
    }
  }

  private func ExpectFileStat(file: ref<File>, fileName: String, extension: String, size: Uint64) -> Void {
    this.ExpectString(s"fileName: '\(fileName)'", file.GetFilename(), fileName);
    this.ExpectString(s"extension: '\(extension)'", file.GetExtension(), extension);
    this.ExpectUint64(s"size: \(size) bytes", file.GetSize(), size);
  }

  private func LogFail(from: String, actual: String, expected: String) {
    LogChannel(n"Error", s"FAIL: \(from)");
    LogChannel(n"Error", s"  Actual: \(actual)");
    LogChannel(n"Error", s"  Expected: \(expected)");
    this.m_failCount += 1;
    this.m_totalCount += 1;
  }

  private func LogPass(from: String) {
    LogChannel(n"Info", s"PASS: \(from)");
    this.m_passCount += 1;
    this.m_totalCount += 1;
  }

}