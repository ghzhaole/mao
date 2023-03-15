#pragma once

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "export.h"

namespace mao::json {
class classJson;

using std::istream;
using std::map;
using std::ostream;
using std::string;
using std::vector;
using list = vector<classJson>;
using object = map<string, classJson>;
union value {
  bool bool_;
  int int_;
  int64_t int64_;
  float float_;
  double double_;
  string *string_;
  list *list_;
  object *object_;
};

enum class valueType {
  NULL_TYPE = 0,
  BOOL_TYPE,
  INT_TYPE,
  INT64_TYPE,
  FLOAT_TYPE,
  DOUBLE_TYPE,
  STRING_TYPE,
  LIST_TYPE,
  OBJECT_TYPE,
  EMPTY_TYPE
};

class MAO_JSON_EXPORTS classJson {
 public:
  classJson();

  classJson(const valueType &);

  classJson(const classJson &);

  classJson(classJson &&);

  classJson &operator=(const classJson &);

  classJson &operator=(classJson &&);

  bool operator==(const classJson &);

  classJson(bool);

  classJson(int);

  classJson(int64_t);

  classJson(float);

  classJson(double);

  classJson(const string &);

  classJson(const char *);

  template <typename T>
  classJson(const std::vector<T> &val) : type_(valueType::LIST_TYPE) {
    value_.list_ = new list;
    for (auto &&item : val) {
      value_.list_->emplace_back(item);
    }
  }

  template <typename T>
  classJson(const std::map<string, T> &val) : type_(valueType::OBJECT_TYPE) {
    value_.object_ = new object;
    for (auto &&item : val) {
      value_.object_->emplace(item.first, item.second);
    }
  }

  valueType get_type() const;

  bool as_bool();

  int as_int();

  int64_t as_int64();

  float as_float();

  double as_double();

  string &as_string();

  list &as_list();

  object &as_object();

  string to_string();

  static classJson from_string(const string &);

  list::iterator list_begin();

  list::iterator list_end();

  auto object_begin();

  auto object_end();

  template <typename T>
  classJson &append(const T &other) {
    if (type_ != valueType::LIST_TYPE) {
      reset();
      type_ = valueType::LIST_TYPE;
    }
    if (!value_.list_) {
      value_.list_ = new list;
    }
    value_.list_->emplace_back(other);
    return *this;
  }

  classJson &operator[](size_t idx);

  classJson &operator[](const string &key);

  template <typename T>
  classJson &insert(size_t idx, const T &val) {
    if (type_ != valueType::LIST_TYPE) {
      reset();
      type_ = valueType::LIST_TYPE;
    }
    if (!value_.list_) {
      value_.list_ = new list;
    }
    if (value_.list_->size() <= idx) {
      value_.list_->resize(idx * 2 + 1);
    }
    value_.list_->emplace(value_.list_->begin() + idx, val);
    return *this;
  }

  template <typename T>
  classJson &insert(const string &key, const T &val) {
    if (type_ != valueType::OBJECT_TYPE) {
      reset();
      type_ = valueType::OBJECT_TYPE;
    }
    if (!value_.object_) {
      value_.object_ = new object;
    }
    value_.object_->emplace(key, val);
    return *this;
  }

  size_t size() const;

  static string set_format(bool ind_, size_t ind_size = 2) {
    indent_ = ind_;
    indent_size_ = ind_size;
    return string();
  }

  friend MAO_JSON_EXPORTS ostream &operator<<(ostream &, const classJson &);

  ~classJson();

 private:
  void reset() noexcept;

  void write(ostream &, size_t ind_size = 0) const;

  string get_ind(size_t ind_size = 0) const;

 private:
  value value_;

  valueType type_{valueType::EMPTY_TYPE};

 private:
  static bool indent_;

  static size_t indent_size_;
};

class MAO_JSON_EXPORTS classJsonParser {
 public:
  classJsonParser(const string &);

  classJson parser();

 private:
  classJson parser_object();

  classJson parser_list();

  classJson parser_bool();

  classJson parser_number();

  classJson parser_null();

  classJson parser_string();

  void skip_white();

  size_t idx_{0};

  size_t size_{0};

  string str_{""};
};

}  // namespace mao::json