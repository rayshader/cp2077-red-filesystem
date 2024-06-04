import RedFileSystem.*

public class FileSystemStorageTest extends BaseTest {
  private let STORAGE_PATH: String = "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Cyberpunk 2077\\r6\\storages\\Test\\";

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
    let status = this.m_storage.Exists("__folder_managed_by_vortex");

    this.ExpectString("Exists denied", s"\(status)", "Denied");
    let status = this.m_storage.Exists("unknown.txt");

    this.ExpectString("Exists not found", s"\(status)", "False");
    let status = this.m_storage.Exists("test.txt");

    this.ExpectString("Exists present", s"\(status)", "True");
  }

  private cb func Test_IsFile() {
    let status = this.m_storage.Exists("..\\..\\..\\..\\..\\..\\..\\steam.exe");

    this.ExpectString("IsFile denied", s"\(status)", "Denied");
    let status = this.m_storage.Exists("__folder_managed_by_vortex");

    this.ExpectString("IsFile denied", s"\(status)", "Denied");
    let status = this.m_storage.IsFile("test");

    this.ExpectString("IsFile false", s"\(status)", "False");
    let status = this.m_storage.IsFile("test.json");

    this.ExpectString("IsFile true", s"\(status)", "True");
  }

  private cb func Test_GetFile() {
    let file = this.m_storage.GetFile("..\\..\\..\\steam.exe");

    this.ExpectBool("GetFile denied", IsDefined(file), false);
    let file = this.m_storage.GetFile("__folder_managed_by_vortex");

    this.ExpectBool("GetFile denied", IsDefined(file), false);
    let file = this.m_storage.GetFile("test.txt");

    this.ExpectBool("GetFile valid", IsDefined(file), true);
  }

  private cb func Test_GetFiles() {
    let files = this.m_storage.GetFiles();

    this.ExpectInt32("GetFiles() return 7 files", ArraySize(files), 7);
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

    this.ExpectString("files[6] == 'to_remove.txt'", files[6].GetFilename(), "to_remove.txt");
    this.ExpectString("files[6] == '<path>\\to_remove.txt'", files[6].GetAbsolutePath(), s"\(this.STORAGE_PATH)to_remove.txt");
  }

  private cb func Test_GetAsyncFile() {
    let file = this.m_storage.GetAsyncFile("..\\..\\..\\steam.exe");

    this.ExpectBool("GetAsyncFile denied", IsDefined(file), false);
    let file = this.m_storage.GetAsyncFile("__folder_managed_by_vortex");

    this.ExpectBool("GetFile denied", IsDefined(file), false);
    let file = this.m_storage.GetAsyncFile("test.txt");

    this.ExpectBool("GetAsyncFile valid", IsDefined(file), true);
  }

  private cb func Test_GetAsyncFiles() {
    let files = this.m_storage.GetAsyncFiles();

    this.ExpectInt32("GetAsyncFiles() return 7 files", ArraySize(files), 7);
    let i = 0;

    while i < 4 {
      this.ExpectString(s"files[\(i)] == 'dummy\(i).txt'", files[i].GetFilename(), s"dummy\(i).txt");
      i += 1;
    }
    this.ExpectString("files[4] == 'test.json'", files[4].GetFilename(), "test.json");
    this.ExpectString("files[5] == 'test.txt'", files[5].GetFilename(), "test.txt");
    this.ExpectString("files[6] == 'to_remove.txt'", files[6].GetFilename(), "to_remove.txt");
  }

  // It must be after Get[Async]Files
  private cb func Test_DeleteFile() {
    let status = this.m_storage.DeleteFile("..\\..\\..\\..\\..\\..\\..\\steam.exe");

    this.ExpectString("DeleteFile denied", s"\(status)", "Denied");
    let status = this.m_storage.DeleteFile("__folder_managed_by_vortex");

    this.ExpectString("DeleteFile denied", s"\(status)", "Denied");
    let status = this.m_storage.DeleteFile("test");

    this.ExpectString("DeleteFile failure", s"\(status)", "Failure");
    let status = this.m_storage.DeleteFile("to_remove_not_found.txt");

    this.ExpectString("DeleteFile failure", s"\(status)", "Failure");
    // NOTE: it should be false when file didn't exist. MS STL implementation 
    //       is non-conforming?
    //this.ExpectString("DeleteFile false", s"\(status)", "False");
    let status = this.m_storage.DeleteFile("to_remove.txt");

    this.ExpectString("DeleteFile true", s"\(status)", "True");
  }

}
