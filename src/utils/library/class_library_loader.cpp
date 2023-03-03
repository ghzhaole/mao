//
// Created by leo on 2023/3/1.
//

#ifndef WIN32

#include "class_library_loader.h"

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace mao::library {
classLibraryLoader::classLibraryLoader() { libraryHandle_ = nullptr; }

classLibraryLoader::~classLibraryLoader() { Free(); }

bool classLibraryLoader::Load(std::string str_utf8_dllfilepath) {
  libraryHandle_ = dlopen(str_utf8_dllfilepath.c_str(), RTLD_LOCAL | RTLD_LAZY);
  if (!libraryHandle_) {
    printf("error msg:%s", dlerror());
    return false;
  }
  return true;
}

void classLibraryLoader::Free() {
  if (libraryHandle_ != nullptr) {
    dlclose(libraryHandle_);
    libraryHandle_ = nullptr;
  }
}
}  // namespace mao::library

#else

#include <windows.h>

#include <codecvt>
#include <locale>

#include "class_library_loader.h"

namespace mao::library {
classLibraryLoader::classLibraryLoader() { libraryHandle_ = nullptr; }
classLibraryLoader::~classLibraryLoader() { Free(); }
bool classLibraryLoader::Load(std::string str_utf8_dllfilepath) {
  std::wstring_convert<std::codecvt_utf8<wchar_t> > conv;
  std::wstring strpath = conv.from_bytes(str_utf8_dllfilepath);
  libraryHandle_ =
      LoadLibraryExW(strpath.c_str(), NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
  if (libraryHandle_ == nullptr) {
    return false;
  }
  return true;
}

void classLibraryLoader::Free() {
  if (libraryHandle_ != nullptr) {
    ::FreeLibrary((HMODULE)libraryHandle_);
    libraryHandle_ = nullptr;
  }
}
}  // namespace mao::library

#endif
