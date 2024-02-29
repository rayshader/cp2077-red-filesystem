#include "JsonString.h"

#include <regex>
#include <utility>
#include <vector>

namespace RedFS {

// Partially implement RFC7159
// - \uXXXX is missing
// See https://www.rfc-editor.org/rfc/rfc7159#section-7
std::vector<JsonString::EscapeRule> JsonString::escape_rules = {
  std::make_pair("\\\\", "\\\\"), std::make_pair("\"", "\\\""),
  std::make_pair("/", "\\/"),     std::make_pair("\b", "\\b"),
  std::make_pair("\f", "\\f"),    std::make_pair("\n", "\\n"),
  std::make_pair("\r", "\\r"),    std::make_pair("\t", "\\t")};

JsonString::JsonString() {
  type = JsonType::String;
}

JsonString::JsonString(std::string p_value) : value(std::move(p_value)) {
  type = JsonType::String;
}

Red::CString JsonString::get_string() const {
  return value;
}

Red::CString JsonString::to_string() const {
  std::string escaped_value = value;

  for (const auto& rule : escape_rules) {
    auto regex = std::regex(rule.first);

    escaped_value = std::regex_replace(escaped_value, regex, rule.second);
  }
  return "\"" + escaped_value + "\"";
}

}  // namespace RedFS