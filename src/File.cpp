#include "File.h"
#include "JsonArray.h"
#include "JsonBool.h"
#include "JsonDouble.h"
#include "JsonInt64.h"
#include "JsonNull.h"
#include "JsonObject.h"
#include "JsonString.h"
#include "JsonVariant.h"

#include <simdjson.h>
#include <fstream>
#include <utility>

#include <RED4ext/RED4ext.hpp>
#include <RedLib.hpp>

namespace RedFS {

File::File(std::filesystem::path p_path, std::filesystem::path p_absolute_path)
    : path(std::move(p_path)), absolute_path(std::move(p_absolute_path)) {}

Red::CString File::get_path() const {
  return path.string();
}

Red::CString File::get_absolute_path() const {
  return absolute_path.string();
}

Red::CString File::get_filename() const {
  return path.filename().string();
}

Red::CString File::get_extension() const {
  return path.extension().string();
}

uint64_t File::get_size() const {
  std::error_code error;

  return std::filesystem::file_size(absolute_path, error);
}

Red::CString File::read_as_text() {
  std::ifstream stream;

  stream.open(absolute_path);
  if (!stream.is_open()) {
    return {};
  }
  std::stringstream data;

  data << stream.rdbuf();
  stream.close();
  return data.str();
}

Red::DynArray<Red::CString> File::read_as_lines() {
  std::ifstream stream;

  stream.open(absolute_path);
  if (!stream.is_open()) {
    return {};
  }
  Red::DynArray<Red::CString> lines;
  std::string line;

  while (std::getline(stream, line)) {
    if (stream.fail() || stream.bad()) {
      break;
    }
    lines.EmplaceBack(line);
  }
  stream.close();
  return lines;
}

Red::Handle<JsonVariant> File::read_as_json() {
  simdjson::dom::parser parser;
  auto document = parser.load(absolute_path.string());

  if (document.is_object()) {
    auto object = simdjson::dom::object(document);
    auto root = Red::MakeHandle<JsonObject>();

    parse_object(object, root);
    return root;
  } else if (document.is_array()) {
    auto array = simdjson::dom::array(document);
    auto root = Red::MakeHandle<JsonArray>();

    parse_array(array, root);
    return root;
  }
  return {};
}

bool File::write_text(const Red::CString& p_text,
                      const Red::Optional<FileSystemWriteMode>& p_mode) {
  std::ios_base::openmode mode = get_mode(p_mode.value);
  std::ofstream stream;

  stream.open(absolute_path, mode);
  if (!stream.is_open()) {
    return false;
  }
  stream << p_text.c_str();
  stream.close();
  return true;
}

bool File::write_lines(const Red::DynArray<Red::CString>& p_lines,
                       const Red::Optional<FileSystemWriteMode>& p_mode) {
  std::ios_base::openmode mode = get_mode(p_mode.value);
  std::ofstream stream;

  stream.open(absolute_path, mode);
  if (!stream.is_open()) {
    return false;
  }
  for (uint32_t i = 0; i < p_lines.size; i++) {
    stream << p_lines[i].c_str();
    if (i + 1 < p_lines.size) {
      stream << '\n';
    }
  }
  stream.close();
  return true;
}

void File::parse_object(const simdjson::dom::object& p_object,
                        Red::Handle<JsonObject>& p_root) {
  for (const auto& key_value : p_object) {
    Red::CName key(std::string(key_value.key).c_str());

    if (key_value.value.is_bool()) {
      auto value = Red::MakeHandle<JsonBool>(bool(key_value.value));

      p_root->insert_field(key, value);
    } else if (key_value.value.is_int64()) {
      auto value = Red::MakeHandle<JsonInt64>(int64_t(key_value.value));

      p_root->insert_field(key, value);
    } else if (key_value.value.is_double()) {
      auto value = Red::MakeHandle<JsonDouble>(double(key_value.value));

      p_root->insert_field(key, value);
    } else if (key_value.value.is_string()) {
      auto value = Red::MakeHandle<JsonString>(std::string(key_value.value));

      p_root->insert_field(key, value);
    } else if (key_value.value.is_object()) {
      auto value = Red::MakeHandle<JsonObject>();
      auto sub_object = simdjson::dom::object(key_value.value);

      parse_object(sub_object, value);
      p_root->insert_field(key, value);
    } else if (key_value.value.is_array()) {
      auto value = Red::MakeHandle<JsonArray>();
      auto array = simdjson::dom::array(key_value.value);

      parse_array(array, value);
      p_root->insert_field(key, value);
    } else if (key_value.value.is_null()) {
      p_root->insert_field(key, Red::MakeHandle<JsonNull>());
    } else {
      p_root->insert_field(key, Red::MakeHandle<JsonVariant>());
    }
  }
}

void File::parse_array(const simdjson::dom::array& p_array,
                       Red::Handle<JsonArray>& p_root) {
  for (const auto& item : p_array) {
    if (item.is_bool()) {
      auto value = Red::MakeHandle<JsonBool>(bool(item));

      p_root->push_back(value);
    } else if (item.is_int64()) {
      auto value = Red::MakeHandle<JsonInt64>(int64_t(item));

      p_root->push_back(value);
    } else if (item.is_double()) {
      auto value = Red::MakeHandle<JsonDouble>(double(item));

      p_root->push_back(value);
    } else if (item.is_string()) {
      auto value = Red::MakeHandle<JsonString>(std::string(item));

      p_root->push_back(value);
    } else if (item.is_object()) {
      auto value = Red::MakeHandle<JsonObject>();
      auto sub_object = simdjson::dom::object(item);

      parse_object(sub_object, value);
      p_root->push_back(value);
    } else if (item.is_array()) {
      auto value = Red::MakeHandle<JsonArray>();
      auto sub_array = simdjson::dom::array(item);

      parse_array(sub_array, value);
      p_root->push_back(value);
    } else if (item.is_null()) {
      p_root->push_back(Red::MakeHandle<JsonNull>());
    } else {
      p_root->push_back(Red::MakeHandle<JsonVariant>());
    }
  }
}

std::ios_base::openmode File::get_mode(FileSystemWriteMode p_mode) {
  std::ios_base::openmode mode = std::ios_base::trunc;

  if (p_mode == FileSystemWriteMode::Append) {
    mode = std::ios_base::app;
  }
  return mode;
}

}  // namespace RedFS