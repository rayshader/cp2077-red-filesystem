public class FileSystemStorageTest extends BaseTest {
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

}
