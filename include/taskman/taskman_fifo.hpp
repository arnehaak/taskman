#pragma once

#include <taskman/taskman_common.hpp>
#include <taskman/policy_fifo.hpp>


namespace taskman {


class TaskManFIFO : public TaskMan
{
  public:
    TaskManFIFO(std::size_t capacity)
      : TaskMan(capacity, std::make_unique<PolicyFIFO>())
    {}
};


}
