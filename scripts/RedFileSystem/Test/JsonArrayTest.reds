public class JsonArrayTest extends JsonBaseTest {

  public func Init() {
    this.m_modName = "RedFileSystem";
    this.m_name = "JsonArray";

    this.AddTest(n"Test_Constructor");
    this.AddTest(n"Test_AddItems");
    this.AddTest(n"Test_SetItems");
    this.AddTest(n"Test_InsertItems");
    this.AddTest(n"Test_RemoveItems");
    this.AddTest(n"Test_Clear");
  }

  private cb func Test_Constructor() {
    let array = new JsonArray();

    this.ExpectBool("Constructor is array", array.IsArray(), true);
    this.ExpectBool("Constructor is empty", array.GetSize() == 0u, true);
    this.ExpectBool("Constructor $[0] is null", IsDefined(array.GetItem(0u)), false);

    array.SetItem(0u, new JsonObject());
    this.ExpectBool("Constructor $[0] = true is ignored", array.GetSize() == 0u, true);
  }

  private cb func Test_AddItems() {
    let array = new JsonArray();

    array.AddItemNull();
    array.AddItemBool(false);
    array.AddItemInt64(1337);
    array.AddItemDouble(2.11);
    array.AddItemString("Choom");

    this.ExpectBool("AddItems $.size == 5", array.GetSize() == 5u, true);

    this.ExpectBool("AddItems $[0] == null", array.GetItem(0u).IsNull(), true);
    this.ExpectBool("AddItems $[1] == false", array.GetItemBool(1u), false);
    this.ExpectInt64("AddItems $[2] == 1337", array.GetItemInt64(2u), 1337);
    this.ExpectDouble("AddItems $[3] == 2.11", array.GetItemDouble(3u), 2.11);
    this.ExpectString("AddItems $[4] == \"Choom\"", array.GetItemString(4u), "Choom");
  }

  private cb func Test_SetItems() {
    let array = new JsonArray();

    array.AddItemNull();
    array.AddItemBool(false);
    array.AddItemInt64(1337);
    array.AddItemDouble(2.11);
    array.AddItemString("Choom");

    array.SetItemString(0u, "Choom");
    array.SetItemDouble(1u, 2.11);
    array.SetItemInt64(2u, 1337);
    array.SetItemBool(3u, false);
    array.SetItemNull(4u);

    this.ExpectBool("SetItems $.size == 5", array.GetSize() == 5u, true);

    this.ExpectString("SetItems $[0] == \"Choom\"", array.GetItemString(0u), "Choom");
    this.ExpectDouble("SetItems $[1] == 2.11", array.GetItemDouble(1u), 2.11);
    this.ExpectInt64("SetItems $[2] == 1337", array.GetItemInt64(2u), 1337);
    this.ExpectBool("SetItems $[3] == false", array.GetItemBool(3u), false);
    this.ExpectBool("SetItems $[4] == null", array.GetItem(4u).IsNull(), true);
  }

  private cb func Test_InsertItems() {
    let array = new JsonArray();

    array.AddItemNull();
    array.AddItemBool(false);
    array.AddItemInt64(1337);
    array.AddItemDouble(2.11);
    array.AddItemString("Choom");

    array.InsertItemString(4u, "Choom");
    array.InsertItemDouble(3u, 2.11);
    array.InsertItemInt64(2u, 1337);
    array.InsertItemBool(1u, false);
    array.InsertItemNull(0u);

    this.ExpectBool("InsertItems $.size == 10", array.GetSize() == 10u, true);

    this.ExpectBool("InsertItems $[0] == null", array.GetItem(0u).IsNull(), true);
    this.ExpectBool("InsertItems $[1] == null", array.GetItem(1u).IsNull(), true);
    this.ExpectBool("InsertItems $[2] == false", array.GetItemBool(2u), false);
    this.ExpectBool("InsertItems $[3] == false", array.GetItemBool(3u), false);
    this.ExpectInt64("InsertItems $[4] == 1337", array.GetItemInt64(4u), 1337);
    this.ExpectInt64("InsertItems $[5] == 1337", array.GetItemInt64(5u), 1337);
    this.ExpectDouble("InsertItems $[6] == 2.11", array.GetItemDouble(6u), 2.11);
    this.ExpectDouble("InsertItems $[7] == 2.11", array.GetItemDouble(7u), 2.11);
    this.ExpectString("InsertItems $[8] == \"Choom\"", array.GetItemString(8u), "Choom");
    this.ExpectString("InsertItems $[9] == \"Choom\"", array.GetItemString(9u), "Choom");
  }

  private cb func Test_RemoveItems() {
    let array = new JsonArray();

    array.AddItemNull();
    array.AddItemBool(false);
    array.AddItemInt64(1337);
    array.AddItemDouble(2.11);
    array.AddItemString("Choom");

    this.ExpectBool("RemoveItems $.size == 5", array.GetSize() == 5u, true);
    this.ExpectBool("RemoveItems $[0] == null", array.GetItem(0u).IsNull(), true);

    array.RemoveItem(0u);
    this.ExpectBool("RemoveItems $[0] == false", array.GetItemBool(0u), false);

    array.RemoveItem(0u);
    this.ExpectInt64("RemoveItems $[0] == 1337", array.GetItemInt64(0u), 1337);

    array.RemoveItem(0u);
    this.ExpectDouble("RemoveItems $[0] == 2.11", array.GetItemDouble(0u), 2.11);

    array.RemoveItem(0u);
    this.ExpectString("RemoveItems $[0] == \"Choom\"", array.GetItemString(0u), "Choom");

    array.RemoveItem(0u);
    this.ExpectBool("RemoveItems $.size == 0", array.GetSize() == 0u, true);
  }

  private cb func Test_Clear() {
    let array = new JsonArray();

    array.AddItemNull();
    array.AddItemBool(false);
    array.AddItemInt64(1337);
    array.AddItemDouble(2.11);
    array.AddItemString("Choom");
    this.ExpectBool("Clear $.size == 5", array.GetSize() == 5u, true);

    array.Clear();

    this.ExpectBool("Clear $.size == 0", array.GetSize() == 0u, true);
  }

}
