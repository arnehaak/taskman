#pragma once

#include <taskman/process.hpp>
#include <vector>
#include <optional>


namespace taskman {


class PolicyIface
{
  public:
    virtual std::optional<os::Process::PID> getPidToKill(os::Process const & newProcess) const = 0;
    virtual void trackProcess(os::Process const & process)                                     = 0;
    virtual void untrackPids(std::vector<os::Process::PID> const & pids)                       = 0;
    virtual void reset()                                                                       = 0;
};


}
