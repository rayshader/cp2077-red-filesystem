module RedFileSystem

public native class FileSystem {

  public static native func GetStorage(name: String) -> ref<FileSystemStorage>;
  public static native func GetSharedStorage() -> ref<FileSystemStorage>;

}