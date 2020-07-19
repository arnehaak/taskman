#pragma once

#include <taskman/policy_iface.hpp>
#include <taskman/heap.hpp>


namespace taskman {


class PolicyPrio : public PolicyIface
{
  public:
    std::optional<os::Process::PID> getPidToKill(os::Process const & newProcess) const override;
    void trackProcess(os::Process const & process) override;
    void untrackPids(std::vector<os::Process::PID> const & pids) override;
    void reset() override;

  protected:

    // Keep a min-heap of pid/priority pairs to determine the lowest priority process
    
    struct PidPrioPair {
      os::Process::PID  id;
      os::Process::Prio prio;
    };

    struct PidPrioPairComparatorPrioGT {
      bool operator ()(PidPrioPair const & lhs, PidPrioPair const & rhs) {
        return lhs.prio > rhs.prio;
      }
    };

    using PrioMinHeap = Heap<PidPrioPair, PidPrioPairComparatorPrioGT>;

    PrioMinHeap m_prioMinHeap;
};


}
