//
// Created by leo on 2023/3/15.
//

#ifndef MAO_SRC_THREADPOOL_CLASS_THREAD_H_
#define MAO_SRC_THREADPOOL_CLASS_THREAD_H_

#include <thread>
#include <condition_variable>
#include <atomic>
#include <string>
#include <mutex>
#include <memory>
#include "export.h"

namespace mao {
namespace thread {
class classTask;
class classThreadPool;

using std::thread;
using std::condition_variable;
using std::atomic;
using std::string;
using std::mutex;

class MAO_THREAD_EXPORTS classThread {
 public:
  classThread(classThreadPool* pool = nullptr);
  bool set_task(std::shared_ptr<classTask> );
  string get_task_description() const;
  auto get_id() const;
  void run();
  void stop();
  void join();
  ~classThread();

 private:
  void move_to_wait();
  void move_to_busy();

 private:
  classThreadPool* pool_{nullptr};

  std::shared_ptr<classTask> task_{nullptr};

  thread *thread_{nullptr};

  mutex mutex_;

  condition_variable condition_var_;

  atomic<bool> if_exit_{false};
};
}
}
#endif //MAO_SRC_THREADPOOL_CLASS_THREAD_H_
