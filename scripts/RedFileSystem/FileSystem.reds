public native class FileSystem {

  public static native func Exists(path: String, opt prefix: FileSystemPrefix) -> FileSystemStatus;
  public static native func IsFile(path: String, opt prefix: FileSystemPrefix) -> FileSystemStatus;

  public static native func GetFile(path: String, opt prefix: FileSystemPrefix) -> ref<File>;

}