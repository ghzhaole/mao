#pragma once

#include <map>
#include <string>
#include <vector>

namespace mao::json {
class classJson;
using std::map;
using std::string;
using std::vector;

using list = vector<classJson>;
using object = map<string, classJson>;

union value {
  bool bool_;
  int int_;
  float float_;
  double double_;
  string* string_;
  list* list_;
  object* object_;
};

enum class valueType {
  NULL_TYPE = 0,
  BOOL_TYPE,
  INT_TYPE,
  FLOAT_TYPE,
  DOUBLE_TYPE,
  STRING_TYPE,
  LIST_TYPE,
  OBJECT_TYPE
};

class classJson {
 public:
  classJson();
  classJson(const valueType&);
  classJson(const classJson&);
  ~classJson();

 private:
};

}  // namespace mao::json