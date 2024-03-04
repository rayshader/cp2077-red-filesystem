// Copy file into r6\scripts\Awesome\
import Codeware.*

// See https://github.com/psiberx/cp2077-codeware/wiki#lifecycle

public class AwesomeEnv extends ScriptableEnv {
  private let m_storage: ref<FileSystemStorage>;

  public func GetStorage() -> ref<FileSystemStorage> {
    return this.m_storage;
  }

  private cb func OnLoad() {
    this.m_storage = FileSystem.GetStorage("Awesome");
  }
}

public static func GetAwesomeEnv() -> ref<AwesomeEnv> {
  return ScriptableEnv.Get(n"AwesomeEnv") as AwesomeEnv;
}
