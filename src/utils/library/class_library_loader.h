//
// Created by leo on 2023/3/1.
//

#ifndef MAO_SRC_UTILS_CLASS_LIBRARY_LOADER_H_
#define MAO_SRC_UTILS_CLASS_LIBRARY_LOADER_H_

#include <string>
#include <type_traits>
#include <utility>

namespace mao::library {
class classLibraryLoader {
 public:
  classLibraryLoader();

  ~classLibraryLoader();

 public:
  bool Load(std::string str_utf8_dllfilepath);

  void Free();

 private:
  void *libraryHandle_;
};
}  // namespace mao::library
#endif  // MAO_SRC_UTILS_CLASS_LIBRARY_LOADER_H_
