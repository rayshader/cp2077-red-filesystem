public native class JsonObject extends JsonVariant {

  public native func GetKeys() -> array<String>;
  public native func GetValues() -> array<ref<JsonVariant>>;

  public native func HasKey(key: String) -> Bool;
  public native func GetKey(key: String) -> ref<JsonVariant>;
  public native func SetKey(key: String, value: ref<JsonVariant>) -> Void;
  public native func RemoveKey(key: String) -> Bool;

  public native func GetKeyBool(key: String) -> Bool;
  public native func GetKeyInt64(key: String) -> Int64;
  public native func GetKeyDouble(key: String) -> Double;
  public native func GetKeyString(key: String) -> String;

  public native func SetKeyNull(key: String) -> Void;
  public native func SetKeyBool(key: String, value: Bool) -> Void;
  public native func SetKeyInt64(key: String, value: Int64) -> Void;
  public native func SetKeyDouble(key: String, value: Double) -> Void;
  public native func SetKeyString(key: String, value: String) -> Void;

  public native func Clear() -> Void;

}