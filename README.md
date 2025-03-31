# Red File System
![Cyberpunk 2077](https://img.shields.io/badge/Cyberpunk%202077-v2.21-blue)
[![MO2](https://img.shields.io/badge/MO2-compatible-green)](https://github.com/ModOrganizer2/modorganizer/)
![GitHub License](https://img.shields.io/github/license/rayshader/cp2077-red-filesystem)
[![Donate](https://img.shields.io/badge/donate-buy%20me%20a%20coffee-yellow)](https://www.buymeacoffee.com/lpfreelance)

This plugin provides access to the file system using read/write operations. It 
supports UTF8 text and Json formats. It can be used with Redscript and CET.

# Getting started

## Compatibility
- Cyberpunk 2077 v2.21
- [Redscript] v0.5.27+
- [Cyber Engine Tweaks] v1.35.0+

## Installation
1. Install requirements:
  - [RED4ext] v1.27.0+
  - [RedData] v0.7.0+ (only required for Json data)
2. Extract the [latest archive] into the Cyberpunk 2077 directory.

## API wrapper

This plugin can be used when writing a RED4ext plugin with RedLib. You can
find a C++ API wrapper in [branch api].

## Usage

All features are defined in module `RedFileSystem`. You need to import it with:
```swift
import RedFileSystem.*
```

### Disclaimer
All read/write operations are restricted within `Cyberpunk 2077\r6\storages\`.

> [!NOTE]  
> Old path was: `Cyberpunk 2077\red4ext\plugins\RedFileSystem\storages\`. Its 
> content will automatically be migrated to `Cyberpunk 2077\r6\storages\`.

You can get a storage with the name of your mod. It must be unique. You can 
only read/write files within your own storage.

For example, if your mod is named `Awesome`, your storage will be located in
`Cyberpunk 2077\r6\storages\Awesome\`.

This is a security measure to prevent malicious access. This way, you and 
other authors cannot break game files nor the operating system of the player.

### Blacklist
Prevent access to files managed by other processes. For example, Vortex 
adds the file `__folder_managed_by_vortex` when installing a mod. This file 
can be present in your storage. However, when using methods from 
`FileSystemStorage`, this file will be invisible for you. It won't be listed 
either when using `GetFiles()` or `GetAsyncFiles()`.

### FileSystem

#### GetStorage
```swift
public static func GetStorage(name: String) -> ref<FileSystemStorage>;
```

You can get a storage for your mod like this:

```swift
let storage = FileSystem.GetStorage("Awesome");

// ...
```

`name` must comply with the following rules:
- minimum of 3 characters
- maximum of 24 characters
- only `A-Z` and `a-z` characters
- it must not be `shared`

If you use a malformed name, `GetStorage` will return `null`.

You must get your storage only one time when running your mod. If you try to 
call `GetStorage` again, you will no longer be able to use it. This is a 
security measure to make sure only you (author of the mod `Awesome`) is using 
this storage.

For example, if an evil mod tries to hack into your storage, it will also call 
`GetStorage("Awesome")`. In this case, two mods are trying to get the storage 
`Awesome`, you and the evil mod. Because there is no way to detect if the call 
is legitimate (your mod) or not (evil mod), all further attempt to use the 
storage will be denied.

You can store the unique reference of your `FileSystemStorage` with 
`ScriptableService` from [Codeware]. You can find an example in [examples/].

#### GetSharedStorage
```swift
public static func GetSharedStorage() -> ref<FileSystemStorage>;
```

You can use it to read/write in a common storage. It can be accessed without 
restrictions if you need to share data with other mods. It is located in
`Cyberpunk 2077\r6\storages\shared\`.

### FileSystemStorage

#### Exists
```swift
public func Exists(path: String) -> FileSystemStatus;
```

You can test if a path exists, it can be a file or a directory:
```swift
// ...
let path = "config.json";
let status = storage.Exists(path);

if Equals(status, FileSystemStatus.Failure) {
  FTLogError("System error.");
} else if Equals(status, FileSystemStatus.Denied) {
  FTLogError("Operation denied.");
} else if Equals(status, FileSystemStatus.False) {
  FTLogError("File/Directory not found.");
} else if Equals(status, FileSystemStatus.True) {
  FTLogError("File/Directory is present.");
}
```

#### IsFile
```swift
public func IsFile(path: String) -> FileSystemStatus;
```

You can test if a path points to a regular file:
```swift
// ...
let path = "README.md";
let status = storage.IsFile(path);

if Equals(status, FileSystemStatus.Failure) {
  FTLogError("System error.");
} else if Equals(status, FileSystemStatus.Denied) {
  FTLogError("Operation denied.");
} else if Equals(status, FileSystemStatus.False) {
  FTLog("Path is not a file.");
} else if Equals(status, FileSystemStatus.True) {
  FTLog("Path is a file.");
}
```

#### GetFile
```swift
public func GetFile(path: String) -> ref<File>;
public func GetAsyncFile(path: String) -> ref<AsyncFile>;
```

You can get a file to get stats:
```swift
// ...
let path = "README.md";
let file = storage.GetFile(path);
// Same usage with asynchronous mode:
// 
// let file = storage.GetAsyncFile(path);

if !IsDefined(file) {
  FTLogError("Operation denied.");
  return;
}
FTLog(s"Relative path: '\(file.GetPath())'");
FTLog(s"Absolute path: '\(file.GetAbsolutePath())'");
FTLog(s"Filename: '\(file.GetFilename())'");
FTLog(s"Extension: '\(file.GetExtension())'");
FTLog(s"Size: \(file.GetSize()) bytes");
// Relative path: 'README.md'
// Absolute path: 'C:\Program Files (x86)\Steam\steamapps\common\Cyberpunk 2077\r6\storages\Awesome\README.md'
// Filename: 'README.md'
// Extension: '.md'
// Size: 0 bytes
```

#### GetFiles
```swift
public func GetFiles() -> array<ref<File>>;
public func GetAsyncFiles() -> array<ref<AsyncFile>>;
```

You can list all files present in storage:
```swift
// ...
let files = storage.GetFiles();
// Same usage with asynchronous mode:
// 
// let files = storage.GetAsyncFiles();

for file in files {
  FTLog(s"Filename: '\(file.GetFilename())'");
}
```

> [!TIP]  
> It will only include regular files. Using `IsFile()` is redundant as it will 
> return `FileSystemStatus.true` for each item.

#### DeleteFile
```swift
public func DeleteFile(path: String) -> FileSystemStatus;
```

You can delete a file from storage:
```swift
// ...
let status = storage.DeleteFile("to_remove.txt");

if Equals(status, FileSystemStatus.Failure) {
  FTLogError("Operation failed.");
} else if Equals(status, FileSystemStatus.Denied) {
  FTLogError("Operation denied.");
} else if Equals(status, FileSystemStatus.True) {
  FTLog("File removed.");
}
```

### Read text
```swift
// File
public func ReadAsText() -> String;
public func ReadAsLines() -> array<String>;

// AsyncFile
public func ReadAsText(promise: FilePromise) -> Void;
public func ReadAsLines(promise: FilePromise) -> Void;
```

You can read all text content of a `File` using:

- A single blob stored in a `String`.

```swift
// ...
let file = storage.GetFile("README.md");
let text = file.ReadAsText();

FTLog(s"File '\(file.GetFilename())' contains \(StrLen(text)) characters:");
FTLog(text);
```

- A list of lines stored in an array of `String`.

```swift
// ...
let file = storage.GetFile("README.md");
let lines = file.ReadAsLines();
let linesSize = ArraySize(lines);
let i = 0;

FTLog(s"File '\(file.GetFilename())' contains \(linesSize) lines:");
while i < linesSize {
  FTLog(s"Line \(i + 1): \(lines[i])");
  i += 1;
}
```

### Write text
```swift
// File
public func WriteText(text: String,
                      opt mode: FileSystemWriteMode = Truncate) -> Bool;
public func WriteLines(lines: array<String>,
                       opt mode: FileSystemWriteMode = Truncate) -> Bool;

// AsyncFile
public func WriteText(promise: FilePromise,
                      text: String,
                      opt mode: FileSystemWriteMode) -> Void;
public func WriteLines(promise: FilePromise,
                       lines: array<String>,
                       opt mode: FileSystemWriteMode) -> Void;
```

You can write text in a `File` which already exists or create the file in the 
same time. When writing, you can truncate the file or append text to the end 
of the file. You can use:

- A single blob with a `String`.

```swift
// ...
// 
// Suppose file doesn't exist yet.
let file = storage.GetFile("welcome.txt");
let text = "";

text += "Welcome to Night City!\n";
text += "Let's do this choom ;)\n";

// Create file and write text in it (default is FileSystemWriteMode.Truncate).
let status = file.WriteText(text);

if !status {
  FTLogError(s"Failed to write in file '\(file.GetFilename())'.");
} else {
  FTLog(s"Wrote text in file '\(file.GetFilename())'.");
}
```

- A list of lines with an array of `String`.

```swift
// ...
// 
// Suppose file already exists.
let file = storage.GetFile("welcome.txt");
let lines = [
  "",
  "Beware of flatlines..."
];

// Only append lines to the end of the file.
let status = file.WriteLines(lines, FileSystemWriteMode.Append);

if !status {
  FTLogError(s"Failed to write in file '\(file.GetFilename())'.");
} else {
  FTLog(s"Wrote text in file '\(file.GetFilename())'.");
}
```

After running the two code snippets above, file should contain:
> Welcome to Night City!  
> Let's do this choom ;)
> 
> Beware of flatlines...

### Read raw data
```swift
// File
public func ReadAsBase64() -> String;

// AsyncFile
public func ReadAsBase64(promise: FilePromise) -> Void;
```

You can read binary data (e.g. PNG file) as an encoded Base64 string.
```swift
let file = FileSystem.GetSharedStorage().GetFile("screenshot.png");
let data = file.ReadAsBase64();
```

> [!NOTE]
> This feature was introduced in `v0.13.0`. This version was not released on
> NexusMod on purpose. It might be published later with a future update.

### Read Json
```swift
// File
public func ReadAsJson() -> ref<JsonVariant>;

// AsyncFile
public func ReadAsJson(promise: FilePromise) -> Void;
```

e.g. with Json file:
```json
{
  "name": "RedFileSystem",
  "version": 1,
  "isEnabled": true,
  "pi": 3.14159265358979323846,
  "items": [2077, 13.37, true, "Cyberpunk"],
  "i18n": {
    "en-US": "Hello",
    "fr-FR": "Bonjour"
  }
}
```

You can read all Json content of a `File` like this:
```swift
// ...
let file = storage.GetFile("config.json");
let json = file.ReadAsJson();

if !IsDefined(json) {
  FTLogError(s"Failed to parse Json of file '\(file.GetFilename())'.");
  return;
}
if !json.IsObject() {
  FTLogError(s"Expect root of Json document to be an object.");
  return;
}
// ...
```

### Write Json
```swift
// File
public func WriteJson(json: ref<JsonVariant>, opt indent: String) -> Bool;

// AsyncFile
public func WriteJson(promise: FilePromise,
                      json: ref<JsonVariant>,
                      opt indent: String) -> Void;
```

You can write Json in a `File` which already exists or create the file in the
same time. You can output pretty Json using `indent` argument, default is 
minified. When writing Json, the file is always truncated:

```swift
// ...
let json = new JsonObject();

json.SetKeyString("name", "RedFileSystem");
json.SetKeyBool("isEnabled", true);
json.SetKeyDouble("version", 2.12);
let file = storage.GetFile("data.json");
let status = file.WriteJson(json);
// Same as:
// let status = file.WriteText(json.ToString());

if !status {
  FTLogError(s"Failed to write in file '\(file.GetFilename())'.");
} else {
  FTLog(s"Wrote Json in file '\(file.GetFilename())'.");
}
```

> [!TIP]  
> See [RedData] to learn more about Json data.

### Asynchronous mode
```swift
public struct FilePromise {
  public static func Create(target: wref<IScriptable>,
                            resolve: CName,
                            opt reject: CName,
                            opt data: array<Variant>) -> FilePromise;
}
```

You can run read/write operations in asynchronous mode. This allows you to 
delegate heavy operations in background. It prevents the game from freezing 
while data is being read/written. It is recommended when operating with big 
chunks of data. It should be used when running into performance issues 
otherwise.

You need to get an `AsyncFile` from a storage to operate in asynchronous mode:
```swift
// ...
let file = storage.GetAsyncFile("async.txt");
```

You need to define a `FilePromise` to execute your own function when an 
operation is finished, whether it was successful or not:
```swift
public class YourClass {
  // ...

  public func YourFunction() {
    // ...
    let file = storage.GetAsyncFile("async.txt");
    let promise = FilePromise.Create(this, n"OnRead", n"OnReadFailed", ["FYI"]);
    //                                                ^
    //                                                | It is not required to 
    //                                                | define this callback.

    file.ReadAsText(promise);
  }

  // Callback when read is done.
  // 
  //                     | Argument's type is the same as the return's 
  //                     | type of the synchronous version.
  //                     v
  private cb func OnRead(text: String, data: array<Variant>) {
    let arg0: String = FromVariant(data[0]); // == "FYI"

    FTLog(s"text in file: \(text)");
  }

  // Callback when read failed.
  //                           
  //                           | Argument is only present when you define an 
  //                           | array of data with FilePromise.Create(...).
  //                           v
  private cb func OnReadFailed(data: array<Variant>) {
    let arg0: String = FromVariant(data[0]); // == "FYI"

    FTLogError("Failed to read in file!");
  }

  // ...
}
```

You can call read/write methods with the same arguments used in the 
synchronous mode. You only need to insert a `FilePromise` as the first 
argument.

When callback is executed for write operations, your function is only executed 
with optional arguments (array of Variant) when provided, no boolean value is 
to be expected (unlike in the synchronous version).

# Development
Contributions are welcome, feel free to fill an issue or a PR.

## Usage
1. Install requirements:
  - CMake v3.27+
  - Visual Studio Community 2022+
  - [red-cli] v0.4.0+
2. Configure project with:
```shell
cmake -G "Visual Studio 17 2022" -A x64 -S . -B build
```

3. Build in debug mode:
```shell
cmake --build build --target RedFileSystem --config Debug
```

It will execute `red-cli install` for you using a CMake custom command.

## Tests
1. Run game.
2. Open CET, show Game Log popup.
3. Output should show tests result.

## Release
1. Build in release mode:

```shell
cmake --build build --target RedFileSystem --config Release
```

It will execute `red-cli pack` for you using a CMake custom command. You should
find an archive `RedFileSystem-X.Y.Z.zip` in the root directory, ready to 
release.

<!-- Table of links -->
[RED4ext]: https://github.com/WopsS/RED4ext
[Redscript]: https://github.com/jac3km4/redscript
[Cyber Engine Tweaks]: https://github.com/maximegmd/CyberEngineTweaks
[latest archive]: https://github.com/rayshader/cp2077-red-filesystem/releases/latest
[Codeware]: https://github.com/psiberx/cp2077-codeware/wiki#lifecycle
[RedData]: https://github.com/rayshader/cp2077-red-data
[examples/]: https://github.com/rayshader/cp2077-red-filesystem/tree/master/examples
[branch api]: https://github.com/rayshader/cp2077-red-filesystem/tree/api
[red-cli]: https://github.com/rayshader/cp2077-red-cli/releases/latest