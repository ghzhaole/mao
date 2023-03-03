//
// Created by leo on 2023/2/28.
//

#ifndef MAO_TEST_STRUCT_H
#define MAO_TEST_STRUCT_H

#include "reflection/class_register.h"

struct parent : public mao::reflection::metaObject {
 public:
  int i{1};
};

struct point : public mao::reflection::metaObject {
  float x{0}, y{0};
};

struct pointTable : public mao::reflection::metaObject {
  std::vector<std::shared_ptr<point>> points;
};

struct child : public parent {
 public:
  int j{1};
  int c{10};
  float k{2.1f};
  double l{3.4};
  bool b{true};
  std::string s{"hello"};
  std::vector<std::shared_ptr<point>> points;
};
#endif  // MAO_TEST_STRUCT_H
