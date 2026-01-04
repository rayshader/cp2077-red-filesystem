#ifndef REDFILESYSTEM_UTILS_H
#define REDFILESYSTEM_UTILS_H

#include <memory>
#include <mutex>
#include <string>

namespace RedFS {

using SharedMutex = std::shared_ptr<std::mutex>;

inline std::string to_lower(const std::string& p_str) {
  std::string str = p_str;
  std::ranges::transform(str, str.begin(),
                         [](const int c) {
                           return std::tolower(c);
                         });
  return str;
}

inline bool equals_insensitive(const std::string& p_a, const std::string& p_b) {
  return std::ranges::equal(p_a, p_b, [](const int a, const int b) -> bool {
    return std::tolower(a) == std::tolower(b);
  });
}

}  // namespace RedFS

#endif  //REDFILESYSTEM_UTILS_H
