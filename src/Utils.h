#ifndef REDFILESYSTEM_UTILS_H
#define REDFILESYSTEM_UTILS_H

#include <memory>
#include <mutex>
#include <string>

namespace RedFS {

using SharedMutex = std::shared_ptr<std::mutex>;

static bool equals_insensitive(const std::string& p_a, const std::string& p_b) {
  return std::ranges::equal(p_a, p_b, [](char a, char b) -> bool {
    return std::tolower(a) == std::tolower(b);
  });
}

}  // namespace RedFS

#endif  //REDFILESYSTEM_UTILS_H
