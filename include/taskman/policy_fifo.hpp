#pragma once

#include <taskman/policy_iface.hpp>
#include <list>


namespace taskman {


class PolicyFIFO : public PolicyIface
{
  public:
    std::optional<os::Process::PID> getPidToKill(os::Process const & newProcess) const override;
    void trackProcess(os::Process const & process) override;
    void untrackPids(std::vector<os::Process::PID> const & pids) override;
    void reset() override;
  protected:
    std::list<os::Process::PID> m_pidsByAge;
};


}
 