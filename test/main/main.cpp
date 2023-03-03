//
// Created by leo on 2023/2/28.
//
#include <iostream>
#include <sstream>

#include "reflection/class_factory.h"
#include "serialization/class_json_stream.h"
#include "utils/library/class_library_loader.h"

using namespace mao::library;
using namespace mao::reflection;

int main() {
  std::string dllfilepath = "./lib/libmao_data_d.dylib";
  auto ptr_shared_library_ = new classLibraryLoader();
  if (ptr_shared_library_->Load(dllfilepath) == false) {
    return 1;
  }
  auto obj = classFactory::instance()->create_class("child");
  auto p3 = classFactory::instance()->create_class("point");

  mao::serialization::classJsonStream cs;

  std::string ss = cs.to_json(obj);
  std::cout << ss << std::endl << "_____________________________________";
  auto obj1 = cs.from_json(ss);
  std::cout << cs.to_json(obj1) << std::endl;

  std::vector<int> vi = {1, 2, 3, 4};
  int* ii = &(vi[0]);

  *ii = 5;

  return 0;
}