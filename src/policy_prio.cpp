
#include <taskman/policy_prio.hpp>
#include <unordered_map>


namespace taskman {


std::optional<os::Process::PID> PolicyPrio::getPidToKill(os::Process const & newProcess) const
{
  std::optional<os::Process::PID> retVal = std::nullopt;

  // Determine the PID and priority of the lowest-priority process
  if (m_prioMinHeap.size() > 0)
  {
    if (newProcess.getPrio() > m_prioMinHeap.top().prio)
    {
      retVal = m_prioMinHeap.top().id;
    }
  }

  return retVal;
}


void PolicyPrio::trackProcess(os::Process const & process)
{
  m_prioMinHeap.push({process.getID(), process.getPrio()});
}


void PolicyPrio::untrackPids(std::vector<os::Process::PID> const & pids)
{
  // Create a hash map of all PIDs to untrack, so we can check in O(1)
  // if a PID still requires tracking
  std::unordered_map<os::Process::PID, bool> pidUntrackMap;
  for (auto const pid : pids) {
    pidUntrackMap[pid] = true;
  }

  // Create a new vector of all PidPrioPairs that still require tracking
  std::vector<PidPrioPair> elementsRemaining;

  for (PidPrioPair const & pidPrioPair : m_prioMinHeap.getAllElements()) {
    if (pidUntrackMap.find(pidPrioPair.id) == pidUntrackMap.end()) {
      elementsRemaining.push_back(pidPrioPair);
    }
  }
  
  // Create a new heap of the remaining elements
  m_prioMinHeap = PrioMinHeap(elementsRemaining);
}


void PolicyPrio::reset() {
  m_prioMinHeap.clear();
}


} // namespace taskman
