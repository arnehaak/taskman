
#include <taskman/policy_blocking.hpp>


namespace taskman {


std::optional<os::Process::PID> PolicyBlocking::getPidToKill(os::Process const &) const
{
  return std::nullopt;
}


void PolicyBlocking::trackProcess(os::Process const &)
{
  // Nothing to do
}


void PolicyBlocking::untrackPids(std::vector<os::Process::PID> const &)
{
  // Nothing to do
}


void PolicyBlocking::reset()
{
  // Nothing to do
}


} // namespace taskman
