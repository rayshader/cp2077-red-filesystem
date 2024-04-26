public native class File {

  public native func GetPath() -> String;
  public native func GetAbsolutePath() -> String;
  public native func GetFilename() -> String;
  public native func GetExtension() -> String;
  public native func GetSize() -> Uint64;

  public native func ReadAsText() -> String;
  public native func ReadAsLines() -> array<String>;

  public native func WriteText(text: String, opt mode: FileSystemWriteMode) -> Bool;
  public native func WriteLines(lines: array<String>, opt mode: FileSystemWriteMode) -> Bool;

  @if(ModuleExists("RedData.Json"))
  public func ReadAsJson() -> ref<JsonVariant> {
    return ParseJson(this.ReadAsText());
  }

  @if(ModuleExists("RedData.Json"))
  public func WriteJson(json: ref<JsonVariant>, opt indent: String) -> Bool {
    return this.WriteText(json.ToString(indent));
  }

}