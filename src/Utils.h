#ifndef REDFILESYSTEM_UTILS_H
#define REDFILESYSTEM_UTILS_H

#include <memory>
#include <mutex>

namespace RedFS {

using SharedMutex = std::shared_ptr<std::mutex>;

}

#endif  //REDFILESYSTEM_UTILS_H
