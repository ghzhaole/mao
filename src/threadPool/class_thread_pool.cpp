//
// Created by leo on 2023/3/15.
//

#include "class_thread_pool.h"
#include "class_thread.h"
#include "class_task.h"
#include <chrono>
#include <iostream>
namespace mao {
namespace thread {
using namespace std::chrono;
classThreadPool::classThreadPool(size_t count) : count_(count) {
}
classThreadPool::~classThreadPool() {
  if (thread_) {
    {
      std::unique_lock<mutex> lock2(task_mutex_);
      if_exit = true;
      condition_var_.notify_all();
    }
    if (thread_->joinable()) {
      thread_->join();
    }
    while (if_exit) {}
    delete thread_;
  }
  for (auto th : ths_) {
    delete th;
  }

}
void classThreadPool::move_to_busy(classThread *th) {
  {
    std::unique_lock<mutex> lock(busy_mutex_);
    if (busy_.contains(th)) {
      busy_.insert(th);
    }
  }
  {
    std::unique_lock<mutex> lock(wait_mutex_);
    auto finder = wait_.find(th);
    if (finder != wait_.end()) {
      wait_.erase(finder);
    }
  }
}
void classThreadPool::move_to_wait(classThread *th) {
  {
    std::unique_lock<mutex> lock(wait_mutex_);
    if (!wait_.contains(th)) {
      wait_.insert(th);
    }
  }
  {
    std::unique_lock<mutex> lock(busy_mutex_);
    auto finder = busy_.find(th);
    if (finder != busy_.end()) {
      busy_.erase(finder);
    }
  }
}
classThread *classThreadPool::get_wait_thread() {
  std::unique_lock<mutex> lock(wait_mutex_);
  if (wait_.empty()) {
    return nullptr;
  }
  return *wait_.begin();
}
void classThreadPool::stop() {
  if (thread_) {
    {
      std::unique_lock<mutex> lock2(task_mutex_);
      tasks_.clear();
      if_exit = true;
      condition_var_.notify_all();
    }

    if (thread_->joinable()) {
      thread_->join();
    }
    while (if_exit) {}
    delete thread_;
    thread_ = nullptr;
  }
  for (auto th : ths_) {
    delete th;
  }
  ths_.clear();
  busy_.clear();
  wait_.clear();
}
void classThreadPool::create() {
  std::unique_lock<mutex> lock(wait_mutex_);
  std::unique_lock<mutex> lock1(busy_mutex_);
  size_t left_size = count_ - wait_.size() - busy_.size();
  for (size_t i = 0; i < left_size; i++) {
    auto th = new classThread(this);
    ths_.insert(th);
    wait_.insert(th);
  }
  if (!thread_) {
    thread_ = new thread(&classThreadPool::run, this);
  }
}
bool classThreadPool::set_task(classTask *task) {
  return set_task(std::shared_ptr<classTask>(task));
}
bool classThreadPool::set_task(std::shared_ptr<classTask> task) {
  if (!task)
    return false;
  if (auto th = get_wait_thread()) {
    th->set_task(task);
    return true;
  }
  return false;
}
void classThreadPool::add_task(std::shared_ptr<classTask> task) {
  std::unique_lock<mutex> lock(task_mutex_);
  tasks_.push_back(task);
  condition_var_.notify_all();
}
void classThreadPool::run() {
  while (true) {
    {
      std::unique_lock<mutex> lock(task_mutex_);
      while (tasks_.empty() && !if_exit) {
        condition_var_.wait_for(lock, 100ms);
      }
    }

    while (true) {
      std::unique_lock<mutex> lock(task_mutex_);
      if (tasks_.empty()) { break; }
      if (auto th = get_wait_thread()) {
        th->set_task(tasks_[0]);
        tasks_.pop_front();
      }
    }
    if (if_exit) { break; }
  }
  if_exit = false;
}
} // mao
} // thread