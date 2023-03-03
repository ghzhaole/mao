//
// Created by leo on 2023/2/28.
//

#include "class_factory.h"

namespace mao::reflection {
const std::string &metaObject::get_class_name() const { return class_name_; }

void metaObject::set_class_name(const string &name) { class_name_ = name; }

size_t metaObject::get_field_count() const {
  return mao::reflection::classFactory::instance()->get_field_count(
      class_name_);
}

classField *metaObject::get_field(size_t idx) const {
  return mao::reflection::classFactory::instance()->get_field(class_name_, idx);
}

classField *metaObject::get_field(const string &fieldName) const {
  return mao::reflection::classFactory::instance()->get_field(class_name_,
                                                              fieldName);
}

void classFactory::register_class(const string &className,
                                  metaObjectCreator creator) {
  class_map_.emplace(className, creator);
}

void classFactory::register_class(const string &className,
                                  const string &parentClassName,
                                  metaObjectCreator creator) {
  class_map_.emplace(className, creator);
  size_t parent_field_count = get_field_count(parentClassName);
  for (size_t idx = 0; idx < parent_field_count; ++idx) {
    auto field = get_field(parentClassName, idx);
    if (field) {
      register_class_field(className, field->offset(), field->name(),
                           field->type(), field->sub_type(), field->is_array());
    }
  }
}

shared_ptr<metaObject> classFactory::create_class(const string &className) {
  if (class_map_.contains(className)) {
    return class_map_[className]();
  }
  return nullptr;
}

classFactory *classFactory::factory_ins_ =
    classSingleton<classFactory>::instance();

void classFactory::register_class_field(const string &className, size_t offset,
                                        const string &fieldName,
                                        const string &fieldType) {
  classField *field = new classField(offset, fieldName, fieldType);
  field_map_[className].emplace_back(field);
}

void classFactory::register_class_field(const string &className, size_t offset,
                                        const string &fieldName,
                                        const string &fieldType,
                                        const string &subFieldType,
                                        bool is_array) {
  classField *field =
      new classField(offset, fieldName, fieldType, subFieldType, is_array);
  field_map_[className].push_back(field);
}

size_t classFactory::get_field_count(const string &className) {
  if (field_map_.contains(className)) {
    return field_map_[className].size();
  }
  return 0;
}

classField *classFactory::get_field(const string &className, size_t idx) {
  if (field_map_.contains(className)) {
    return field_map_[className][idx];
  }
  return nullptr;
}

classField *classFactory::get_field(const string &className,
                                    const string &fieldName) {
  if (field_map_.contains(className)) {
    for (auto field : field_map_[className]) {
      if (field->name() == fieldName) {
        return field;
      }
    }
  }
  return nullptr;
}
}  // namespace mao::reflection
