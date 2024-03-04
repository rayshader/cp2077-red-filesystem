-- Copy file into bin\x64\plugins\cyber_engine_tweaks\mods\AwesomeExample\

local function ExampleText(storage)
  print()
  print("== Text ==")
  local file = storage:GetFile("test.txt")

  if file == nil then
    print("Access to file denied.")
    return
  end
  local status = file:WriteText("Welcome to Night City!\n")

  if not status then
    print("Failed to write in file.")
    return;
  end
  local lines = {
    "Let's do this choom ;)",
    "",
    "Beware of flatlines..."
  }

  status = file:WriteLines(lines, FileSystemWriteMode.Append)
  if not status then
    print("Failed to write in file.")
    return;
  end
  local lines = file:ReadAsLines()

  print("File '" .. file:GetFilename() .. "' contains:")
  for _, line in ipairs(lines) do
    print(line)
  end
end

local function ExampleJson(storage)
  print()
  print("== Json ==")
  local file = storage:GetFile("test.json")

  if file == nil then
    print("Access to JSON file denied.")
    return
  end
  local json = JsonObject.new()

  json:SetKeyString("name", "Awesome")
  json:SetKeyInt64("version", 1)
  json:SetKeyBool("isEnabled", true)
  json:SetKeyNull("data")
  local status = file:WriteJson(json)

  if not status then
    print("Failed to write in JSON file.")
    return;
  end
  json = file:ReadAsJson() -- as JsonObject
  if json == nil then
    print("Cannot parse JSON file.")
    return
  end
  print("File '" .. file:GetFilename() .. "' contains:")
  print(json:ToString())
end

registerForEvent('onInit', function()
  print("== RedFS - Examples ==")
  local storage = FileSystem.GetStorage("Awesome")

  ExampleText(storage)
  ExampleJson(storage)
  print("== RedFS - Examples ==")
end)