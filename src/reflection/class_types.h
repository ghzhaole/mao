//
// Created by leo on 2023/3/6.
//

#ifndef MAO_SRC_REFLECTION_CLASS_TYPES_H_
#define MAO_SRC_REFLECTION_CLASS_TYPES_H_

#include <string>

namespace mao::reflection {
enum class metaTypes {
  TYPE_UNDEFINED = 0,
  TYPE_INT,
  TYPE_INT64,
  TYPE_FLOAT,
  TYPE_DOUBLE,
  TYPE_LIST,
  TYPE_MAP,
  TYPE_BOOL,
  TYPE_STRING,
  TYPE_CHAR,
  TYPE_OBJECT
};
using std::string;

class classTypes {
 public:
  classTypes() {}

  classTypes(metaTypes type) : type_(type) {}

  classTypes(const classTypes &ctype) : type_(ctype.type_), object_type_name_(ctype.object_type_name_) {}

  classTypes(classTypes &&ctype) : type_(ctype.type_), object_type_name_(ctype.object_type_name_) {}

  classTypes(const char *stype) : classTypes(std::string(stype)) {}

  classTypes(const string &stype) {
	if (stype == "int" || stype == "INT" || stype == "Int" || stype == "int32_t" || stype == "int32" || stype == "INT32"
		|| stype == "Int32" || stype == "i") {
	  type_ = metaTypes::TYPE_INT;
	} else if (stype == "int64" || stype == "INT64" || stype == "Int64" || stype == "int64_t" || stype == "long"
		|| stype == "long long" || stype == "LONG" || stype == "Long" || stype == "LONG LONG" || stype == "Long Long"
		|| stype == "l") {
	  type_ = metaTypes::TYPE_INT64;
	} else if (stype == "float" || stype == "Float" || stype == "FLOAT" || stype == "f") {
	  type_ = metaTypes::TYPE_FLOAT;
	} else if (stype == "double" || stype == "Double" || stype == "DOUBLE" || stype == "d") {
	  type_ = metaTypes::TYPE_DOUBLE;
	} else if (stype == "vector" || stype == "Vector" || stype == "VECTOR" || stype == "list" || stype == "List"
		|| stype == "LIST" || stype == "v") {
	  // name of std::vector<>
	  type_ = metaTypes::TYPE_LIST;
	} else if (stype == "map" || stype == "Map" || stype == "MAP" || stype == "dict" || stype == "Dict"
		|| stype == "DICT" || stype == "m") {
	  // mame of std::map<>
	  type_ = metaTypes::TYPE_MAP;
	} else if (stype == "bool" || stype == "Bool" || stype == "BOOL" || stype == "b") {
	  type_ = metaTypes::TYPE_BOOL;
	} else if (stype == "string" || stype == "String" || stype == "STRING" || stype == "s") {
	  type_ = metaTypes::TYPE_STRING;
	} else if (stype == "char" || stype == "Char" || stype == "CHAR" || stype == "c") {
	  type_ = metaTypes::TYPE_CHAR;
	} else if (stype == "") {
	  type_ = metaTypes::TYPE_UNDEFINED;
	} else {
	  object_type_name_ = stype;
	  type_ = metaTypes::TYPE_OBJECT;
	}
  }

  bool operator<(const classTypes &ctype) const {
	if (type_ != metaTypes::TYPE_OBJECT) { return type_ < ctype.type_; }
	else { return object_type_name_ < ctype.object_type_name_; }
  }

  bool operator==(const classTypes &ctype) const {
	if (type_ != metaTypes::TYPE_OBJECT) { return type_ == ctype.type_; }
	else { return object_type_name_ == ctype.object_type_name_; }
  }

  bool operator==(const string &stype) const {
	if (stype == "int" || stype == "INT" || stype == "Int" || stype == "int32_t" || stype == "int32" || stype == "INT32"
		|| stype == "Int32" || stype == "i") {
	  return type_ == metaTypes::TYPE_INT;
	} else if (stype == "int64" || stype == "INT64" || stype == "Int64" || stype == "int64_t" || stype == "long"
		|| stype == "long long" || stype == "LONG" || stype == "Long" || stype == "LONG LONG" || stype == "Long Long"
		|| stype == "l") {
	  return type_ == metaTypes::TYPE_INT64;
	} else if (stype == "float" || stype == "Float" || stype == "FLOAT" || stype == "f") {
	  return type_ == metaTypes::TYPE_FLOAT;
	} else if (stype == "double" || stype == "Double" || stype == "DOUBLE" || stype == "d") {
	  return type_ == metaTypes::TYPE_DOUBLE;
	} else if (stype == "vector" || stype == "Vector" || stype == "VECTOR" || stype == "list" || stype == "List"
		|| stype == "LIST" || stype == "v") {
	  // name of std::vector<>
	  return type_ == metaTypes::TYPE_LIST;
	} else if (stype == "map" || stype == "Map" || stype == "MAP" || stype == "dict" || stype == "Dict"
		|| stype == "DICT" || stype == "m") {
	  // mame of std::map<>
	  return type_ == metaTypes::TYPE_MAP;
	} else if (stype == "bool" || stype == "Bool" || stype == "BOOL" || stype == "b") {
	  return type_ == metaTypes::TYPE_BOOL;
	} else if (stype == "string" || stype == "String" || stype == "STRING" || stype == "s") {
	  return type_ == metaTypes::TYPE_STRING;
	} else if (stype == "char" || stype == "Char" || stype == "CHAR" || stype == "c") {
	  return type_ == metaTypes::TYPE_CHAR;
	} else if (stype == "" || stype == "undefined" || stype == "UNDEFINED" || stype == "Undefined" || stype == "NULL"
		|| stype == "nullptr") {
	  return type_ == metaTypes::TYPE_UNDEFINED;
	} else {
	  return object_type_name_ == stype;
	}
	return true;
  }

  bool operator==(const char* stype) const {
	return operator==(string(stype));
  }

  operator std::string() {
	switch (type_) {
	  case metaTypes::TYPE_UNDEFINED: return "undefined";
	  case metaTypes::TYPE_INT:return "int";
	  case metaTypes::TYPE_INT64:return "int64";
	  case metaTypes::TYPE_FLOAT:return "float";
	  case metaTypes::TYPE_DOUBLE:return "double";
	  case metaTypes::TYPE_LIST:return "vector";
	  case metaTypes::TYPE_MAP:return "map";
	  case metaTypes::TYPE_BOOL:return "bool";
	  case metaTypes::TYPE_STRING:return "string";
	  case metaTypes::TYPE_CHAR:return "char";
	  case metaTypes::TYPE_OBJECT:return object_type_name_;
	}
	return "undefined";
  }

  metaTypes type() const { return type_; }

  const string &object_type_name() const {
	return object_type_name_;
  }

 private:
  metaTypes type_{metaTypes::TYPE_UNDEFINED};

  string object_type_name_{""};
};
}
#endif //MAO_SRC_REFLECTION_CLASS_TYPES_H_
