#pragma once

#include <taskman/process.hpp>
#include <vector>
#include <stdexcept>
#include <ctime>


namespace taskman
{
  // Options to sort in the list method
  enum class SortBy { creationTime, prio, id };
  
  // Struct that is used as a return type for the list functionality
  struct ProcessDisplayInfo {
    os::Process::PID  id;
    os::Process::Prio prio;
    std::time_t       creationTime;
  };
  
  
  // Exceptions that might occur while using the task manager interface
  namespace error
  {
    struct AddingProcFailed : public std::runtime_error
    {
      using std::runtime_error::runtime_error;
    };

    struct KillingProcFailed : public std::runtime_error
    {
      using std::runtime_error::runtime_error;
    };
  }
  

  // Public interface for all task managers
  class TaskManIface
  {
    public:
      virtual void add(os::Process const & process) = 0;

      virtual std::vector<ProcessDisplayInfo> list(SortBy sortBy) const = 0;

      virtual void kill(os::Process::PID pid)       = 0;
      virtual void killPrio(os::Process::Prio prio) = 0;
      virtual void killAll()                        = 0;
  };
  
  
} // namespace taskman
