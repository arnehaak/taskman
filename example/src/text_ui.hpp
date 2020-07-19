#pragma once

#include <taskman/taskman_iface.hpp>

#include <iostream>
#include <iomanip>
#include <string>
#include <ctime>
#include <sstream>


class TextUI
{
  public:

    static std::string formatDateTime(std::time_t const & datetime, std::string formatStr = "%Y-%m-%d %H:%M:%S")
    {
      char buf[100];
      std::strftime(buf, sizeof(buf), formatStr.c_str(), std::localtime(&datetime));
      return std::string(buf);
    }

    static std::string formatProcess(os::Process process, std::time_t creationTime, bool aligned = true)
    {
      taskman::ProcessDisplayInfo pdi;
      pdi.id           = process.getID();
      pdi.prio         = process.getPrio();
      pdi.creationTime = creationTime;
      return formatProcess(pdi, aligned);
    }
    
    static std::string formatProcess(taskman::ProcessDisplayInfo pdi, bool aligned = true)
    {
      std::stringstream ss;

      // PID
      if (aligned) {
        ss << std::right << std::setfill(' ') << std::setw(6);
      }
      ss << pdi.id << "  ";

      // Priority
      if (aligned) {
        ss << std::left << std::setfill(' ') << std::setw(4);
      }
      ss << ((pdi.prio == os::Process::Prio::low) ? "LOW" : ((pdi.prio == os::Process::Prio::high) ? "HIGH" : "MID")) << "  ";

      // Time
      ss << formatDateTime(pdi.creationTime);

      return ss.str();
    }

    static void printProcesses(std::vector<taskman::ProcessDisplayInfo> const & processDisplayList)
    {
      std::cout << "\n";
      for (auto const & processDisplayInfo : processDisplayList) {
        std::cout << formatProcess(processDisplayInfo) << "\n";
      }    
      std::cout << "\n\n";
    }
  
};
