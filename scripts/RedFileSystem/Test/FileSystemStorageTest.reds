import RedFileSystem.*

public class FileSystemStorageTest extends BaseTest {
  private let STORAGE_PATH: String = "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Cyberpunk 2077\\red4ext\\plugins\\RedFileSystem\\storages\\Test\\";

  private let m_storage: ref<FileSystemStorage>;

  public func Create() {
    this.m_modName = "RedFileSystem";
    this.m_name = "FileSystemStorage";
  }

  public func Setup() {
    this.m_storage = GetRedFSTest().GetStorage();
  }

  private cb func Test_Exists() {
    let status = this.m_storage.Exists("..\\..\\..\\..\\..\\..\\..\\steam.exe");

    this.ExpectString("Exists denied", s"\(status)", "Denied");
    let status = this.m_storage.Exists("..\\");

    this.ExpectString("Exists denied", s"\(status)", "Denied");
    let status = this.m_storage.Exists("unknown.txt");

    this.ExpectString("Exists not found", s"\(status)", "False");
    let status = this.m_storage.Exists("test.txt");

    this.ExpectString("Exists present", s"\(status)", "True");
  }

  private cb func Test_IsFile() {
    let status = this.m_storage.Exists("..\\..\\..\\..\\..\\..\\..\\steam.exe");

    this.ExpectString("IsFile denied", s"\(status)", "Denied");
    let status = this.m_storage.IsFile("test\\");

    this.ExpectString("IsFile false", s"\(status)", "False");
    let status = this.m_storage.IsFile("test.json");

    this.ExpectString("IsFile true", s"\(status)", "True");
  }

  private cb func Test_GetFile() {
    let file = this.m_storage.GetFile("..\\..\\..\\steam.exe");

    this.ExpectBool("GetFile denied", IsDefined(file), false);
    let file = this.m_storage.GetFile("test.txt");

    this.ExpectBool("GetFile valid", IsDefined(file), true);
  }

  private cb func Test_GetFiles() {
    let files = this.m_storage.GetFiles();

    this.ExpectInt32("GetFiles() return 6 files", ArraySize(files), 6);
    let i = 0;

    while i < 4 {
      this.ExpectString(s"files[\(i)] == 'dummy\(i).txt'", files[i].GetFilename(), s"dummy\(i).txt");
      this.ExpectString(s"files[\(i)] == '<path>\\dummy\(i).txt'", files[i].GetAbsolutePath(), s"\(this.STORAGE_PATH)dummy\(i).txt");
      i += 1;
    }
    this.ExpectString("files[4] == 'test.json'", files[4].GetFilename(), "test.json");
    this.ExpectString("files[4] == '<path>\\test.json'", files[4].GetAbsolutePath(), s"\(this.STORAGE_PATH)test.json");

    this.ExpectString("files[5] == 'test.txt'", files[5].GetFilename(), "test.txt");
    this.ExpectString("files[5] == '<path>\\test.txt'", files[5].GetAbsolutePath(), s"\(this.STORAGE_PATH)test.txt");
  }

}
