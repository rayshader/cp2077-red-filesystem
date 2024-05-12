import RedFileSystem.*
import RedData.Json.*

public class AsyncFileTest extends BaseTest {
  private let STORAGE_PATH: String = "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Cyberpunk 2077\\red4ext\\plugins\\RedFileSystem\\storages\\Test\\";

  private let m_storage: ref<FileSystemStorage>;

  public func Create() {
    this.m_modName = "RedFileSystem";
    this.m_name = "AsyncFile";
    this.m_isAsync = true;
  }

  public func Setup() {
    this.m_storage = GetRedFSTest().GetStorage();
  }

  /// Stat functions ///

  private cb func Test_Stat(done: ref<CallbackTest>) {
    let path = "test.json";
    let file = this.m_storage.GetAsyncFile(path);

    this.ExpectString("GetPath", file.GetPath(), path);
    this.ExpectString("GetAbsolutePath", file.GetAbsolutePath(), s"\(this.STORAGE_PATH)\(path)");
    this.ExpectString("GetFilename", file.GetFilename(), "test.json");
    this.ExpectString("GetExtension", file.GetExtension(), ".json");
    this.ExpectUint64("GetSize", file.GetSize(), 1063ul);
    done.Call();
  }

  /// Read ///

  private cb func Test_ReadAsText(done: ref<CallbackTest>) {
    let file = this.m_storage.GetAsyncFile("test.txt");

    if !IsDefined(file) {
      this.ExpectBool("ReadAsText /!\\ File not found /!\\", IsDefined(file), true);
      done.Call();
      return;
    }
    let promise = FilePromise.Create(this, n"Async_ReadAsText_Pass", n"Async_ReadAsText_Fail", [done]);
    
    file.ReadAsText(promise);
  }

  private cb func Async_ReadAsText_Pass(actual: String, data: array<Variant>) {
    let expect = "صِف خَلقَ خَودِ كَمِثلِ الشَمسِ إِذ بَزَغَت — يَحظى الضَجيعُ بِها نَجلاءَ مِعطارِ (A poem by Al Farāhīdi)\n" +
      "ऋषियों को सताने वाले दुष्ट राक्षसों के राजा रावण का सर्वनाश करने वाले विष्णुवतार भगवान श्रीराम, अयोध्या के महाराज दशरथ के बड़े सपुत्र थे।\n" +
      "いろはにほへと ちりぬるを わかよたれそ つねならむ うゐのおくやま けふこえて あさきゆめみし ゑひもせす（ん）\n" +
      "Sic fugiens, dux, zelotypos, quam Karus haberis.\n" +
      "\n" +
      "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.";

    this.ExpectBool("ReadAsText size", StrLen(actual) == StrLen(expect), true);
    this.ExpectUnicodeString("ReadAsText content", actual, expect);
    let done: ref<CallbackTest> = FromVariant(data[0]);

    done.Call();
  }

  private cb func Async_ReadAsText_Fail(data: array<Variant>) {
    this.ExpectBool("ReadAsText failure", true, false);
    let done: ref<CallbackTest> = FromVariant(data[0]);

    done.Call();
  }

  private cb func Test_ReadAsLines(done: ref<CallbackTest>) {
    let file = this.m_storage.GetAsyncFile("test.txt");

    if !IsDefined(file) {
      this.ExpectBool("ReadAsLines /!\\ File not found /!\\", IsDefined(file), true);
      return;
    }
    let promise = FilePromise.Create(this, n"Async_ReadAsLines_Pass", n"Async_ReadAsLines_Fail", [done]);
    
    file.ReadAsLines(promise);
  }

  private cb func Async_ReadAsLines_Pass(actual: array<String>, data: array<Variant>) {
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
    let done: ref<CallbackTest> = FromVariant(data[0]);

    done.Call();
  }

  private cb func Async_ReadAsLines_Fail(data: array<Variant>) {
    this.ExpectBool("ReadAsLines failure", true, false);
    let done: ref<CallbackTest> = FromVariant(data[0]);

    done.Call();
  }

  private cb func Test_ReadAsJson(done: ref<CallbackTest>) {
    let file = this.m_storage.GetAsyncFile("test.json");

    if !IsDefined(file) {
      this.ExpectBool("ReadAsJson /!\\ File not found /!\\", IsDefined(file), true);
      return;
    }
    let promise = FilePromise.Create(this, n"Async_ReadAsJson_Pass", n"Async_ReadAsJson_Fail", [done]);
  
    file.ReadAsJson(promise);
  }

  private cb func Async_ReadAsJson_Pass(actual: ref<JsonVariant>, data: array<Variant>) {
    let json = actual as JsonObject;

    this.ExpectBool("Json is valid", IsDefined(json), true);
    let done: ref<CallbackTest> = FromVariant(data[0]);

    done.Call();
  }

  private cb func Async_ReadAsJson_Fail(data: array<Variant>) {
    this.ExpectBool("ReadAsJson failure", true, false);
    let done: ref<CallbackTest> = FromVariant(data[0]);

    done.Call();
  }

  /// Write ///

  private cb func Test_WriteText(done: ref<CallbackTest>) {
    let path = "write-text-async.txt";
    let status = this.m_storage.Exists(path);

    if !Equals(status, FileSystemStatus.False) {
      LogChannel(n"Error", s"WriteText file already present /!\\ Abort /!\\");
      return;
    }
    let file = this.m_storage.GetAsyncFile(path);
    let text = "Welcome to Night City!\n" +
               "Let's do this choom ;)\n";
    let promise = FilePromise.Create(this, n"Async_WriteText_Pass", n"Async_WriteText_Fail", [done]);

    file.WriteText(promise, text, FileSystemWriteMode.Truncate);
  }

  private cb func Async_WriteText_Pass(data: array<Variant>) {
    this.ExpectBool("WriteText truncate mode", true, true);
    let path = "write-text-async.txt";
    let status = this.m_storage.Exists(path);

    this.ExpectString("WriteText file created", s"\(status)", "True");
    let expect = "Welcome to Night City!\n" +
                 "Let's do this choom ;)\n";
    let file = this.m_storage.GetFile(path);
    let actual = file.ReadAsText();

    this.ExpectUnicodeString("WriteText content", actual, expect);
    let done: ref<CallbackTest> = FromVariant(data[0]);

    done.Call();
  }

  private cb func Async_WriteText_Fail(data: array<Variant>) {
    this.ExpectBool("WriteText failure", true, false);
    let done: ref<CallbackTest> = FromVariant(data[0]);

    done.Call();
  }

  private cb func Test_WriteLines(done: ref<CallbackTest>) {
    let path = "write-lines-async.txt";
    let status = this.m_storage.Exists(path);

    if !Equals(status, FileSystemStatus.False) {
      LogChannel(n"Error", s"WriteLines file already present /!\\ Abort /!\\");
      return;
    }
    let file = this.m_storage.GetAsyncFile(path);
    let lines = [
      "Welcome to Night City!",
      "Let's do this choom ;)",
      ""
    ];
    let promise = FilePromise.Create(this, n"Async_WriteLines_Pass", n"Async_WriteLines_Fail", [done]);

    file.WriteLines(promise, lines, FileSystemWriteMode.Truncate);
  }

  private cb func Async_WriteLines_Pass(data: array<Variant>) {
    this.ExpectBool("WriteLines truncate mode", true, true);
    let path = "write-lines-async.txt";
    let status = this.m_storage.Exists(path);

    this.ExpectString("WriteLines file created", s"\(status)", "True");
    let expect = "Welcome to Night City!\n" +
                 "Let's do this choom ;)\n";
    let file = this.m_storage.GetFile(path);
    let text = file.ReadAsText();

    this.ExpectUnicodeString("WriteLines content", text, expect);
    let done: ref<CallbackTest> = FromVariant(data[0]);

    done.Call();
  }

  private cb func Async_WriteLines_Fail(data: array<Variant>) {
    this.ExpectBool("WriteLines failure", true, false);
    let done: ref<CallbackTest> = FromVariant(data[0]);

    done.Call();
  }

  /*
  private cb func Test_WriteJson_Minified(done: ref<CallbackTest>) {
  }

  private cb func Test_WriteJson_Pretty(done: ref<CallbackTest>) {
  }
  */

  private cb func Test_WriteText_ThreadSafe(done: ref<CallbackTest>) {
    let path = "write-text-async-threadsafe.txt";
    let status = this.m_storage.Exists(path);

    if !Equals(status, FileSystemStatus.False) {
      LogChannel(n"Error", s"WriteText ThreadSafe file already present /!\\ Abort /!\\");
      return;
    }
    let i = 0;

    while i < 10 {
      let promise = FilePromise.Create(this, n"Async_WriteText_ThreadSafe_Pass", n"Async_WriteText_ThreadSafe_Fail", [i, done]);
      let file = this.m_storage.GetAsyncFile(path);
      let text = s"Choom #\(i + 1) ;)\n";

      file.WriteText(promise, text, FileSystemWriteMode.Append);
      i += 1;
    }
  }

  private cb func Async_WriteText_ThreadSafe_Pass(data: array<Variant>) {
    let i: Int32 = FromVariant(data[0]);

    this.ExpectBool(s"WriteText ThreadSafe #\(i + 1) append mode", true, true);
    if i != 9 {
      return;
    }
    let path = "write-text-async-threadsafe.txt";
    let status = this.m_storage.Exists(path);

    this.ExpectString(s"WriteText ThreadSafe file created", s"\(status)", "True");
    let expect = "";
    let i = 0;

    while i < 10 {
      expect += s"Choom #\(i + 1) ;)\n";
      i += 1;
    }
    let file = this.m_storage.GetFile(path);
    let actual = file.ReadAsText();

    this.ExpectUnicodeString(s"WriteText ThreadSafe content", actual, expect);
    let done: ref<CallbackTest> = FromVariant(data[1]);

    done.Call();
  }

  private cb func Async_WriteText_ThreadSafe_Fail(data: array<Variant>) {
    let i: Int32 = FromVariant(data[0]);

    this.ExpectBool(s"WriteText ThreadSafe #\(i + 1) failure", true, false);
    let done: ref<CallbackTest> = FromVariant(data[1]);

    done.Call();
  }

}
