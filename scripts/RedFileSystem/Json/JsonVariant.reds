public abstract native class JsonVariant {

  public native func IsUndefined() -> Bool;
  public native func IsNull() -> Bool;
  public native func IsBool() -> Bool;
  public native func IsInt64() -> Bool;
  public native func IsDouble() -> Bool;
  public native func IsString() -> Bool;
  public native func IsArray() -> Bool;
  public native func IsObject() -> Bool;

  public native func GetBool() -> Bool;
  public native func GetInt64() -> Int64;
  public native func GetDouble() -> Double;
  public native func GetString() -> String;

  public native func ToString() -> String;

}