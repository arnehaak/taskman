#pragma once

#include <taskman/policy_iface.hpp>


namespace taskman {


class PolicyBlocking : public PolicyIface
{
  public:
    std::optional<os::Process::PID> getPidToKill(os::Process const & newProcess) const override;
    void trackProcess(os::Process const & process) override;
    void untrackPids(std::vector<os::Process::PID> const & pids) override;
    void reset() override;
};


}
