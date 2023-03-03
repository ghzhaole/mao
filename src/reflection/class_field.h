//
// Created by leo on 2023/2/28.
//

#include <string>
#include <vector>

#include "export.h"

#ifndef MAO_CLASS_FIELD_H
#define MAO_CLASS_FIELD_H
namespace mao::reflection {
using std::string;
using std::vector;

class metaObject;

class MAO_REFLECTION_EXPORTS classField {
 public:
  classField()
      : offset_(0), name_(""), type_(""), sub_type_(""), is_array_(false) {}

  classField(size_t offset, const string &name, const string &type,
             const string &subType, bool isArray)
      : offset_(offset),
        name_(name),
        type_(type),
        sub_type_(subType),
        is_array_(isArray) {}

  classField(size_t offset, const string &name, const string &type)
      : offset_(offset),
        name_(name),
        type_(type),
        sub_type_(""),
        is_array_(false) {}

  const string &type() const { return type_; }

  const string &name() const { return name_; }

  const string &sub_type() const { return sub_type_; }

  bool is_array() const { return is_array_; }

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
    if (objPtr && is_array_) {
      auto val = (vector<T> *)((unsigned char *)(objPtr.get()) + offset_);
      if (idx < val->size()) {
        value = val->at(idx);
      }
    }
  }

  template <class T>
  T *get(const std::shared_ptr<metaObject> &objPtr, size_t idx) {
    if (objPtr && is_array_) {
      vector<T> *val = (vector<T> *)((unsigned char *)(objPtr.get()) + offset_);
      if (idx < val->size()) {
        T *valPtr = &((*val)[idx]);
        return valPtr;
      }
    }
    return nullptr;
  }

  template <>
  bool *get(const std::shared_ptr<metaObject> &objPtr, size_t idx) {
    if (objPtr && is_array_) {
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
    *((T *)((unsigned char *)(objPtr.get()) + offset_)) = value;
  }

  template <class T>
  void set(const std::shared_ptr<metaObject> &objPtr, size_t idx,
           const T &value) {
    if (objPtr && is_array_) {
      auto val = (vector<T> *)((unsigned char *)(objPtr.get()) + offset_);
      if (val->size() <= idx) {
        val->resize(idx + 1);
      }
      (*val)[idx] = value;
    }
  }

  template <class T>
  size_t size(const std::shared_ptr<metaObject> &objPtr) {
    if (objPtr && is_array_) {
      return ((vector<T> *)((unsigned char *)(objPtr.get()) + offset_))->size();
    }
    return 0;
  }

 private:
  size_t offset_;

  string name_;

  string type_;

  string sub_type_;

  bool is_array_;
};
}  // namespace mao::reflection
#endif  // MAO_CLASS_FIELD_H
