#pragma once

#include <taskman/taskman_iface.hpp>
#include <taskman/process.hpp>
#include <taskman/policy_blocking.hpp>
#include <unordered_map>
#include <optional>
#include <memory>
#include <ctime>
#include <shared_mutex>


namespace taskman {


class TaskMan : public TaskManIface
{
  public:
    TaskMan(std::size_t capacity, std::unique_ptr<PolicyIface> policy = std::make_unique<PolicyBlocking>())
      : m_capacity(capacity)
      , m_policy(std::move(policy))
    {}
          
    void add(os::Process const & process) override;

    // Add a process with custom creation time. Useful for adding a process to the
    // task manager long after its creation. Also useful for testing.
    void addWithCreationTime(os::Process const & process, std::time_t creationTime);

    std::vector<ProcessDisplayInfo> list(SortBy sortBy) const override;

    // Kill process(es)
    void kill(os::Process::PID pid) override;      
    void killAll() override;
    void killPrio(os::Process::Prio prio) override;
      
  protected:
    // Mutex for reader writer locks
    mutable std::shared_mutex m_sharedMutex;

    std::size_t m_capacity;

    // Policy which defines how this task manager reacts when its capacity is exhausted
    std::unique_ptr<PolicyIface> m_policy;

    // Internally used process management structure
    struct ProcessManagementData {
      os::Process process;
      std::time_t creationTime; 
    };

    // Hash map to store all processes
    std::unordered_map<os::Process::PID, ProcessManagementData> m_processes;

    void terminateProcess(os::Process::PID pid);      
};

  
} // namespace taskman
