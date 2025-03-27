module RedFileSystem

@if(ModuleExists("RedData.Json"))
import RedData.Json.*

public native class AsyncFile {

  public native func GetPath() -> String;
  public native func GetAbsolutePath() -> String;
  public native func GetFilename() -> String;
  public native func GetExtension() -> String;
  public native func GetSize() -> Uint64;

  public native func ReadAsText(promise: FilePromise) -> Void;
  public native func ReadAsBase64(promise: FilePromise) -> Void;
  public native func ReadAsLines(promise: FilePromise) -> Void;
  @if(ModuleExists("RedData.Json"))
  public native func ReadAsJson(promise: FilePromise) -> Void;

  public native func WriteText(promise: FilePromise, text: String, opt mode: FileSystemWriteMode) -> Void;
  public native func WriteLines(promise: FilePromise, lines: array<String>, opt mode: FileSystemWriteMode) -> Void;
  @if(ModuleExists("RedData.Json"))
  public native func WriteJson(promise: FilePromise, json: ref<JsonVariant>, opt indent: String) -> Void;

}