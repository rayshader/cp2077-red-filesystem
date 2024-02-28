local function exampleText()
  print()
  print("== Text ==")
  local path = "engine\\config\\base\\multiplayer.ini"
  local status = FileSystem.Exists(path)

  if status ~= FileSystemStatus.True then
    print("Cannot find INI file.")
    return
  end
  local file = FileSystem.GetFile(path)

  if file == nil then
    print("Access to INI file denied.")
    return
  end
  local lines = file:ReadAsLines()

  print("INI file '" .. file:GetFilename() .. "':")
  for _, line in ipairs(lines) do
    print(line)
  end
end

local function exampleJson()
  print()
  print("== Json ==")
  local path = "r6\\config\\settings\\platform\\pc\\options.json"
  local status = FileSystem.Exists(path)

  if status ~= FileSystemStatus.True then
    print("Cannot find JSON file.")
    return
  end
  local file = FileSystem.GetFile(path)

  if file == nil then
    print("Access to JSON file denied.")
    return
  end
  local json = file:ReadAsJson()

  if json == nil then
    print("Cannot parse JSON file.")
    return
  end
  local root = json -- as JsonObject

  if not root:HasKey("version") or not root:HasKey("groups") or not root:HasKey("options") then
    print("JSON schema is unknown.")
    return
  end
  local options = root:GetKey("options") -- as JsonArray
  local size = options:GetSize() - 1 -- Unlike arrays in Lua, you must iterate from 0 to N - 1.
  local names = {}
  local i = 0

  for i = 0, size do
    local option = options:GetItem(i) -- as JsonObject
    local name = option:GetKeyString("group_name")

    table.insert(names, name)
  end
  print("List of groups in game options:")
  for _, name in ipairs(names) do
    print(name)
  end
end

registerForEvent('onInit', function()
  print("== RedFS - Examples ==")
  exampleText()
  exampleJson()
  print("== RedFS - Examples ==")
end)