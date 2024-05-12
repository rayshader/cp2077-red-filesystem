public native class FileSystemStorage {

  public native func Exists(path: String) -> FileSystemStatus;
  public native func IsFile(path: String) -> FileSystemStatus;

  public native func GetFile(path: String) -> ref<File>;
  public native func GetFiles() -> array<ref<File>>;

  public native func GetAsyncFile(path: String) -> ref<AsyncFile>;
  public native func GetAsyncFiles() -> array<ref<AsyncFile>>;

}