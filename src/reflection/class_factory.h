//
// Created by leo on 2023/2/28.
//

#ifndef MAO_CLASS_FACTORY_H
#define MAO_CLASS_FACTORY_H

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "class_field.h"
#include "export.h"
#include "utils/singleton/classSingleton.h"

namespace mao::reflection {
using namespace singleton;
using std::function;
using std::map;
using std::shared_ptr;
using std::string;
using std::vector;

class MAO_REFLECTION_EXPORTS metaObject {
 public:
  metaObject() : class_name_("") {}

  virtual ~metaObject() {}

  void set_class_name(const string &name);

  const string &get_class_name() const;

  size_t get_field_count() const;

  classField *get_field(size_t idx) const;

  classField *get_field(const string &fieldName) const;

  template <class T>
  void get(const string &fieldName, T &value) {
    auto field = get_field(fieldName);
    if (field) {
      value = *((T *)((unsigned char *)(this) + field->offset()));
    }
  }

  template <class T>
  T *get(const string &fieldName) {
    auto field = get_field(fieldName);
    if (field) {
      return ((T *)((unsigned char *)(this) + field->offset()));
    }
    return nullptr;
  }

  template <class T>
  void get(const string &fieldName, size_t idx, T &value) {
    auto field = get_field(fieldName);
    if (field && field->is_list()) {
      auto val = (vector<T> *)((unsigned char *)(this) + field->offset());
      if (idx < val->size()) {
        value = val->at(idx);
      }
    }
  }

  template <class T>
  void get(const string &fieldName, const string &key, T &value) {
    auto field = get_field(fieldName);
    if (field && field->is_map()) {
      map < string, T > *val = (map<string, T> *)((unsigned char *)(this) + field->offset());
      if (val->contains(key)) {
        value = (*val)[key];
      }
    }
  }

  template <class T>
  T *get(const string &fieldName, size_t idx) {
    auto field = get_field(fieldName);
    if (field && field->is_list()) {
      vector<T> *val = (vector<T> *)((unsigned char *)(this) + field->offset());
      if (idx < val->size()) {
        T *valPtr = &((*val)[idx]);
        return valPtr;
      }
    }
    return nullptr;
  }

  template <class T>
  T *get(const string &fieldName, const string &key) {
    auto field = get_field(fieldName);
    if (field && field->is_map()) {
      map < string, T > *val = (map<string, T> *)((unsigned char *)(this) + field->offset());
      if (val->contains(key)) {
        T *valPtr = &((*val)[key]);
        return valPtr;
      }
    }
    return nullptr;
  }

  template <>
  bool *get(const string &fieldName, size_t idx) {
    auto field = get_field(fieldName);
    if (field && field->is_list()) {
      vector<char> *val = (vector<char> *)((unsigned char *)(this) + field->offset());
      if (idx < val->size()) {
        char *valPtr = &((*val)[idx]);
        return (bool *)valPtr;
      }
    }
    return nullptr;
  }

  template <class T>
  void set(const string &fieldName, const T &value) {
    auto field = get_field(fieldName);
    if (field) {
      *((T *)((unsigned char *)(this) + field->offset())) = value;
    }
  }

  template <class T>
  void set(const string &fieldName, size_t idx, const T &value) {
    auto field = get_field(fieldName);
    if (field && field->is_list()) {
      auto val = (vector<T> *)((unsigned char *)(this) + field->offset());
      if (val->size() <= idx) {
        val->resize(idx + 1);
      }
      (*val)[idx] = value;
    }
  }

  template <class T>
  void set(const string &fieldName, const string &key, const T &value) {
    auto field = get_field(fieldName);
    if (field && field->is_map()) {
      auto val = (map<string, T> *)((unsigned char *)(this) + field->offset());
      (*val)[key] = value;
    }
  }

  template <>
  void set(const string &fieldName, size_t idx, const bool &value) {
    auto field = get_field(fieldName);
    if (field && field->is_list()) {
      auto val = (vector<char> *)((unsigned char *)(this) + field->offset());
      if (val->size() <= idx) {
        val->resize(idx + 1);
      }
      (*val)[idx] = (char)value;
    }
  }

  template <class T>
  size_t size(const string &fieldName) {
    auto field = get_field(fieldName);
    if (!field) {
      return 0;
    }
    if (field->is_list()) {
      return ((vector<T> *)((unsigned char *)(this) + field->offset()))->size();
    } else if (field->is_map()) {
      return ((map<std::string, T> *)((unsigned char *)(this) + field->offset()))->size();
    }
    return 0;
  }

 private:
  string class_name_{""};
};

typedef function<shared_ptr<metaObject>()> metaObjectCreator;

class MAO_REFLECTION_EXPORTS classFactory {
  friend class classSingleton<classFactory>;

 public:
  static classFactory *instance() { return factory_ins_; }

  void register_class(const string &className, metaObjectCreator creator);

  void register_class(const string &className, const string &parentClassName, metaObjectCreator creator);

  shared_ptr<metaObject> create_class(const string &className);

  void register_class_field(const string &className,
                            size_t offset,
                            const string &fieldName,
                            const classTypes &fieldType,
                            const classTypes &subFieldType = "");

  size_t get_field_count(const string &className);

  classField *get_field(const string &className, size_t idx);

  classField *get_field(const string &className, const string &fieldName);

 private:
  classFactory() {}

 private:
  static classFactory *factory_ins_;

  map<string, metaObjectCreator> class_map_;

  map<string, vector<classField *>> field_map_;
};
}  // namespace mao::reflection
#endif  // MAO_CLASS_FACTORY_H
