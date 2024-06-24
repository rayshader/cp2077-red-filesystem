module RedFileSystem

public native struct FilePromise {

  public native let target: wref<IScriptable>;
  public native let resolve: CName;
  public native let reject: CName;
  public native let data: array<Variant>;

  public static func Create(target: wref<IScriptable>, resolve: CName, opt reject: CName, opt data: array<Variant>) -> FilePromise {
    let self: FilePromise;

    self.target = target;
    self.resolve = resolve;
    self.reject = reject;
    self.data = data;
    return self;
  }

}