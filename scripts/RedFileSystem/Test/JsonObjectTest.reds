public class JsonObjectTest extends JsonBaseTest {

  public func Init() {
    this.m_modName = "RedFileSystem";
    this.m_name = "JsonObject";

    this.AddTest(n"Test_Constructor");

    this.AddTest(n"Test_GetKeys");
    this.AddTest(n"Test_GetValues");

    this.AddTest(n"Test_HasKey");
    this.AddTest(n"Test_GetKey");
    this.AddTest(n"Test_SetKey");
    this.AddTest(n"Test_RemoveKey");

    this.AddTest(n"Test_Clear");
  }

  private cb func Test_Constructor() {
    let object = new JsonObject();

    this.ExpectBool("Constructor is object", object.IsObject(), true);
    this.ExpectBool("Constructor $['none'] is null", IsDefined(object.GetKey("none")), false);
  }

  private cb func Test_GetKeys() {
    let object = new JsonObject();

    //
  }

}
