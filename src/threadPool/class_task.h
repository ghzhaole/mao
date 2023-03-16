//
// Created by leo on 2023/3/15.
//

#ifndef MAO_SRC_THREADPOOL_CLASS_TASK_H_
#define MAO_SRC_THREADPOOL_CLASS_TASK_H_

#include <functional>
#include <string>
#include "export.h"
namespace mao {
namespace thread {
using std::function;
using std::string;
class MAO_THREAD_EXPORTS classTask {
 public:
  classTask() {};
  virtual ~classTask() {};
  virtual string description() { return "custom task"; };
  virtual void init() = 0;
  virtual void run() = 0;
  virtual void clear() = 0;
};

} // mao
} // thread

#endif //MAO_SRC_THREADPOOL_CLASS_TASK_H_
