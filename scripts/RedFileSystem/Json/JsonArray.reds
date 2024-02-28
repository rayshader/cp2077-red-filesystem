public native class JsonArray extends JsonVariant {

  public native func GetSize() -> Uint32;
  public native func GetItem(index: Uint32) -> ref<JsonVariant>;

  public native func GetItemBool(index: Uint32) -> Bool;
  public native func GetItemInt64(index: Uint32) -> Int64;
  public native func GetItemDouble(index: Uint32) -> Double;
  public native func GetItemString(index: Uint32) -> String;

}