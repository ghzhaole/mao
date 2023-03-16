//
// Created by leo on 2023/3/15.
//

#include "class_thread.h"
#include "class_task.h"
#include "class_thread_pool.h"
#include <iostream>
#include <chrono>

namespace mao {
namespace thread {
using namespace std::chrono;

classThread::classThread(classThreadPool *pool) : pool_(pool), if_exit_(false) {
  thread_ = new thread(&classThread::run, this);
}
bool classThread::set_task(std::shared_ptr<classTask> task) {
  if (!task_ && thread_) {
    move_to_busy();
    std::unique_lock<mutex> lock(mutex_);
    task_ = task;
    condition_var_.notify_all();
    return true;
  }
  return false;
}
string classThread::get_task_description() const {
  if (task_) {
    return task_->description();
  }
  return std::string();
}
void classThread::run() {
  while (true) {
    std::unique_lock<mutex> lock(mutex_);
    while (task_ == nullptr && if_exit_ == false) {
      condition_var_.wait_for(lock, 100ms);
    }
    if (task_ != nullptr) {
      task_->init();
      task_->run();
      task_->clear();
      task_.reset();
      move_to_wait();
    }
    if (if_exit_) { break; }
  }
  if_exit_ = false;
}
classThread::~classThread() {
  {
    std::unique_lock<mutex> lock(mutex_);
    if_exit_ = true;
    condition_var_.notify_all();
  }
  if (thread_ && thread_->joinable()) {
    thread_->join();
  }
  while (if_exit_) {}
  delete thread_;
}
void classThread::join() {
  if (thread_ && thread_->joinable()) {
    thread_->join();
  }
}
auto classThread::get_id() const {
  if (thread_) {
    return thread_->get_id();
  }
  return thread::id();
}
void classThread::move_to_wait() {
  if (pool_) {
    pool_->move_to_wait(this);
  }
}
void classThread::move_to_busy() {
  if (pool_) {
    pool_->move_to_busy(this);
  }
}
}
}