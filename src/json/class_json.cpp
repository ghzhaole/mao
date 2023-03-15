#include "class_json.h"
#include <cassert>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace mao::json {
bool classJson::indent_ = false;

size_t classJson::indent_size_ = 2;

classJson::classJson() : type_(valueType::EMPTY_TYPE) { value_.double_ = 0; }

classJson::classJson(const valueType &type) : type_(type) {
  if (type_ == valueType::STRING_TYPE) {
    value_.string_ = new string;
  } else if (type_ == valueType::LIST_TYPE) {
    value_.list_ = new list;
  } else if (type_ == valueType::OBJECT_TYPE) {
    value_.object_ = new object;
  } else {
    value_.double_ = 0;
  }
}

classJson::classJson(const classJson &other) : type_(other.type_) {
  switch (type_) {
    case valueType::NULL_TYPE:value_.double_ = 0;
      break;
    case valueType::BOOL_TYPE:value_.bool_ = other.value_.bool_;
      break;
    case valueType::INT_TYPE:value_.int_ = other.value_.int_;
      break;
    case valueType::INT64_TYPE:value_.int64_ = other.value_.int64_;
      break;
    case valueType::FLOAT_TYPE:value_.float_ = other.value_.float_;
      break;
    case valueType::DOUBLE_TYPE:value_.double_ = other.value_.double_;
      break;
    case valueType::STRING_TYPE:value_.string_ = new string(*other.value_.string_);
      break;
    case valueType::LIST_TYPE:value_.list_ = new list(*other.value_.list_);
      break;
    case valueType::OBJECT_TYPE:value_.object_ = new object(*other.value_.object_);
      break;
    default:value_.double_ = 0;
      break;
  }
}

classJson::classJson(classJson &&other) : type_(other.type_) {
  switch (type_) {
    case valueType::NULL_TYPE:value_.double_ = 0;
      break;
    case valueType::BOOL_TYPE:value_.bool_ = other.value_.bool_;
      break;
    case valueType::INT_TYPE:value_.int_ = other.value_.int_;
      break;
    case valueType::INT64_TYPE:value_.int64_ = other.value_.int64_;
      break;
    case valueType::FLOAT_TYPE:value_.float_ = other.value_.float_;
      break;
    case valueType::DOUBLE_TYPE:value_.double_ = other.value_.double_;
      break;
    case valueType::STRING_TYPE:value_.string_ = other.value_.string_;
      other.value_.string_ = 0;
      break;
    case valueType::LIST_TYPE:value_.list_ = other.value_.list_;
      other.value_.list_ = 0;
      break;
    case valueType::OBJECT_TYPE:value_.object_ = other.value_.object_;
      other.value_.object_ = 0;
      break;
    default:value_.double_ = 0;
      break;
  }
  other.type_ = valueType::EMPTY_TYPE;
}

classJson &classJson::operator=(const classJson &other) {
  reset();
  switch (other.type_) {
    case valueType::NULL_TYPE:value_.double_ = 0;
      break;
    case valueType::BOOL_TYPE:value_.bool_ = other.value_.bool_;
      break;
    case valueType::INT_TYPE:value_.int_ = other.value_.int_;
      break;
    case valueType::INT64_TYPE:value_.int64_ = other.value_.int64_;
      break;
    case valueType::FLOAT_TYPE:value_.float_ = other.value_.float_;
      break;
    case valueType::DOUBLE_TYPE:value_.double_ = other.value_.double_;
      break;
    case valueType::STRING_TYPE:value_.string_ = new string(*other.value_.string_);
      break;
    case valueType::LIST_TYPE:value_.list_ = new list(*other.value_.list_);
      break;
    case valueType::OBJECT_TYPE:value_.object_ = new object(*other.value_.object_);
      break;
    default:value_.double_ = 0;
      break;
  }
  type_ = other.type_;
  return *this;
}

classJson &classJson::operator=(classJson &&other) {
  reset();
  switch (other.type_) {
    case valueType::NULL_TYPE:value_.double_ = 0;
      break;
    case valueType::BOOL_TYPE:value_.bool_ = other.value_.bool_;
      break;
    case valueType::INT_TYPE:value_.int_ = other.value_.int_;
      break;
    case valueType::INT64_TYPE:value_.int64_ = other.value_.int64_;
      break;
    case valueType::FLOAT_TYPE:value_.float_ = other.value_.float_;
      break;
    case valueType::DOUBLE_TYPE:value_.double_ = other.value_.double_;
      break;
    case valueType::STRING_TYPE:value_.string_ = other.value_.string_;
      other.value_.string_ = 0;
      break;
    case valueType::LIST_TYPE:value_.list_ = other.value_.list_;
      other.value_.list_ = 0;
      break;
    case valueType::OBJECT_TYPE:value_.object_ = other.value_.object_;
      other.value_.object_ = 0;
      break;
    default:value_.double_ = 0;
      break;
  }
  type_ = other.type_;
  other.type_ = valueType::EMPTY_TYPE;
  return *this;
}

bool classJson::operator==(const classJson &other) {
  if (type_ != other.type_) {
    return false;
  }
  switch (type_) {
    case valueType::STRING_TYPE:return *(value_.string_) == *(other.value_.string_);
    case valueType::LIST_TYPE: {
      size_t len1 = value_.list_->size();
      size_t len2 = other.value_.list_->size();
      if (len1 != len2) {
        return false;
      }
      for (size_t idx = 0; idx < len1; ++idx) {
        if (!(value_.list_->at(idx) == other.value_.list_->at(idx))) {
          return false;
        }
      }
      return true;
    }
    case valueType::OBJECT_TYPE: {
      size_t len1 = value_.object_->size();
      size_t len2 = other.value_.object_->size();
      if (len1 != len2) {
        return false;
      }
      for (auto &&item : *value_.object_) {
        if (0 == other.value_.object_->count(item.first) || !(other.value_.object_->at(item.first) == item.second)) {
          return false;
        }
      }
      return true;
    }
    case valueType::BOOL_TYPE:return value_.bool_ == value_.bool_;
    case valueType::INT_TYPE:return value_.int_ == value_.int_;
    case valueType::INT64_TYPE:return value_.int64_ == value_.int64_;
    case valueType::FLOAT_TYPE:return value_.float_ == value_.float_;
    case valueType::DOUBLE_TYPE:return value_.double_ == value_.double_;
    default:return true;
  }
}

classJson::classJson(bool b) : type_(valueType::BOOL_TYPE) { value_.bool_ = b; }

classJson::classJson(int i) : type_(valueType::INT_TYPE) { value_.int_ = i; }

classJson::classJson(int64_t i64) : type_(valueType::INT64_TYPE) {
  value_.int64_ = i64;
}

classJson::classJson(float f) : type_(valueType::FLOAT_TYPE) {
  value_.float_ = f;
}

classJson::classJson(double d) : type_(valueType::DOUBLE_TYPE) {
  value_.double_ = d;
}

classJson::classJson(const string &s) : type_(valueType::STRING_TYPE) {
  value_.string_ = new string(s);
}

classJson::classJson(const char *s) : classJson(string(s)) {}

valueType classJson::get_type() const { return type_; }

bool classJson::as_bool() {
  if (type_ == valueType::BOOL_TYPE) {
    return value_.bool_;
  }
  return false;
}

int classJson::as_int() {
  if (type_ == valueType::INT_TYPE) {
    return value_.int_;
  }
  return 0;
}

int64_t classJson::as_int64() {
  if (type_ == valueType::INT64_TYPE) {
    return value_.int64_;
  }
  return 0;
}

float classJson::as_float() {
  if (type_ == valueType::FLOAT_TYPE) {
    return value_.float_;
  }
  return 0.f;
}

double classJson::as_double() {
  if (type_ == valueType::DOUBLE_TYPE) {
    return value_.double_;
  }
  return 0.;
}

string &classJson::as_string() {
  if (type_ == valueType::STRING_TYPE) {
    return *value_.string_;
  }
  return *((string *)0);
}

list &classJson::as_list() {
  if (type_ == valueType::LIST_TYPE) {
    return *value_.list_;
  }
  return *((list *)0);
}

object &classJson::as_object() {
  if (type_ == valueType::OBJECT_TYPE) {
    return *value_.object_;
  }
  return *((object *)0);
}

string classJson::to_string() {
  std::stringstream ss;
  operator<<(ss, *this);
  return ss.str();
}

classJson classJson::from_string(const string &str) {
  classJsonParser parser(str);
  return parser.parser();
}

list::iterator classJson::list_begin() {
  assert(type_ == valueType::LIST_TYPE);
  return value_.list_->begin();
}

list::iterator classJson::list_end() {
  assert(type_ == valueType::LIST_TYPE);
  return value_.list_->end();
}

auto classJson::object_begin() {
  assert(type_ == valueType::OBJECT_TYPE);
  return value_.object_->begin();
}

auto classJson::object_end() {
  assert(type_ == valueType::OBJECT_TYPE);
  return value_.object_->end();
}

classJson &classJson::operator[](size_t idx) {
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
  return (*value_.list_)[idx];
}

classJson &classJson::operator[](const string &key) {
  if (type_ != valueType::OBJECT_TYPE) {
    reset();
    type_ = valueType::OBJECT_TYPE;
  }
  if (!value_.object_) {
    value_.object_ = new object;
  }
  if (value_.object_->count(key) == 0) {
    (*value_.object_)[key] = classJson();
  }
  return (*value_.object_)[key];
}

size_t classJson::size() const {
  assert(type_ == valueType::LIST_TYPE || type_ == valueType::OBJECT_TYPE);
  if (type_ == valueType::LIST_TYPE) {
    return value_.list_->size();
  } else {
    return value_.object_->size();
  }
}

classJson::~classJson() { reset(); }

void classJson::reset() noexcept {
  switch (type_) {
    case valueType::NULL_TYPE:
    case valueType::BOOL_TYPE:
    case valueType::INT_TYPE:
    case valueType::INT64_TYPE:
    case valueType::FLOAT_TYPE:
    case valueType::DOUBLE_TYPE:value_.double_ = 0;
      break;
    case valueType::STRING_TYPE:
      if (value_.string_) {
        delete value_.string_;
      }
      value_.string_ = 0;
      break;
    case valueType::LIST_TYPE:
      if (value_.list_) {
        delete value_.list_;
      }
      value_.list_ = 0;
      break;
    case valueType::OBJECT_TYPE:
      if (value_.object_) {
        delete value_.object_;
      }
      value_.object_ = 0;
      break;
    default:value_.bool_ = false;
      break;
  }
  type_ = valueType::NULL_TYPE;
}

void classJson::write(ostream &os, size_t ind_size_) const {
  switch (type_) {
    case valueType::NULL_TYPE:os << "null";
      break;
    case valueType::BOOL_TYPE:os << (value_.bool_ ? "true" : "false");
      break;
    case valueType::INT_TYPE:os << value_.int_;
      break;
    case valueType::INT64_TYPE:os << value_.int64_;
      break;
    case valueType::FLOAT_TYPE: {
      std::stringstream ss;
      ss << std::setprecision(7) << value_.float_;
      if (ss.str().find(".") == std::string::npos) {
        ss << ".0";
      }
      os << ss.str();
      break;
    }
    case valueType::DOUBLE_TYPE: {
      std::stringstream ss;
      ss << std::setprecision(15) << value_.double_;
      if (ss.str().find(".") == std::string::npos) {
        ss << ".0";
      }
      os << get_ind(ind_size_) << ss.str();
      break;
    }
    case valueType::STRING_TYPE:
      if (value_.string_) {
        os << "\"" << *value_.string_ << "\"";
      }
      break;
    case valueType::LIST_TYPE:
      if (value_.list_) {
        os << "[";
        bool first_ = true;
        for (auto iter = value_.list_->begin(); iter != value_.list_->end(); ++iter) {
          if (!first_) {
            os << ", ";
          } else {
            first_ = false;
          }
          os << get_ind(ind_size_ + indent_size_);
          iter->write(os, ind_size_ + indent_size_);
        }
        os << get_ind(ind_size_) << "]";
      }
      break;
    case valueType::OBJECT_TYPE:
      if (value_.object_) {
        os << "{";
        bool first_ = true;
        for (auto iter = value_.object_->begin(); iter != value_.object_->end(); ++iter) {
          if (!first_) {
            os << ", ";
          } else {
            first_ = false;
          }
          os << get_ind(ind_size_ + indent_size_);
          os << "\"" << iter->first << "\"";
          os << ": ";
          iter->second.write(os, ind_size_ + indent_size_);
        }
        os << get_ind(ind_size_) << "}";
      }
      break;
    default:break;
  }
}

string classJson::get_ind(size_t ind_size) const {
  return indent_ ? (string("\n") + string(ind_size, ' ')) : string();
}

ostream &operator<<(ostream &os, const classJson &cjson) {
  cjson.write(os);
  return os;
}

classJsonParser::classJsonParser(const string &str) : str_(str), size_(str.size()) {}

classJson classJsonParser::parser() {
  skip_white();
  if (idx_ >= size_) {
    return classJson();
  }
  char ch = str_[idx_];
  switch (ch) {
    case '{':return parser_object();
      break;
    case '[':return parser_list();
      break;
    case '"':return parser_string();
      break;
    case 't':
    case 'f':return parser_bool();
      break;
    case 'n':return parser_null();
      break;
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case '-':return parser_number();
      break;
    default:return classJson();
      break;
  }
}

classJson classJsonParser::parser_object() {
  classJson json = valueType::OBJECT_TYPE;
  while (idx_++ < size_) {
    skip_white();
    auto sub_key = parser();
    if (sub_key.get_type() != valueType::STRING_TYPE) {
      throw "parser object error! key type is not string!";
    }
    skip_white();
    if (idx_ >= size_) {
      throw "parser object error!";
    } else if (str_[idx_++] != ':') {
      throw "parser object error! missing ':' !";
    }
    skip_white();
    if (idx_ >= size_) {
      throw "parser object error!";
    }
    auto sub_val = parser();
    if (sub_val.get_type() == valueType::EMPTY_TYPE) {
      throw "parser object error! missing value!";
    }
    json.insert(sub_key.as_string(), sub_val);
    skip_white();
    if (idx_ >= size_) {
      throw "parser object error!";
    } else if (str_[idx_] == ',') {
      continue;
    } else if (str_[idx_] == '}') {
      idx_++;
      break;
    } else {
      throw "parser object error!";
    }
  }
  return json;
}

classJson classJsonParser::parser_list() {
  classJson json = valueType::LIST_TYPE;
  while (idx_++ < size_) {
    skip_white();
    auto sub_val = parser();
    if (sub_val.get_type() != valueType::EMPTY_TYPE) {
      json.append(sub_val);
    }
    skip_white();
    if (idx_ >= size_) {
      throw "parser list error!";
    } else if (str_[idx_] == ',') {
      continue;
    } else if (str_[idx_] == ']') {
      idx_++;
      break;
    } else {
      throw "parser list error!";
    }
  }
  return json;
}

classJson classJsonParser::parser_bool() {
  if (str_[idx_] == 't' && str_.substr(idx_, 4) == "true") {
    idx_ += 4;
    return true;
  } else if (str_[idx_] == 'f' && str_.substr(idx_, 5) == "false") {
    idx_ += 5;
    return false;
  }
  throw "parser bool error!";
}

classJson classJsonParser::parser_number() {
  size_t start_pos = idx_;
  while (++idx_ < size_) {
    char ch = str_[idx_];
    if ('0' <= ch && ch <= '9') {
      continue;
    }
    break;
  }
  bool is_dbl = false;
  if (str_[idx_] == '.') {
    is_dbl = true;
    while (++idx_ < size_) {
      char ch = str_[idx_];
      if ('0' <= ch && ch <= '9') {
        continue;
      }
      break;
    }
  }
  if (is_dbl) {
    return std::stod(str_.substr(start_pos, idx_ - start_pos));
  } else {
    return std::stoi(str_.substr(start_pos, idx_ - start_pos));
  }
}

classJson classJsonParser::parser_null() {
  if (str_[idx_] == 'n' && str_.substr(idx_, 4) == "null") {
    idx_ += 4;
    return valueType::NULL_TYPE;
  }
  throw "parser null error!";
}

classJson classJsonParser::parser_string() {
  size_t start_pos = idx_;
  while (++idx_ < size_) {
    char ch = str_[idx_];
    if (ch == '"') {
      idx_++;
      break;
    }
  }
  return str_.substr(start_pos + 1, idx_ - start_pos - 2);
}

void classJsonParser::skip_white() {
  while (idx_ < size_) {
    char ch = str_[idx_];
    if (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r') {
      idx_++;
      continue;
    }
    break;
  }
}
}  // namespace mao::json
