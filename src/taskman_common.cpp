
#include <taskman/taskman_common.hpp>

#include <algorithm>
#include <stdexcept>
#include <utility>


namespace taskman {


namespace comparators
{
  using PDI = taskman::ProcessDisplayInfo;

  auto const idLT           = [](PDI const & lhs, PDI const & rhs) -> bool { return lhs.id           < rhs.id;           };
  auto const prioLT         = [](PDI const & lhs, PDI const & rhs) -> bool { return lhs.prio         < rhs.prio;         };
  auto const creationTimeLT = [](PDI const & lhs, PDI const & rhs) -> bool { return lhs.creationTime < rhs.creationTime; };
}


void TaskMan::add(os::Process const & process) {

  // Approximate process creation time by the current time, i.e., the time when
  // the process is added to the task manager
  std::time_t const creationTime = std::time(0);
  addWithCreationTime(process, creationTime);
}


void TaskMan::addWithCreationTime(os::Process const & process, std::time_t creationTime) {

  // Exclusive lock (this method modifies the internal state)
  std::unique_lock lock(m_sharedMutex);

  // The same process (identified via PID) must not be added multiple times
  if (m_processes.find(process.getID()) != m_processes.end()) {
    throw error::AddingProcFailed("already added (duplicate PID)");
  }

  // Check if capacity is exhausted and follow the policy in this case
  if (m_processes.size() == m_capacity) {
    std::optional<os::Process::PID> const pidToKill = m_policy->getPidToKill(process);
    
    if (pidToKill.has_value()) {
      terminateProcess(pidToKill.value());
    } else {
      throw error::AddingProcFailed("capacity exhausted");
    }
  }
  
  m_policy->trackProcess(process);
  m_processes.emplace(std::make_pair(process.getID(), ProcessManagementData{process, creationTime}));
}

  
std::vector<ProcessDisplayInfo> TaskMan::list(SortBy sortBy) const {

  // Shared lock (this method doesn't modify the internal state)
  std::shared_lock lock(m_sharedMutex);

  std::vector<ProcessDisplayInfo> procInfoVector;
  for (auto const & keyValPair : m_processes)
  {
    os::Process const & process      = keyValPair.second.process;
    std::time_t const   creationTime = keyValPair.second.creationTime;
    
    taskman::ProcessDisplayInfo pdi;
    pdi.id           = process.getID();
    pdi.prio         = process.getPrio();
    pdi.creationTime = creationTime;

    procInfoVector.push_back(pdi);
  }
    
  switch (sortBy) {
    case SortBy::id:
      std::sort(procInfoVector.begin(), procInfoVector.end(), comparators::idLT);
      break;
    case SortBy::prio:
      std::sort(procInfoVector.begin(), procInfoVector.end(), comparators::prioLT);
      break;
    case SortBy::creationTime:
      std::sort(procInfoVector.begin(), procInfoVector.end(), comparators::creationTimeLT);
      break;
    default:
      throw std::runtime_error("Invalid sort order!");
  }
        
  return procInfoVector;   
}


void TaskMan::kill(os::Process::PID pid) {

  // Exclusive lock (this method modifies the internal state)
  std::unique_lock lock(m_sharedMutex);

  terminateProcess(pid);
}


void TaskMan::killAll() {

  // Exclusive lock (this method modifies the internal state)
  std::unique_lock lock(m_sharedMutex);

  for (auto & keyValPair : m_processes) {
    os::Process & process = keyValPair.second.process;
    process.kill();
  }

  m_policy->reset();
  m_processes.clear();
}
  

void TaskMan::killPrio(os::Process::Prio prio) {

  // Exclusive lock (this method modifies the internal state)
  std::unique_lock lock(m_sharedMutex);

  // Determine all processes to kill
  std::vector<os::Process::PID> pidsToUntrack;
  for (auto & keyValPair : m_processes) {
    os::Process & process = keyValPair.second.process;
    if (process.getPrio() == prio) {
      pidsToUntrack.push_back(process.getID());
      process.kill();
    }
  }

  m_policy->untrackPids(pidsToUntrack);

  for (auto const pid : pidsToUntrack) {
    m_processes.erase(pid);
  }
}


void TaskMan::terminateProcess(os::Process::PID pid) {

  auto findIter = m_processes.find(pid);
  if (findIter == m_processes.end()) {
    throw error::KillingProcFailed("Process not found!");
  }

  os::Process & process = findIter->second.process;
  process.kill();

  m_policy->untrackPids({pid});
  m_processes.erase(findIter);
}
  
  
} // namespace taskman
