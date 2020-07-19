#pragma once

#include <taskman/taskman_common.hpp>
#include <taskman/policy_prio.hpp>


namespace taskman {

class TaskManPrio : public TaskMan
{
  public:
    TaskManPrio(std::size_t capacity)
      : TaskMan(capacity, std::make_unique<PolicyPrio>())
    {}
};


}
