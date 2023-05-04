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

#define REGISTER_CLASS(className,...)                                          \
  std::shared_ptr<mao::reflection::metaObject> createObject##className() { \
    std::shared_ptr<mao::reflection::metaObject> obj =                     \
        std::make_shared<className>();                                     \
    obj->set_class_name(#className);                                       \
    return obj;                                                            \
  }                                                                        \
  mao::reflection::classRegister classRegister##className(                 \
      #className,__STR__ARGS(__VA_ARGS__), createObject##className);

#define REGISTER_CLASS_FIELD(className, fieldName, fieldType, ...)       \
  mao::reflection::classRegister classRegister##className##fieldName(    \
      #className, (size_t)(&(((className *)0)->fieldName)), #fieldName,  \
      #fieldType, __STR__ARGS(__VA_ARGS__));

#define __CONNECT__FLAG(f1, f2) f1##f2 // 宏参数拼接成符号
#define __CALL(f1, f2) __CONNECT__FLAG(f1, f2) // 宏嵌套

#define __STR__ARGS(...) #__VA_ARGS__ // 宏变长参数
#define __CONNTECT__ARGS(...) (__VA_ARGS__) // 宏变长参数
#define __CALL2(f1,f2,...) __CALL(f1,f2)__CONNTECT__ARGS(__VA_ARGS__) // 嵌套+变长参数

#endif  // MAO_CLASS_REGISTER_H
