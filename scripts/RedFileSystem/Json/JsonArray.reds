public native class JsonArray extends JsonVariant {

  public native func GetSize() -> Uint32;
  public native func GetItem(index: Uint32) -> ref<JsonVariant>;
  public native func SetItem(index: Uint32, value: ref<JsonVariant>) -> Void;
  public native func RemoveItem(index: Uint32) -> Bool;
  public native func AddItem(value: ref<JsonVariant>) -> Void;
  public native func InsertItem(index: Uint32, value: ref<JsonVariant>) -> Void;

  public native func GetItemBool(index: Uint32) -> Bool;
  public native func GetItemInt64(index: Uint32) -> Int64;
  public native func GetItemDouble(index: Uint32) -> Double;
  public native func GetItemString(index: Uint32) -> String;

  public native func SetItemNull(index: Uint32) -> Void;
  public native func SetItemBool(index: Uint32, value: Bool) -> Void;
  public native func SetItemInt64(index: Uint32, value: Int64) -> Void;
  public native func SetItemDouble(index: Uint32, value: Double) -> Void;
  public native func SetItemString(index: Uint32, value: String) -> Void;

  public native func AddItemNull() -> Void;
  public native func AddItemBool(value: Bool) -> Void;
  public native func AddItemInt64(value: Int64) -> Void;
  public native func AddItemDouble(value: Double) -> Void;
  public native func AddItemString(value: String) -> Void;

  public native func InsertItemNull(index: Uint32) -> Void;
  public native func InsertItemBool(index: Uint32, value: Bool) -> Void;
  public native func InsertItemInt64(index: Uint32, value: Int64) -> Void;
  public native func InsertItemDouble(index: Uint32, value: Double) -> Void;
  public native func InsertItemString(index: Uint32, value: String) -> Void;

  public native func Clear() -> Void;

}