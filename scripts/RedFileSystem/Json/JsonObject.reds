public native class JsonObject extends JsonVariant {

  public native func GetKeys() -> array<String>;
  public native func GetValues() -> array<ref<JsonVariant>>;

  public native func HasKey(key: String) -> Bool;
  public native func GetKey(key: String) -> ref<JsonVariant>;

  public native func GetKeyBool(key: String) -> Bool;
  public native func GetKeyInt64(key: String) -> Int64;
  public native func GetKeyDouble(key: String) -> Double;
  public native func GetKeyString(key: String) -> String;

}