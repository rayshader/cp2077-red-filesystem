# Red File System
![Cyberpunk 2077](https://img.shields.io/badge/Cyberpunk%202077-v2.12a-blue)
![GitHub License](https://img.shields.io/github/license/rayshader/cp2077-red-filesystem)
[![Donate](https://img.shields.io/badge/donate-buy%20me%20a%20coffee-yellow)](https://www.buymeacoffee.com/lpfreelance)

This plugin provides access to the file system using read/write operations. It 
supports UTF8 text and Json formats. It can be used with Redscript and CET.

# Getting started

## Compatibility
- Cyberpunk 2077 v2.12a
- [Redscript] v0.5.19+
- [Cyber Engine Tweaks] v1.32.2+

## Installation
1. Install requirements:
  - [RED4ext] v1.24.3+
  - [RedData] v0.4.0+
2. Extract the [latest archive] into the Cyberpunk 2077 directory.

## API wrapper

This plugin can be used when writing a RED4ext plugin with RedLib. You can
find a C++ API wrapper in [branch api].

## Usage

### Disclaimer
All read/write operations are restricted within:
> ...\Cyberpunk 2077\red4ext\plugins\RedFileSystem\storages\

You can get a storage with the name of your mod. It must be unique. You can 
only read/write files within your own storage.

For example, if your mod is named `Awesome`, your storage will be located in:
> ...\Cyberpunk 2077\red4ext\plugins\RedFileSystem\storages\Awesome\

This is a security measure to prevent malicious access. This way, you and 
other authors cannot break game files nor the operating system of the player.

All features are defined in module `RedFileSystem`. You need to import it with:
```swift
import RedFileSystem.*
``` 

### FileSystem

#### GetStorage
> GetStorage(name: String) -> ref&lt;FileSystemStorage&gt;

You can get a storage for your mod like this:

```swift
let storage = FileSystem.GetStorage("Awesome");

// ...
```

`name` must complies with the following rules:
- minimum of 3 characters
- maximum of 24 characters
- only `A-Z` and `a-z` characters

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

You can get a file to get stats:
```swift
// ...
let path = "README.md";
let file = storage.GetFile(path);

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
// Absolute path: 'C:\Program Files (x86)\Steam\steamapps\common\Cyberpunk 2077\red4ext\plugins\RedFileSystem\storages\Awesome\README.md'
// Filename: 'README.md'
// Extension: '.md'
// Size: 0 bytes
```

#### GetFiles
> GetFiles() -> array&lt;ref&lt;File&gt;&gt;

You can list all files present in storage:
```swift
// ...
let files = storage.GetFiles();

for file in files {
  LogChannel(n"Info", s"Filename: '\(file.GetFilename())'");
}
```

> [!TIP]  
> It will only include regular files. Using `IsFile()` is redundant as it will 
> return `FileSystemStatus.true` for each item.

### Read text
> ReadAsText() -> String  
> ReadAsLines() -> array&lt;String&gt;

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

# Development
Contributions are welcome, feel free to fill an issue or a PR.

## Usage
1. Install requirements:
  - CMake v3.27+
  - Visual Studio Community 2022+
  - Node JS v20.11+
    - run `npm install --save-dev archiver`
2. Configure project with:
```shell
cmake -G "Visual Studio 17 2022" -A x64 -S . -B build
```

3. Build in debug mode:
```shell
cmake --build build --target RedFileSystem --config Debug
```

## Tests
1. Install in your game directory:

```shell
node install.mjs
```
 
2. Run game.
3. Open CET, show Game Log popup.
4. Output should show tests result.

## Release
1. Build in release mode:

```shell
cmake --build build --target RedFileSystem --config Release
```

2. Bundle release:

```shell
node bundle.mjs
```

<!-- Table of links -->
[RED4ext]: https://github.com/WopsS/RED4ext
[Redscript]: https://github.com/jac3km4/redscript
[Cyber Engine Tweaks]: https://github.com/maximegmd/CyberEngineTweaks
[latest archive]: https://github.com/rayshader/cp2077-red-filesystem/releases/latest
[Codeware]: https://github.com/psiberx/cp2077-codeware/wiki#lifecycle
[RedData]: https://github.com/rayshader/cp2077-red-data
[examples/]: https://github.com/rayshader/cp2077-red-filesystem/tree/master/examples
[branch api]: https://github.com/rayshader/cp2077-red-filesystem/tree/api