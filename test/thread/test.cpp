//
// Created by leo on 2023/3/15.
//

#include <iostream>
#include "threadPool/class_thread_pool.h"
#include "threadPool/class_thread.h"
#include "threadPool/class_task.h"
#include <chrono>

using namespace std::chrono;

using namespace mao::thread;

mutex m;
class myTask : public classTask {
 public:
  myTask(int ii) : i(ii) {};
  void init() override {
  };
  void run() override {
    std::cout << "my task run " << i << std::endl;
    std::this_thread::sleep_for(10ms);
  };
  void clear() override {
  };
 private:
  int i{1};
};

int main() {
  classThreadPool pool(10);
  pool.create();
  for(int i=0;i<1000;i++)
    pool.add_task(std::make_shared<myTask>(i));

}