
#include <taskman/policy_fifo.hpp>
#include <unordered_map>


namespace taskman {


std::optional<os::Process::PID> PolicyFIFO::getPidToKill(os::Process const & newProcess) const {

  // Determine the PID of the oldest process
  std::optional<os::Process::PID> retVal = std::nullopt;
  
  if (m_pidsByAge.size() > 0)
  {
    // Get the oldest PID
    retVal = m_pidsByAge.front();
  }
  
  return retVal;
}


void PolicyFIFO::trackProcess(os::Process const & process) {
    m_pidsByAge.push_back(process.getID());
}


void PolicyFIFO::untrackPids(std::vector<os::Process::PID> const & pids) {

  // Create a hash map of all PIDs to untrack, so we can check in O(1)
  // if a PID still requires tracking
  std::unordered_map<os::Process::PID, bool> pidUntrackMap;
  for (auto const pid : pids) {
    pidUntrackMap[pid] = true;
  }
 
  // Iterate over all tracked PIDs and remove those that need to be killed
  for (auto it = m_pidsByAge.begin(); it != m_pidsByAge.end(); ) {
      if (pidUntrackMap.find(*it) != pidUntrackMap.end()) {
          it = m_pidsByAge.erase(it);
      } else {
          ++it;
      }
  }    
}


void PolicyFIFO::reset()
{
  m_pidsByAge.clear();
}


} // namespace taskman
