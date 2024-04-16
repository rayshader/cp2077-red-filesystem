import RedFileSystem.*

public class FileSystemTest extends BaseTest {
  private let m_storage: ref<FileSystemStorage>;

  public func Create() {
    this.m_modName = "RedFileSystem";
    this.m_name = "FileSystem";
  }

  /// GetStorage ///

  private cb func Test_GetStorage_InvalidName() {
    let storage = FileSystem.GetStorage("..\\");

    this.ExpectBool("Invalid storage name: invalid characters", IsDefined(storage), false);
    storage = FileSystem.GetStorage("1337-_-");

    this.ExpectBool("Invalid storage name: invalid characters", IsDefined(storage), false);
    storage = FileSystem.GetStorage("Oh");

    this.ExpectBool("Invalid storage name: mininum 3 characters", IsDefined(storage), false);
    storage = FileSystem.GetStorage("LetsDoSomethingTooLongLikeForReal");

    this.ExpectBool("Invalid storage name: maximum 24 characters", IsDefined(storage), false);
  }

  private cb func Test_GetStorage_Granted() {
    this.m_storage = FileSystem.GetStorage("TestGetStorage");

    this.ExpectBool("First access to storage granted", IsDefined(this.m_storage), true);
  }

  private cb func Test_GetStorage_Revoked() {
    let storage = FileSystem.GetStorage("TestGetStorage");

    this.ExpectBool("Second access to storage revoked", IsDefined(storage), false);
    let status = this.m_storage.Exists("unknown.txt");

    this.ExpectString("First storage 'Exists' is revoked", s"\(status)", "Denied");
    status = this.m_storage.IsFile("unknown.txt");

    this.ExpectString("First storage 'IsFile' is revoked", s"\(status)", "Denied");
    let file = this.m_storage.GetFile("unknown.txt");

    this.ExpectBool("First storage 'GetFile' is revoked", IsDefined(file), false);
  }

}
