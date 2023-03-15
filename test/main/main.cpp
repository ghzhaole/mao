//
// Created by leo on 2023/2/28.
//
#include <iostream>
#include <sstream>
#include "json/class_json.h"
#include "reflection/class_factory.h"
#include "serialization/class_json_stream.h"
#include "utils/library/class_library_loader.h"

using namespace mao::library;
using namespace mao::reflection;
using namespace mao::json;

int main() {
  classJson json = classJson::from_string(
      R"({"1":{"a":[3,2,true,false,null,{"v":true,"t":null,"f":{"32":5,"vc":true}}]},"3":4,"5":[6,7,8,{"9":"abc","10":"def"}]})");
  std::cout << classJson::set_format(true) << json;
  return 0;
}