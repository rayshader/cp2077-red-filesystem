public class MainTest extends ScriptableSystem {
  private let m_runner: ref<RedTest>;

  /// Lifecycle ///

  private func OnAttach() {
    this.m_runner = new RedTest();
    this.m_runner.Setup("RedFileSystem", [
      new FileSystemTest(),
      new FileTest(),

      new JsonArrayTest()
    ]);
  }

  private func OnDetach() {
    this.m_runner.TearDown();
  }

}