public native class File {

  public native func GetPath() -> String;
  public native func GetAbsolutePath() -> String;
  public native func GetFilename() -> String;
  public native func GetExtension() -> String;
  public native func GetSize() -> Uint64;

  public native func ReadAsText() -> String;
  public native func ReadAsLines() -> array<String>;
  @if(ModuleExists("RedData.Json"))
  public native func ReadAsJson() -> ref<JsonVariant>;

  public native func WriteText(text: String, opt mode: FileSystemWriteMode) -> Bool;
  public native func WriteLines(lines: array<String>, opt mode: FileSystemWriteMode) -> Bool;
  @if(ModuleExists("RedData.Json"))
  public native func WriteJson(json: ref<JsonVariant>, opt indent: String) -> Bool;

}