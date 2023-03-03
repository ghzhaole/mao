//
// Created by leo on 2023/3/1.
//

#ifndef MAO_SRC_SERIALIZATION_CLASS_JSON_WRITER_H_
#define MAO_SRC_SERIALIZATION_CLASS_JSON_WRITER_H_

#include <iostream>
#include <memory>
#include <string>

#include "export.h"
#include "reflection/class_factory.h"

namespace mao::serialization {
using namespace reflection;
using std::shared_ptr;
using std::string;

class MAO_SERIALIZATION_EXPORTS classJsonStream {
 public:
  classJsonStream() {}

  string to_json(const shared_ptr<metaObject> &objPtr);

  shared_ptr<metaObject> from_json(const string &jsonStr);
};
}  // namespace mao::serialization
#endif  // MAO_SRC_SERIALIZATION_CLASS_JSON_WRITER_H_
