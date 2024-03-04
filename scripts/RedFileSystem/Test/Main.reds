import Codeware.*

public class MainTest extends ScriptableEnv {
  private let m_storage: ref<FileSystemStorage>;

  private let m_runner: ref<RedTest>;

  // Get singleton instance for right cases.
  public func GetStorage() -> ref<FileSystemStorage> {
    return this.m_storage;
  }

  /// Lifecycle ///

  private cb func OnLoad() {
    this.m_storage = FileSystem.GetStorage("Test");
    this.m_runner = new RedTest();
    this.m_runner.Setup("RedFileSystem", [
      new FileSystemTest(),
      new FileSystemStorageTest(),
      new FileTest(),

      new JsonArrayTest()
    ]);
  }

}

public static func GetMainTest() -> ref<MainTest> {
  return ScriptableEnv.Get(n"MainTest") as MainTest;
}
