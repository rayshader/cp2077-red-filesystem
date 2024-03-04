public class JsonObjectTest extends JsonBaseTest {

  public func Init() {
    this.m_modName = "RedFileSystem";
    this.m_name = "JsonObject";
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
