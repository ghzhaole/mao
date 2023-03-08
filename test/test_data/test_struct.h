//
// Created by leo on 2023/2/28.
//

#ifndef MAO_TEST_STRUCT_H
#define MAO_TEST_STRUCT_H

#include "reflection/class_register.h"

struct parent : public mao::reflection::metaObject {
 public:
  int i{1};
  int k{9};
};

struct point : public mao::reflection::metaObject {
  float x{0}, y{0};
};

struct child : public parent {
 public:
 std::vector<std::shared_ptr<point>> ps;
  std::vector<int> vi{3,4};
  std::map<std::string, int> mi{{"1",2}};
};
#endif  // MAO_TEST_STRUCT_H
