//
// Created by leo on 2023/2/28.
//

#ifndef MAO_CLASS_REGISTER_H
#define MAO_CLASS_REGISTER_H

#include "class_factory.h"

namespace mao::reflection {
class classRegister {
 public:
  classRegister(const string &className, metaObjectCreator creator) {
    classFactory::instance()->register_class(className, creator);
  }

  classRegister(const string &className, const string &parentClassName, metaObjectCreator creator) {
    classFactory::instance()->register_class(className, parentClassName, creator);
  }

  template<typename... Ts>
  classRegister(const string &className,
                size_t offset,
                const string &fieldName,
                const string &fieldType,
                const Ts &...fieldSubType) {
    classFactory::instance()->register_class_field(className, offset, fieldName, fieldType, fieldSubType...);
  }
};
}  // namespace mao::reflection

#define REGISTER_CLASS(className)                                          \
  std::shared_ptr<mao::reflection::metaObject> createObject##className() { \
    std::shared_ptr<mao::reflection::metaObject> obj =                     \
        std::make_shared<className>();                                     \
    obj->set_class_name(#className);                                       \
    return obj;                                                            \
  }                                                                        \
  mao::reflection::classRegister classRegister##className(                 \
      #className, createObject##className);
#define REGISTER_CLASS_WITH_INHERIT(className, parentClassName)            \
  std::shared_ptr<mao::reflection::metaObject> createObject##className() { \
    std::shared_ptr<mao::reflection::metaObject> obj =                     \
        std::make_shared<className>();                                     \
    obj->set_class_name(#className);                                       \
    return obj;                                                            \
  }                                                                        \
  mao::reflection::classRegister classRegister##className(                 \
      #className, #parentClassName, createObject##className);
#define REGISTER_CLASS_FIELD(className, fieldName, fieldType)           \
  mao::reflection::classRegister classRegister##className##fieldName(   \
      #className, (size_t)(&(((className *)0)->fieldName)), #fieldName, \
      #fieldType);
#define REGISTER_CLASS_FIELD_1(className, fieldName, fieldType, subType) \
  mao::reflection::classRegister classRegister##className##fieldName(    \
      #className, (size_t)(&(((className *)0)->fieldName)), #fieldName,  \
      #fieldType, #subType);
#endif  // MAO_CLASS_REGISTER_H
