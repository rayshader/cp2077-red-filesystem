module RedFileSystem.Test

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
    storage = FileSystem.GetStorage("shared");

    this.ExpectBool("Invalid storage name: \"shared\" is forbidden", IsDefined(storage), false);
    storage = FileSystem.GetStorage("ShaRed");

    this.ExpectBool("Invalid storage name: \"ShaRed\" is forbidden", IsDefined(storage), false);
  }

  private cb func Test_GetStorage_Granted() {
    this.m_storage = FileSystem.GetStorage("TestGetStorage");

    this.ExpectBool("First access to storage granted", IsDefined(this.m_storage), true);
  }

  private cb func Test_GetStorage_Revoked() {
    let storage = FileSystem.GetStorage("TestGetStorage");

    this.ExpectBool("Second access to storage revoked", IsDefined(storage), false);
    storage = FileSystem.GetStorage("testgetstorage");

    this.ExpectBool("Third access to storage revoked with case-insensitive", IsDefined(storage), false);
    let status = this.m_storage.Exists("unknown.txt");

    this.ExpectString("First storage 'Exists' is revoked", s"\(status)", "Denied");
    status = this.m_storage.IsFile("unknown.txt");

    this.ExpectString("First storage 'IsFile' is revoked", s"\(status)", "Denied");
    let file = this.m_storage.GetFile("unknown.txt");

    this.ExpectBool("First storage 'GetFile' is revoked", IsDefined(file), false);
  }

  private cb func Test_GetSharedStorage_Granted() {
    let storage = FileSystem.GetSharedStorage();

    this.ExpectBool("Access to shared storage granted", IsDefined(storage), true);
    storage = FileSystem.GetSharedStorage();

    this.ExpectBool("Access to shared storage is not restricted", IsDefined(storage), true);
  }

}
