public class FileSystemTest extends BaseTest {
  private static let GAME_PATH: String = "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Cyberpunk 2077\\";
  private static let CET_PATH: String = "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Cyberpunk 2077\\bin\\x64\\plugins\\cyber_engine_tweaks\\mods\\";

  public func Init() {
    this.m_modName = "RedFileSystem";
    this.m_name = "FileSystem";

    this.AddTest(n"Test_Exists_Denied");
    this.AddTest(n"Test_Exists_NotFound");
    this.AddTest(n"Test_Exists_Present");

    this.AddTest(n"Test_IsFile_Denied");
    this.AddTest(n"Test_IsFile_True");
    this.AddTest(n"Test_IsFile_False");

    this.AddTest(n"Test_GetFile_Denied");
    this.AddTest(n"Test_GetFile_Valid");
  }

  /// Exists ///

  private cb func Test_Exists_Denied() {
    let status = FileSystem.Exists("..\\..\\..\\steam.exe");

    this.ExpectString("Exists denied", s"\(status)", "Denied");
    status = FileSystem.Exists("..\\config.json", FileSystemPrefix.CET);

    this.ExpectString("Exists(CET) denied", s"\(status)", "Denied");
    status = FileSystem.Exists("..\\..\\bin", FileSystemPrefix.Redscript);

    this.ExpectString("Exists(Redscript) denied", s"\(status)", "Denied");
  }

  private cb func Test_Exists_NotFound() {
    let status = FileSystem.Exists("unknown.txt");

    this.ExpectString("Exists not found", s"\(status)", "False");
    let status = FileSystem.Exists("unknown.txt", FileSystemPrefix.CET);

    this.ExpectString("Exists(CET) not found", s"\(status)", "False");
    let status = FileSystem.Exists("unknown.txt", FileSystemPrefix.Redscript);

    this.ExpectString("Exists(Redscript) not found", s"\(status)", "False");
  }

  private cb func Test_Exists_Present() {
    let status = FileSystem.Exists("bin\\x64\\Cyberpunk2077.exe");

    this.ExpectString("Exists present", s"\(status)", "True");
    /*
    let status = FileSystem.Exists("", FileSystemPrefix.CET);

    this.ExpectString("Exists(CET) present", s"\(status)", "True");
    */
    let status = FileSystem.Exists("RedFileSystem\\Test\\FileSystemTest.reds", FileSystemPrefix.Redscript);

    this.ExpectString("Exists(Redscript) present", s"\(status)", "True");
  }

  /// IsFile ///

  private cb func Test_IsFile_Denied() {
    let status = FileSystem.IsFile("..\\..\\..\\steam.exe");

    this.ExpectString("IsFile denied", s"\(status)", "Denied");
  }

  private cb func Test_IsFile_False() {
    let status = FileSystem.IsFile("bin\\");

    this.ExpectString("IsFile false", s"\(status)", "False");
  }

  private cb func Test_IsFile_True() {
    let status = FileSystem.IsFile("bin\\x64\\Cyberpunk2077.exe");

    this.ExpectString("IsFile true", s"\(status)", "True");
  }

  /// GetFile ///

  private cb func Test_GetFile_Denied() {
    let file = FileSystem.GetFile("..\\..\\..\\steam.exe");

    this.ExpectBool("GetFile denied", IsDefined(file), false);
  }

  private cb func Test_GetFile_Valid() {
    let file = FileSystem.GetFile("bin\\x64\\Cyberpunk2077.exe");

    this.ExpectBool("GetFile valid", IsDefined(file), true);
  }

}
