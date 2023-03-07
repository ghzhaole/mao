//
// Created by leo on 2023/2/28.
//
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "class_types.h"
#include "export.h"

#ifndef MAO_CLASS_FIELD_H
#define MAO_CLASS_FIELD_H
namespace mao::reflection {
using std::map;
using std::string;
using std::vector;

class metaObject;

class MAO_REFLECTION_EXPORTS classField {
 public:
  classField() : offset_(0), name_(""), type_(), sub_type_() {}

  classField(size_t offset, const string &name, const string &type,
             const string &subType)
      : offset_(offset), name_(name), type_(type), sub_type_(subType) {}

  classField(size_t offset, const string &name, const classTypes &type,
             const classTypes &subType)
      : offset_(offset), name_(name), type_(type), sub_type_(subType) {}

  classField(size_t offset, const string &name, const string &type)
      : offset_(offset), name_(name), type_(type), sub_type_() {}

  classField(size_t offset, const string &name, const classTypes &type)
      : offset_(offset), name_(name), type_(type), sub_type_() {}

  const classTypes &type() const { return type_; }

  const string &name() const { return name_; }

  const classTypes &sub_type() const { return sub_type_; }

  bool is_list() const { return type_.type() == metaTypes::TYPE_LIST; }

  bool is_map() const { return type_.type() == metaTypes::TYPE_MAP; }

  size_t offset() const { return offset_; }

  template <class T>
  void get(const std::shared_ptr<metaObject> &objPtr, T &value) {
    if (objPtr) {
      value = *((T *)((unsigned char *)(objPtr.get()) + offset_));
    }
  }

  template <class T>
  T *get(const std::shared_ptr<metaObject> &objPtr) {
    if (objPtr) {
      return ((T *)((unsigned char *)(objPtr.get()) + offset_));
    }
    return nullptr;
  }

  template <class T>
  void get(const std::shared_ptr<metaObject> &objPtr, size_t idx, T &value) {
    if (objPtr && is_list()) {
      auto val = (vector<T> *)((unsigned char *)(objPtr.get()) + offset_);
      if (idx < val->size()) {
        value = val->at(idx);
      }
    }
  }

  template <class T>
  void get(const std::shared_ptr<metaObject> &objPtr, const string &key,
           T &value) {
    if (objPtr && is_map()) {
      map<string, T> *val =
          (map<string, T> *)((unsigned char *)(objPtr.get()) + offset_);
      if (val->contains(key)) {
        value = (*val)[key];
      }
    }
  }

  template <class T>
  T *get(const std::shared_ptr<metaObject> &objPtr, size_t idx) {
    if (objPtr && is_list()) {
      vector<T> *val = (vector<T> *)((unsigned char *)(objPtr.get()) + offset_);
      if (idx < val->size()) {
        T *valPtr = &((*val)[idx]);
        return valPtr;
      }
    }
    return nullptr;
  }

  template <class T>
  T *get(const std::shared_ptr<metaObject> &objPtr, const string &key) {
    if (objPtr && is_map()) {
      map<string, T> *val =
          (map<string, T> *)((unsigned char *)(objPtr.get()) + offset_);
      if (val->contains(key)) {
        T *valPtr = &((*val)[key]);
        return valPtr;
      }
    }
    return nullptr;
  }

  template <>
  bool *get(const std::shared_ptr<metaObject> &objPtr, size_t idx) {
    if (objPtr && is_list()) {
      vector<char> *val =
          (vector<char> *)((unsigned char *)(objPtr.get()) + offset_);
      if (idx < val->size()) {
        char *valPtr = &((*val)[idx]);
        return (bool *)valPtr;
      }
    }
    return nullptr;
  }

  template <class T>
  void set(const std::shared_ptr<metaObject> &objPtr, const T &value) {
    std::cout << value << std::endl;
    *((T *)((unsigned char *)(objPtr.get()) + offset_)) = value;
  }

  template <class T>
  void set(const std::shared_ptr<metaObject> &objPtr, size_t idx,
           const T &value) {
    if (objPtr && is_list()) {
      auto val = (vector<T> *)((unsigned char *)(objPtr.get()) + offset_);
      if (val->size() <= idx) {
        val->resize(idx + 1);
      }
      (*val)[idx] = value;
    }
  }

  template <class T>
  void set(const std::shared_ptr<metaObject> &objPtr, const string &key,
           const T &value) {
    if (objPtr && is_map()) {
      auto val = (map<string, T> *)((unsigned char *)(objPtr.get()) + offset_);
      (*val)[key] = value;
    }
  }

  template <>
  void set(const std::shared_ptr<metaObject> &objPtr, size_t idx,
           const bool &value) {
    if (objPtr && is_list()) {
      auto val = (vector<char> *)((unsigned char *)(objPtr.get()) + offset_);
      if (val->size() <= idx) {
        val->resize(idx + 1);
      }
      (*val)[idx] = (char)value;
    }
  }

  template <class T>
  size_t size(const std::shared_ptr<metaObject> &objPtr) {
    if (!objPtr) {
      return 0;
    }
    if (is_list()) {
      return ((vector<T> *)((unsigned char *)(objPtr.get()) + offset_))->size();
    } else if (is_map()) {
      return ((map<std::string, T> *)((unsigned char *)(objPtr.get()) +
                                      offset_))
          ->size();
    }
    return 0;
  }

 private:
  size_t offset_;

  string name_;

  classTypes type_;

  classTypes sub_type_;
};
}  // namespace mao::reflection
#endif  // MAO_CLASS_FIELD_H
