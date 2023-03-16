//
// Created by leo on 2023/3/15.
//

#ifndef MAO_SRC_THREADPOOL_CLASS_THREAD_POOL_H_
#define MAO_SRC_THREADPOOL_CLASS_THREAD_POOL_H_

#include <set>
#include <thread>
#include <deque>
#include <mutex>
#include <condition_variable>

namespace mao {
namespace thread {
class classThread;
class classTask;

using std::set;
using std::thread;
using std::mutex;
using std::condition_variable;
class classThreadPool {
 public:
  classThreadPool(size_t count = 8);
  void create();
  ~classThreadPool();

  void move_to_wait(classThread *);
  void move_to_busy(classThread *);

  classThread *get_wait_thread();
  void stop();
  void run();

  void add_task(std::shared_ptr<classTask>);

 private:
  bool set_task(classTask *);
  bool set_task(std::shared_ptr<classTask>);

 private:
  size_t count_{8};

  set<classThread *> wait_;

  set<classThread *> busy_;

  set<classThread *> ths_;

  std::deque<std::shared_ptr<classTask>> tasks_;

  mutex wait_mutex_;

  mutex busy_mutex_;

  mutex task_mutex_;

  condition_variable condition_var_;

  thread* thread_{nullptr};

  std::atomic<bool> if_exit{false};

};

} // mao
} // thread

#endif //MAO_SRC_THREADPOOL_CLASS_THREAD_POOL_H_
