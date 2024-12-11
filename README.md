# Red File System
![Cyberpunk 2077](https://img.shields.io/badge/Cyberpunk%202077-v2.2-blue)
[![MO2](https://img.shields.io/badge/MO2-compatible-green)](https://github.com/ModOrganizer2/modorganizer/)
![GitHub License](https://img.shields.io/github/license/rayshader/cp2077-red-filesystem)
[![Donate](https://img.shields.io/badge/donate-buy%20me%20a%20coffee-yellow)](https://www.buymeacoffee.com/lpfreelance)

This plugin provides access to the file system using read/write operations. It 
supports UTF8 text and Json formats. It can be used with Redscript and CET.

# Getting started

## Compatibility
- Cyberpunk 2077 v2.2
- [Redscript] v0.5.27+
- [Cyber Engine Tweaks] v1.34.0+

## Installation
1. Install requirements:
  - [RED4ext] v1.26.0+
  - [RedData] v0.6.0+ (only required for Json data)
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
All read/write operations are restricted within:
> Cyberpunk 2077\r6\storages\

> [!NOTE]  
> Old path was: `Cyberpunk 2077\red4ext\plugins\RedFileSystem\storages\`. Its 
> content will automatically be migrated to `Cyberpunk 2077\r6\storages\`.

You can get a storage with the name of your mod. It must be unique. You can 
only read/write files within your own storage.

For example, if your mod is named `Awesome`, your storage will be located in:
> Cyberpunk 2077\r6\storages\Awesome\

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
> GetStorage(name: String) -> ref&lt;FileSystemStorage&gt;

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
`ScriptableEnv` from [Codeware]. You can find an example in [examples/].

#### GetSharedStorage
> GetSharedStorage() -> ref&lt;FileSystemStorage&gt;

You can use it to read/write in a common storage. It can be accessed without 
restrictions if you need to share data with other mods. It is located in:
> Cyberpunk 2077\r6\storages\shared\

### FileSystemStorage

#### Exists
> Exists(path: String) -> FileSystemStatus

You can test if a path exists, it can be a file or a directory:
```swift
// ...
let path = "config.json";
let status = storage.Exists(path);

if Equals(status, FileSystemStatus.Failure) {
  LogChannel(n"Error", "System error.");
} else if Equals(status, FileSystemStatus.Denied) {
  LogChannel(n"Error", "Operation denied.");
} else if Equals(status, FileSystemStatus.False) {
  LogChannel(n"Info", "File/Directory not found.");
} else if Equals(status, FileSystemStatus.True) {
  LogChannel(n"Info", "File/Directory is present.");
}
```

#### IsFile
> IsFile(path: String) -> FileSystemStatus

You can test if a path points to a regular file:
```swift
// ...
let path = "README.md";
let status = storage.IsFile(path);

if Equals(status, FileSystemStatus.Failure) {
  LogChannel(n"Error", "System error.");
} else if Equals(status, FileSystemStatus.Denied) {
  LogChannel(n"Error", "Operation denied.");
} else if Equals(status, FileSystemStatus.False) {
  LogChannel(n"Info", "Path is not a file.");
} else if Equals(status, FileSystemStatus.True) {
  LogChannel(n"Info", "Path is a file.");
}
```

#### GetFile
> GetFile(path: String) -> ref&lt;File&gt;  
> GetAsyncFile(path: String) -> ref&lt;AsyncFile&gt;

You can get a file to get stats:
```swift
// ...
let path = "README.md";
let file = storage.GetFile(path);
// Same usage with asynchronous mode:
// 
// let file = storage.GetAsyncFile(path);

if !IsDefined(file) {
  LogChannel(n"Error", "Operation denied.");
  return;
}
LogChannel(n"Info", s"Relative path: '\(file.GetPath())'");
LogChannel(n"Info", s"Absolute path: '\(file.GetAbsolutePath())'");
LogChannel(n"Info", s"Filename: '\(file.GetFilename())'");
LogChannel(n"Info", s"Extension: '\(file.GetExtension())'");
LogChannel(n"Info", s"Size: \(file.GetSize()) bytes");
// Relative path: 'README.md'
// Absolute path: 'C:\Program Files (x86)\Steam\steamapps\common\Cyberpunk 2077\r6\storages\Awesome\README.md'
// Filename: 'README.md'
// Extension: '.md'
// Size: 0 bytes
```

#### GetFiles
> GetFiles() -> array&lt;ref&lt;File&gt;&gt;  
> GetAsyncFiles() -> array&lt;ref&lt;AsyncFile&gt;&gt;

You can list all files present in storage:
```swift
// ...
let files = storage.GetFiles();
// Same usage with asynchronous mode:
// 
// let files = storage.GetAsyncFiles();

for file in files {
  LogChannel(n"Info", s"Filename: '\(file.GetFilename())'");
}
```

> [!TIP]  
> It will only include regular files. Using `IsFile()` is redundant as it will 
> return `FileSystemStatus.true` for each item.

#### DeleteFile
> DeleteFile(path: String) -> FileSystemStatus

You can delete a file from storage:
```swift
// ...
let status = storage.DeleteFile("to_remove.txt");

if Equals(status, FileSystemStatus.Failure) {
  LogChannel(n"Error", "Operation failed.");
} else if Equals(status, FileSystemStatus.Denied) {
  LogChannel(n"Error", "Operation denied.");
} else if Equals(status, FileSystemStatus.True) {
  LogChannel(n"Info", "File removed.");
}
```

### Read text
> ReadAsText() -> String  
> ReadAsLines() -> array&lt;String&gt;  
>   
> ReadAsText(promise: FilePromise) -> Void  
> ReadAsLines(promise: FilePromise) -> Void

You can read all text content of a `File` using:

- A single blob stored in a `String`.

```swift
// ...
let file = storage.GetFile("README.md");
let text = file.ReadAsText();

LogChannel(n"Info", s"File '\(file.GetFilename())' contains \(StrLen(text)) characters:");
LogChannel(n"Info", text);
```

- A list of lines stored in an array of `String`.

```swift
// ...
let file = storage.GetFile("README.md");
let lines = file.ReadAsLines();
let linesSize = ArraySize(lines);
let i = 0;

LogChannel(n"Info", s"File '\(file.GetFilename())' contains \(linesSize) lines:");
while i < linesSize {
  LogChannel(n"Info", s"Line \(i + 1): \(lines[i])");
  i += 1;
}
```

### Write text
> WriteText(text: String, opt mode: FileSystemWriteMode = Truncate) -> Bool  
> WriteLines(lines: array&lt;String&gt;, opt mode: FileSystemWriteMode = Truncate) -> Bool  
>   
> WriteText(promise: FilePromise, text: String, opt mode: FileSystemWriteMode) -> Void  
> WriteLines(promise: FilePromise, lines: array&lt;String&gt;, opt mode: FileSystemWriteMode) -> Void

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
  LogChannel(n"Error", s"Failed to write in file '\(file.GetFilename())'.");
} else {
  LogChannel(n"Info", s"Wrote text in file '\(file.GetFilename())'.");
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
  LogChannel(n"Error", s"Failed to write in file '\(file.GetFilename())'.");
} else {
  LogChannel(n"Info", s"Wrote text in file '\(file.GetFilename())'.");
}
```

After running the two code snippets above, file should contain:
> Welcome to Night City!  
> Let's do this choom ;)
> 
> Beware of flatlines...

### Read Json
> ReadAsJson() -> ref&lt;JsonVariant&gt;  
>   
> ReadAsJson(promise: FilePromise) -> Void

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
  LogChannel(n"Error", s"Failed to parse Json of file '\(file.GetFilename())'.");
  return;
}
if !json.IsObject() {
  LogChannel(n"Error", s"Expect root of Json document to be an object.");
  return;
}
// ...
```

### Write Json
> WriteJson(json: ref&lt;JsonVariant&gt;, opt indent: String) -> Bool  
>   
> WriteJson(promise: FilePromise, json: ref&lt;JsonVariant&gt;, opt indent: String) -> Void

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
  LogChannel(n"Error", s"Failed to write in file '\(file.GetFilename())'.");
} else {
  LogChannel(n"Info", s"Wrote Json in file '\(file.GetFilename())'.");
}
```

> [!TIP]  
> See [RedData] to learn more about Json data.

### Asynchronous mode
> FilePromise.Create(target: wref&lt;IScriptable&gt;, resolve: CName, opt reject: CName, opt data: array&lt;Variant&gt;) -> FilePromise

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

    LogChannel(n"Info", s"text in file: \(text)");
  }

  // Callback when read failed.
  //                           
  //                           | Argument is only present when you define an 
  //                           | array of data with FilePromise.Create(...).
  //                           v
  private cb func OnReadFailed(data: array<Variant>) {
    let arg0: String = FromVariant(data[0]); // == "FYI"

    LogChannel(n"Error", "Failed to read in file!");
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
  - [red-cli] v0.2.2+
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