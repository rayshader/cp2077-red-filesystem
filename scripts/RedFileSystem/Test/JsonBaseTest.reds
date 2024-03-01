public abstract class JsonBaseTest extends BaseTest {

  protected func ExpectJsonKeyNull(from: String, obj: wref<JsonObject>, key: String) -> Void {
    if !obj.HasKey(key) {
      this.LogFail(from, "<undefined>", "<null>");
      return;
    }
    let actual = obj.GetKey(key);

    if !actual.IsNull() {
      this.LogFail(from, "?", "<null>");
    } else {
      this.LogPass(from);
    }
  }

  protected func ExpectJsonKeyBool(from: String, obj: wref<JsonObject>, key: String, expected: Bool) -> Void {
    if !obj.HasKey(key) {
      this.LogFail(from, "<undefined>", s"\(expected)");
      return;
    }
    let actual = obj.GetKeyBool(key);

    if !Equals(actual, expected) {
      this.LogFail(from, s"\(actual)", s"\(expected)");
    } else {
      this.LogPass(from);
    }
  }

  protected func ExpectJsonKeyInt64(from: String, obj: wref<JsonObject>, key: String, expected: Int64) -> Void {
    if !obj.HasKey(key) {
      this.LogFail(from, "<undefined>", s"\(expected)");
      return;
    }
    let actual = obj.GetKeyInt64(key);

    if !Equals(actual, expected) {
      this.LogFail(from, s"\(actual)", s"\(expected)");
    } else {
      this.LogPass(from);
    }
  }

  protected func ExpectJsonKeyDouble(from: String, obj: wref<JsonObject>, key: String, expected: Double) -> Void {
    if !obj.HasKey(key) {
      this.LogFail(from, "<undefined>", s"\(expected)");
      return;
    }
    let actual = obj.GetKeyDouble(key);

    // Use formatted string to compare, raw type comparison is not precise.
    if !Equals(s"\(actual)", s"\(expected)") {
      this.LogFail(from, s"\(actual)", s"\(expected)");
    } else {
      this.LogPass(from);
    }
  }

  protected func ExpectJsonKeyString(from: String, obj: wref<JsonObject>, key: String, expected: String) -> Void {
    if !obj.HasKey(key) {
      this.LogFail(from, "<undefined>", expected);
      return;
    }
    let actual = obj.GetKeyString(key);

    if !Equals(actual, expected) {
      this.LogFail(from, s"'\(actual)'", s"'\(expected)'");
    } else {
      this.LogPass(from);
    }
  }

  protected func ExpectJsonKeyUnicodeString(from: String, obj: wref<JsonObject>, key: String, expected: String) -> Void {
    if !obj.HasKey(key) {
      this.LogFail(from, "<undefined>", expected);
      return;
    }
    let actual = obj.GetKeyString(key);

    if !UnicodeStringEqual(actual, expected) {
      this.LogFail(from, s"'\(actual)'", s"'\(expected)'");
    } else {
      this.LogPass(from);
    }
  }

}
