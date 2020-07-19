
#include <taskman/taskman.hpp>
#include "text_ui.hpp"
#include "../test/src/test_sample_generator.hpp"

#include <iostream>
#include <ctime>



int main()
{
  std::size_t const capacity = 5;
  
  std::vector<std::unique_ptr<taskman::TaskMan>> taskmanagers;
  taskmanagers.push_back(std::make_unique<taskman::TaskMan>(capacity));
  taskmanagers.push_back(std::make_unique<taskman::TaskManFIFO>(capacity));
  taskmanagers.push_back(std::make_unique<taskman::TaskManPrio>(capacity));

  // Generate deterministic pseudo-random test samples
  std::size_t const testSetSize = (capacity * 3 + 1) / 2;
  test::SampleDataGenerator sdg;
  auto const samples = sdg.generateSamples(testSetSize);
  
  // Demonstrate behavior of all task manager types
  for (auto & tm : taskmanagers) {
    std::cout << typeid(*tm).name() << "\n";
    
    // Add a bunch of processes
    for (auto const & sample : samples) {
      
      std::cout << "Attempting to add process (" << TextUI::formatProcess(sample.process, sample.creationTime, false) << ")...\n";

      try {
        // tm->add(sample.process);

        // For testing purposes, add processes with a custom creation time.
        // Otherwise we would not see much of a difference here (all processes
        // would have pretty much exactly the same time, as this loop finishes
        // very quickly)
        tm->addWithCreationTime(sample.process, sample.creationTime);

      } catch (taskman::error::AddingProcFailed const & e) {
        std::cout << "AddingProcFailed: " << e.what() << "\n";
      }

      std::cout << "Resulting processes (sorted by creation time):";
      TextUI::printProcesses(tm->list(taskman::SortBy::creationTime));
    }    
    

    // Demonstrate sort orders
    std::cout << "Sorted by PID:";
    TextUI::printProcesses(tm->list(taskman::SortBy::id));

    std::cout << "Sorted by priority:";
    TextUI::printProcesses(tm->list(taskman::SortBy::prio));

    std::cout << "Sorted by creation time:";
    TextUI::printProcesses(tm->list(taskman::SortBy::creationTime));


    // Kill all processes with a certain priority
    std::cout << "Killing all MID priority processes...\n";
    tm->killPrio(os::Process::Prio::mid);
    std::cout << "Resulting processes (sorted by creation time):";
    TextUI::printProcesses(tm->list(taskman::SortBy::creationTime));

    // Kill all processes
    std::cout << "Killing all processes...\n";
    tm->killAll();
    std::cout << "Resulting processes (sorted by creation time):";
    TextUI::printProcesses(tm->list(taskman::SortBy::creationTime));
  }
}
