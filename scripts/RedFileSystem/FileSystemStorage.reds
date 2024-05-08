public native class FileSystemStorage {

  public native func Exists(path: String) -> FileSystemStatus;
  public native func IsFile(path: String) -> FileSystemStatus;

  public native func GetFile(path: String) -> ref<File>;
  public native func GetFiles() -> array<ref<File>>;

}