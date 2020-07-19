#pragma once

#include <iostream> /////////////////////////////////// REMOVE!!!!!!!!!!!!!!!!!!!!!!

namespace os
{

  // OS-defined process structure (according to the task description)
  class Process
  {
    public:
      using PID = int;
      enum class Prio { low, mid, high };

      Process(PID id, Prio prio)
        : m_id(id)
        , m_prio(prio)
      {}  
      
      PID  getID()   const { return m_id;   }
      Prio getPrio() const { return m_prio; }
      
      void kill() {
        std::cout << "Killing process with PID " << m_id << std::endl;
      }

    private:
      PID  m_id;
      Prio m_prio;
  };
  
  
} // namespace os
