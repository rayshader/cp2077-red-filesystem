public class FileTest extends JsonBaseTest {
  private static let GAME_PATH: String = "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Cyberpunk 2077\\";
  private static let CET_PATH: String = "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Cyberpunk 2077\\bin\\x64\\plugins\\cyber_engine_tweaks\\mods\\";

  public func Init() {
    this.m_modName = "RedFileSystem";
    this.m_name = "File";

    this.AddTest(n"Test_Stat");

    this.AddTest(n"Test_ReadAsText");
    this.AddTest(n"Test_ReadAsLines");
    this.AddTest(n"Test_ReadAsJson");

    this.AddTest(n"Test_WriteText");
    this.AddTest(n"Test_WriteLines");

    this.AddTest(n"Test_WriteJson");
  }

  /// Stat functions ///

  private cb func Test_Stat() {
    let path = "bin\\x64\\Cyberpunk2077.exe";
    let file = FileSystem.GetFile(path);

    this.ExpectString("GetPath", file.GetPath(), path);
    this.ExpectString("GetAbsolutePath", file.GetAbsolutePath(), s"\(this.GAME_PATH)\(path)");
    this.ExpectString("GetFilename", file.GetFilename(), "Cyberpunk2077.exe");
    this.ExpectString("GetExtension", file.GetExtension(), ".exe");
    this.ExpectUint64("GetSize", file.GetSize(), 59170304ul);
  }

  /// Read ///

  private cb func Test_ReadAsText() {
    let file = FileSystem.GetFile("RedFileSystem\\Test\\test.txt", FileSystemPrefix.Redscript);

    if !IsDefined(file) {
      this.ExpectBool("ReadAsText /!\\ File not found /!\\", IsDefined(file), true);
      return;
    }
    let actual = file.ReadAsText();
    let expect = "صِف خَلقَ خَودِ كَمِثلِ الشَمسِ إِذ بَزَغَت — يَحظى الضَجيعُ بِها نَجلاءَ مِعطارِ (A poem by Al Farāhīdi)\n" +
      "ऋषियों को सताने वाले दुष्ट राक्षसों के राजा रावण का सर्वनाश करने वाले विष्णुवतार भगवान श्रीराम, अयोध्या के महाराज दशरथ के बड़े सपुत्र थे।\n" +
      "いろはにほへと ちりぬるを わかよたれそ つねならむ うゐのおくやま けふこえて あさきゆめみし ゑひもせす（ん）\n" +
      "Sic fugiens, dux, zelotypos, quam Karus haberis.\n" +
      "\n" +
      "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.";

    this.ExpectBool("ReadAsText size", StrLen(actual) == StrLen(expect), true);
    this.ExpectUnicodeString("ReadAsText content", actual, expect);
  }

  private cb func Test_ReadAsLines() {
    let file = FileSystem.GetFile("RedFileSystem\\Test\\test.txt", FileSystemPrefix.Redscript);

    if !IsDefined(file) {
      this.ExpectBool("ReadAsLines /!\\ File not found /!\\", IsDefined(file), true);
      return;
    }
    let actual = file.ReadAsLines();
    let expect = [
      "صِف خَلقَ خَودِ كَمِثلِ الشَمسِ إِذ بَزَغَت — يَحظى الضَجيعُ بِها نَجلاءَ مِعطارِ (A poem by Al Farāhīdi)",
      "ऋषियों को सताने वाले दुष्ट राक्षसों के राजा रावण का सर्वनाश करने वाले विष्णुवतार भगवान श्रीराम, अयोध्या के महाराज दशरथ के बड़े सपुत्र थे।",
      "いろはにほへと ちりぬるを わかよたれそ つねならむ うゐのおくやま けふこえて あさきゆめみし ゑひもせす（ん）",
      "Sic fugiens, dux, zelotypos, quam Karus haberis.",
      "",
      "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua."
    ];
    let pass = this.ExpectBool("ReadAsLines size", ArraySize(actual) == ArraySize(expect), true);

    if !pass {
      LogChannel(n"Error", "ReadAsLines /!\\ Abort /!\\");
      return;
    }
    let match = true;
    let i = 0;

    while i < ArraySize(expect) {
      if !UnicodeStringEqual(actual[i], expect[i]) {
        match = false;
        break;
      }
      i += 1;
    }
    this.ExpectBool("ReadAsLines content", match, true);
  }

  private cb func Test_ReadAsJson() {
    let file = FileSystem.GetFile("RedFileSystem\\Test\\test.json", FileSystemPrefix.Redscript);

    if !IsDefined(file) {
      this.ExpectBool("ReadAsJson /!\\ File not found /!\\", IsDefined(file), true);
      return;
    }
    let json = file.ReadAsJson() as JsonObject;
    let pass = this.ExpectBool("ReadAsJson format valid", IsDefined(json), true);

    if !pass {
      LogChannel(n"Error", "ReadAsJson /!\\ invalid Json format /!\\");
      return;
    }
    this.ExpectJsonKeyString("ReadAsJson $['name']", json, "name", "FileSystemTest");
    this.ExpectJsonKeyInt64("ReadAsJson $['version']", json, "version", 1l);
    this.ExpectJsonKeyBool("ReadAsJson $['isJson']", json, "isJson", true);
    this.ExpectJsonKeyDouble("ReadAsJson $['delta']", json, "delta", 0.016);
    let pangrams = json.GetKey("pangrams") as JsonObject;

    pass = this.ExpectBool("ReadAsJson $['pangrams']{...}", pangrams.IsObject(), true);
    if !pass {
      LogChannel(n"Error", "ReadAsJson $['pangrams'] /!\\ invalid Json format /!\\");
      return;
    }
    this.ExpectJsonKeyUnicodeString("ReadAsJson $['pangrams']['Arabic']", pangrams, "Arabic", "صِف خَلقَ خَودِ كَمِثلِ الشَمسِ إِذ بَزَغَت — يَحظى الضَجيعُ بِها نَجلاءَ مِعطارِ (A poem by Al Farāhīdi)");
    this.ExpectJsonKeyUnicodeString("ReadAsJson $['pangrams']['Hindi']", pangrams, "Hindi", "ऋषियों को सताने वाले दुष्ट राक्षसों के राजा रावण का सर्वनाश करने वाले विष्णुवतार भगवान श्रीराम, अयोध्या के महाराज दशरथ के बड़े सपुत्र थे।");
    this.ExpectJsonKeyUnicodeString("ReadAsJson $['pangrams']['Japanese']", pangrams, "Japanese", "いろはにほへと ちりぬるを わかよたれそ つねならむ うゐのおくやま けふこえて あさきゆめみし ゑひもせす（ん）");
    this.ExpectJsonKeyUnicodeString("ReadAsJson $['pangrams']['Latin']", pangrams, "Latin", "Sic fugiens, dux, zelotypos, quam Karus haberis.");
    let random = json.GetKey("random") as JsonArray;

    pass = this.ExpectBool("ReadAsJson $['random'][...]", random.IsArray(), true);
    if !pass {
      LogChannel(n"Error", "ReadAsJson $['random'] /!\\ invalid Json format /!\\");
      return;
    }
    this.ExpectBool("ReadAsJson $['random'][0]", random.GetItemBool(0u), true);
    this.ExpectInt64("ReadAsJson $['random'][1]", random.GetItemInt64(1u), 42l);
    this.ExpectDouble("ReadAsJson $['random'][2]", random.GetItemDouble(2u), 13.37);
    this.ExpectString("ReadAsJson $['random'][3]", random.GetItemString(3u), "Weird");
    let obj = random.GetItem(4u) as JsonObject;

    pass = this.ExpectBool("ReadAsJson $['random'][4]{...}", obj.IsObject(), true);
    if !pass {
      LogChannel(n"Error", "ReadAsJson $['random'][4] /!\\ invalid Json format /!\\");
      return;
    }
    this.ExpectJsonKeyNull("ReadAsJson $['random'][4]['object']", obj, "object");
    let array = random.GetItem(5u) as JsonArray;

    pass = this.ExpectBool("ReadAsJson $['random'][5][...]", array.IsArray(), true);
    if !pass {
      LogChannel(n"Error", "ReadAsJson $['random'][5] /!\\ invalid Json format /!\\");
      return;
    }
    this.ExpectString("ReadAsJson $['random'][5][0]", array.GetItemString(0u), "array");
  }

  /// Write ///

  private cb func Test_WriteText() {
    let path = "RedFileSystem\\Test\\write-text.txt";
    let status = FileSystem.Exists(path, FileSystemPrefix.Redscript);

    if !Equals(status, FileSystemStatus.False) {
      LogChannel(n"Error", s"WriteText file already present /!\\ Abort /!\\");
      return;
    }
    let file = FileSystem.GetFile(path, FileSystemPrefix.Redscript);
    let text = "Welcome to Night City!\n" +
               "Let's do this choom ;)\n";
    let pass = file.WriteText(text, FileSystemWriteMode.Truncate);

    pass = this.ExpectBool("WriteText truncate mode", pass, true);
    if !pass {
      return;
    }
    status = FileSystem.Exists(path, FileSystemPrefix.Redscript);
    pass = this.ExpectString("WriteText file created", s"\(status)", "True");
    if !pass {
      return;
    }
    text = "\n" + 
           "Beware of flatlines...";
    pass = file.WriteText(text, FileSystemWriteMode.Append);
    pass = this.ExpectBool("WriteText append mode", pass, true);
    if !pass {
      return;
    }
    let expect = "Welcome to Night City!\n" +
                 "Let's do this choom ;)\n" +
                 "\n" + 
                 "Beware of flatlines...";
    text = file.ReadAsText();

    this.ExpectUnicodeString("WriteText content", text, expect);
  }

  private cb func Test_WriteLines() {
    let path = "RedFileSystem\\Test\\write-lines.txt";
    let status = FileSystem.Exists(path, FileSystemPrefix.Redscript);

    if !Equals(status, FileSystemStatus.False) {
      LogChannel(n"Error", s"WriteLines file already present /!\\ Abort /!\\");
      return;
    }
    let file = FileSystem.GetFile(path, FileSystemPrefix.Redscript);
    let lines = [
      "Welcome to Night City!",
      "Let's do this choom ;)",
      ""
    ];
    let pass = file.WriteLines(lines, FileSystemWriteMode.Truncate);

    pass = this.ExpectBool("WriteLines truncate mode", pass, true);
    if !pass {
      return;
    }
    status = FileSystem.Exists(path, FileSystemPrefix.Redscript);
    pass = this.ExpectString("WriteLines file created", s"\(status)", "True");
    if !pass {
      return;
    }
    lines = [
      "",
      "Beware of flatlines..."
    ];
    pass = file.WriteLines(lines, FileSystemWriteMode.Append);
    pass = this.ExpectBool("WriteLines append mode", pass, true);
    if !pass {
      return;
    }
    let expect = "Welcome to Night City!\n" +
                 "Let's do this choom ;)\n" +
                 "\n" + 
                 "Beware of flatlines...";
    let text = file.ReadAsText();

    this.ExpectUnicodeString("WriteLines content", text, expect);
  }

  private cb func Test_WriteJson() {
    let path = "RedFileSystem\\Test\\write.json";
    let status = FileSystem.Exists(path, FileSystemPrefix.Redscript);

    if !Equals(status, FileSystemStatus.False) {
      LogChannel(n"Error", s"WriteJson file already present /!\\ Abort /!\\");
      return;
    }
    let file = FileSystem.GetFile(path, FileSystemPrefix.Redscript);
    let json = new JsonObject();

    json.SetKeyString("message", "Welcome to Night City!");
    json.SetKeyDouble("version", 2.11);
    json.SetKeyInt64("code", 1337);
    json.SetKeyBool("test", true);
    json.SetKeyNull("empty");
    let array = new JsonArray();

    array.AddItemString("Hello");
    array.AddItemInt64(42);
    array.AddItemDouble(3.14159);
    array.AddItemNull();
    json.SetKey("items", array);
    let pass = file.WriteJson(json);

    pass = this.ExpectBool("WriteJson write", pass, true);
    if !pass {
      return;
    }
    status = FileSystem.Exists(path, FileSystemPrefix.Redscript);
    pass = this.ExpectString("WriteJson file created", s"\(status)", "True");
    if !pass {
      return;
    }
    let actual = file.ReadAsText();
    // Follow hash order of 'std::unordered_map'.
    let expect = "{\n" +
                 "  \"code\": 1337,\n" +
                 "  \"message\": \"Welcome to Night City!\",\n" +
                 "  \"items\": [\n" +
                 "    \"Hello\",\n" +
                 "    42,\n" +
                 "    3.14159,\n" +
                 "    null\n" +
                 "  ],\n" +
                 "  \"version\": 2.11,\n" +
                 "  \"test\": true,\n" +
                 "  \"empty\": null\n" +
                 "}";

    this.ExpectUnicodeString("WriteJson content", actual, expect);
  }

}
