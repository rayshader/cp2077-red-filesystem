public class FileTest extends BaseTest {
  private let STORAGE_PATH: String = "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Cyberpunk 2077\\red4ext\\plugins\\RedFileSystem\\storages\\Test\\";

  private let m_storage: ref<FileSystemStorage>;

  public func Init() {
    this.m_modName = "RedFileSystem";
    this.m_name = "File";

    this.m_storage = GetRedFSTest().GetStorage();
  }

  /// Stat functions ///

  private cb func Test_Stat() {
    let path = "test.json";
    let file = this.m_storage.GetFile(path);

    this.ExpectString("GetPath", file.GetPath(), path);
    this.ExpectString("GetAbsolutePath", file.GetAbsolutePath(), s"\(this.STORAGE_PATH)\(path)");
    this.ExpectString("GetFilename", file.GetFilename(), "test.json");
    this.ExpectString("GetExtension", file.GetExtension(), ".json");
    this.ExpectUint64("GetSize", file.GetSize(), 1063ul);
  }

  /// Read ///

  private cb func Test_ReadAsText() {
    let file = this.m_storage.GetFile("test.txt");

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
    let file = this.m_storage.GetFile("test.txt");

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
    let file = this.m_storage.GetFile("test.json");

    if !IsDefined(file) {
      this.ExpectBool("ReadAsJson /!\\ File not found /!\\", IsDefined(file), true);
      return;
    }
    let json = file.ReadAsJson() as JsonObject;
    
    this.ExpectBool("Json is valid", IsDefined(json), true);
  }

  /// Write ///

  private cb func Test_WriteText() {
    let path = "write-text.txt";
    let status = this.m_storage.Exists(path);

    if !Equals(status, FileSystemStatus.False) {
      LogChannel(n"Error", s"WriteText file already present /!\\ Abort /!\\");
      return;
    }
    let file = this.m_storage.GetFile(path);
    let text = "Welcome to Night City!\n" +
               "Let's do this choom ;)\n";
    let pass = file.WriteText(text, FileSystemWriteMode.Truncate);

    pass = this.ExpectBool("WriteText truncate mode", pass, true);
    if !pass {
      return;
    }
    status = this.m_storage.Exists(path);
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
    let path = "write-lines.txt";
    let status = this.m_storage.Exists(path);

    if !Equals(status, FileSystemStatus.False) {
      LogChannel(n"Error", s"WriteLines file already present /!\\ Abort /!\\");
      return;
    }
    let file = this.m_storage.GetFile(path);
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
    status = this.m_storage.Exists(path);
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
    let path = "write.json";
    let status = this.m_storage.Exists(path);

    if !Equals(status, FileSystemStatus.False) {
      LogChannel(n"Error", s"WriteJson file already present /!\\ Abort /!\\");
      return;
    }
    let file = this.m_storage.GetFile(path);
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
    status = this.m_storage.Exists(path);
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
