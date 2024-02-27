# Red File System

This plugin provides access to the file system using read/write operations. It 
supports UTF8 text and Json formats. It can be used with Redscript and CET.

# Getting started

## Compatibility
- Cyberpunk 2077 v2.11
- [Redscript] 0.5.16+
- [Cyber Engine Tweaks] 1.30.0+

## Installation
1. Install requirements:
  - [RED4ext] v1.22.0+
2. Extract the [latest archive] into the Cyberpunk 2077 directory.

## Usage

### Disclaimer
All operations are restricted to the game directory. You will not be able to 
read/write files outside the game directory. This is a security measure. Worst 
case scenario, you can corrupt the game files, nothing else.

### Relative path
All paths are relative to the game directory by default 
(`FileSystemPrefix.None`).
> ...\Cyberpunk 2077\

Make all paths relative to CET `mods` directory with `FileSystemPrefix.CET`.
> ...\Cyberpunk 2077\bin\x64\cyber_engine_tweaks\mods\

Make all paths relative to `scripts` directory with 
`FileSystemPrefix.Redscript`.
> ...\Cyberpunk 2077\r6\scripts\

You can read/write only in your own mod directory with `FileSystemPrefix` and 
your mod's directory.
> path: "MyMod\\"

Path will be automatically resolved to:
> CET: ...\Cyberpunk 2077\bin\x64\cyber_engine_tweaks\mods\MyMod\
> Redscript: ...\Cyberpunk 2077\r6\scripts\MyMod\

### Exists
> Exists(path: String, opt prefix: FileSystemPrefix = None) -> FileSystemStatus

You can test if a path exists, can be a file or a directory (e.g. game 
directory):
```swift
let path: String = "launcher-configuration.json";
let status: FileSystemStatus = FileSystem.Exists(path);

if Equals(status, FileSystemStatus.Failure) {
  LogChannel(n"Error", "System error.");
} else if Equals(status, FileSystemStatus.Denied) {
  LogChannel(n"Error", "Path is outside the game directory.");
} else if Equals(status, FileSystemStatus.False) {
  LogChannel(n"Info", "File/Directory not found.");
} else if Equals(status, FileSystemStatus.True) {
  LogChannel(n"Info", "File/Directory is present.");
}
```

### IsFile
> IsFile(path: String, opt prefix: FileSystemPrefix = None) -> FileSystemStatus

You can test if a path points to a regular file (e.g. game directory):
```swift
let path: String = "README.md";
let status: FileSystemStatus = FileSystem.IsFile(path);

if Equals(status, FileSystemStatus.Failure) {
  LogChannel(n"Error", "System error.");
} else if Equals(status, FileSystemStatus.Denied) {
  LogChannel(n"Error", "Path is outside of the game directory.");
} else if Equals(status, FileSystemStatus.False) {
  LogChannel(n"Info", "Path is a not file.");
} else if Equals(status, FileSystemStatus.True) {
  LogChannel(n"Info", "Path is a file.");
}
```

### GetFile
> GetFile(path: String, opt prefix: FileSystemPrefix = None) -> ref<File>

You can get a file to get stats (e.g. Redscript mod directory):
```swift
let path: String = "MyMod\\README.md";
let file: ref<File> = FileSystem.GetFile(path, FileSystemPrefix.Redscript);

if !IsDefined(file) {
  LogChannel(n"Error", "Path is outside of the Redscript mod directory.");
  return;
}
LogChannel(n"Info", s"Relative path: '\(file.GetPath())'");
LogChannel(n"Info", s"Absolute path: '\(file.GetAbsolutePath())'");
LogChannel(n"Info", s"Filename: '\(file.GetFilename())'");
LogChannel(n"Info", s"Extension: '\(file.GetExtension())'");
LogChannel(n"Info", s"Size: \(file.GetSize()) bytes");
// Relative path: 'MyMod\\README.md'
// Absolute path: 'C:\Program Files (x86)\Steam\steamapps\common\Cyberpunk 2077\r6\scripts\MyMod\README.md'
// Filename: 'README.md'
// Extension: '.md'
// Size: 0 bytes
```

### Read text
> ReadAsText() -> String
> ReadAsLines() -> array<String>

You can read all text content of a `File` using:

- A single blob stored in a `String`.

```swift
let file: ref<File> = FileSystem.GetFile("MyMod\\README.md", FileSystemPrefix.Redscript);
let text: String = file.ReadAsText();

LogChannel(n"Info", s"File '\(file.GetFilename())' contains \(StrLen(text)) characters:");
LogChannel(n"Info", text);
```

- A list of lines stored in an array of `String`.

```swift
let file: ref<File> = FileSystem.GetFile("MyMod\\README.md", FileSystemPrefix.Redscript);
let lines: array<String> = file.ReadAsLines();
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
> WriteLines(lines: array<String>, opt mode: FileSystemWriteMode = Truncate) -> Bool 

You can write text in a `File` which already exists or create the file in the 
same time. When writing, you can truncate the file or append text to the end 
of the file. You can use:

- A single blob with a `String`.

```swift
// Suppose file doesn't exist yet.
let file: ref<File> = FileSystem.GetFile("MyMod\\welcome.txt", FileSystemPrefix.Redscript);
let text: String = "";

text += "Welcome Night City!\n";
text += "Lets do this choom ;)\n";

// Create file and write text in it (default is FileSystemWriteMode.Truncate).
let status: Bool = file.WriteText(text);

if !status {
  LogChannel(n"Error", s"Failed to write in file '\(file.GetFilename())'.");
} else {
  LogChannel(n"Info", s"Wrote text in file '\(file.GetFilename())'.");
}
```

- A list of lines with an array of `String`.

```swift
// Suppose file already exists.
let file: ref<File> = FileSystem.GetFile("MyMod\\welcome.txt", FileSystemPrefix.Redscript);
let lines: array<String> = [
  "",
  "Beware of flatlines..."
];

// Only append lines to the end of the file.
let status: Bool = file.WriteLines(lines, FileSystemWriteMode.Append);

if !status {
  LogChannel(n"Error", s"Failed to write in file '\(file.GetFilename())'.");
} else {
  LogChannel(n"Info", s"Wrote text in file '\(file.GetFilename())'.");
}
```

After running the two code snippets above, file should contain:
> Welcome Night City!
> Lets do this choom ;)
> 
> Beware of flatlines...

### Read Json
> ReadAsJson() -> ref<JsonVariant>

e.g. with Json file:
```json
{
  "name": "RedFS",
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
let file: ref<File> = FileSystem.GetFile("MyMod\\config.json", FileSystemPrefix.Redscript);
let json: ref<JsonVariant> = file.ReadAsJson();

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

#### JsonVariant
> IsUndefined() -> Bool
> IsNull() -> Bool
>
> IsBool() -> Bool
> IsInt64() -> Bool
> IsDouble() -> Bool
> IsString() -> Bool
>
> IsArray() -> Bool
> IsObject() -> Bool

#### JsonObject
> GetKeys() -> array<String>
> GetValues() -> array<ref<JsonVariant>>
>
> HasKey(key: String) -> Bool
> GetKey(key: String) -> ref<JsonVariant>
>
> GetKeyBool(key: String) -> Bool
> GetKeyInt64(key: String) -> Int64
> GetKeyDouble(key: String) -> Double
> GetKeyString(key: String) -> String

Get values:
```swift
// ...
let obj: ref<JsonObject> = json as JsonObject;
let name = obj.GetKeyString("name");
let version = obj.GetKeyInt64("version");
let isEnabled = obj.GetKeyBool("isEnabled");
let pi = obj.GetKeyDouble("pi");
// Same as:
// let name = obj.GetKey("name").GetString();
// let version = obj.GetKey("version").GetInt64();
// let isEnabled = obj.GetKey("isEnabled").GetBool();
// let pi = obj.GetKey("pi").GetDouble();
```

Get a list of keys:
```swift
// ...
let keys: array<String> = obj.GetKeys();

// keys == ["name", "version", "isEnabled", "pi", "items", "i18n"]
```

Get a list of values:
```swift
// ...
let values: array<ref<JsonVariant>> = obj.GetValues();

// values[0].GetString() == "RedFS"
// values[1].GetInt64() == 1
// values[2].GetBool() == true
// values[3].GetDouble() == 3.14159265358979323846
// values[4] == [...]
// values[5] == {...}
```

Test whether a key is present:
```swift
// ...
let hasName: Bool = obj.HasKey("name");
let hasUnknown: Bool = obj.HasKey("unknown");

// hasName == true
// hasUnknown == false
```

#### JsonArray
> GetSize() -> Uint32
> GetItem(index: Uint32) -> ref<JsonVariant>
> 
> GetItemBool(index: Uint32) -> Bool
> GetItemInt64(index: Uint32) -> Int64
> GetItemDouble(index: Uint32) -> Double
> GetItemString(index: Uint32) -> String

Get items:
```swift
// ...
let items: ref<JsonArray> = obj.GetKey("items") as JsonArray;
let year = items.GetItemInt64(0u);
let elite = items.GetItemDouble(1u);
let theWay = items.GetItemBool(2u);
let title = items.GetItemString(3u);
// Same as:
// let year = items.GetItem(0u).GetInt64();
// let elite = items.GetItem(1u).GetDouble();
// let theWay = items.GetItem(2u).GetBool();
// let title = items.GetItem(3u).GetString();
```

Get the number of items:
```swift
let size: Uint32 = items.GetSize();
```

# Development
Contributions are welcome, feel free to fill an issue or a PR.

## Setup


## Tests


<!-- Table of links -->
[RED4ext]: https://github.com/WopsS/RED4ext
[Redscript]: https://github.com/jac3km4/redscript
[Cyber Engine Tweaks]: https://github.com/maximegmd/CyberEngineTweaks
[latest archive]: https://github.com/rayshader/cp2077-redfs/releases/latest