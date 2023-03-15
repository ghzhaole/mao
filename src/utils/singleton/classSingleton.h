//
// Created by leo on 2023/2/28.
//

#ifndef MAO_CLASSSINGLETON_H
#define MAO_CLASSSINGLETON_H

#include <memory>

namespace mao::singleton {
template<typename T>
class classSingleton {
 public:
  static T *instance() {
    static auto ins = new T();
    return ins;
  }

 private:
  classSingleton() {}
};
}  // namespace mao::singleton
#endif  // MAO_CLASSSINGLETON_H
