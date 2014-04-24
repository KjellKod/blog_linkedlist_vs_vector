#include <iostream>
#include "g2_chrono.h"
#include "linear_performance.h"


// from  Wikipedia C++11 Random 
// http://en.wikipedia.org/wiki/C%2B%2B11#Extensible_random_number_facility
// more details at http://www.codeguru.com/cpp/cpp/cpp_mfc/stl/article.php/c15319


int main(int argc, char** argv)
{ 

  // Generate N random integers and insert them in its proper position in the numerical order using
  // LINEAR search
  std::cout << "Comparison of insert and erase (delete) of item from a linked list and vector (array)" << std::endl;
  std::cout << "Insert is done on random item, with linear search to get to the insert position" << std::endl;
  std::cout << "Erase is done on a random position within the available elements. Linear search to get to the position" << std::endl;
  std::cout << "\n\n Linked-list shows exponential time usage in contrast to the vectors almost linear time usage"<< std::endl;
  std::cout << "The HUGE difference is due to that a vector is sequential in memory and thereby is maximazing cache-line usage" << std::endl;
  std::cout << "The linked-list uses RAM much more and cache-line misses are maximized. " << std::endl;
  std::cout << "\n\n CONCLUSION: When comparing linked list and vector/array then the linear search COMPLETELY" << std::endl; 
  std::cout << "DOMINATES the time consumption. Cache-line friendly data structures show much promise in speed" << std::endl;
  std::cout << "The fact that a vector uses many memory shuffling when increasing, inserting deleting elements is of " << std::endl; 
  std::cout << "LIMITED importance compared to the RAM access slow-down for a linked-list" << std::endl;

  std::cout << "\nFor test results on Windows and Linux please go to: " << std::endl;
  std::cout << "https://docs.google.com/spreadsheet/pub?key=0AkliMT3ZybjAdGJMU1g5Q0QxWEluWGRzRnZKZjNMMGc&output=html" << std::endl;

  std::cout << "\n\n********** Times in milliseconds **********" << std::endl;
  g2::StopWatch watch;
  // Generate N random integers and insert them in its proper position in the numerical order using
  // LINEAR search
  std::cout << "[elements, linear add time [ms] [list, vector],    linear erase time[ms] [list, vector]" << std::endl;
  listVsVectorLinearPerformance(10);
  listVsVectorLinearPerformance(100);
  listVsVectorLinearPerformance(1000);
  listVsVectorLinearPerformance(10000);
  listVsVectorLinearPerformance(20000);  
  listVsVectorLinearPerformance(40000);
  size_t cnt = 50000;
  g2::StopWatch w2;
  do{
    w2.restart();
    listVsVectorLinearPerformance(cnt);
    auto t = w2.elapsedMs().count(); 
    std::cout << cnt << " items took " << t/1000 << " seconds or ";
    std::cout << t/ 60000  << " minutes\n" << std::endl;
    cnt+=50000; 
  }
  while(cnt <= 500000); 
  auto total_time_ms = watch.elapsedMs().count();

  std::cout << "Exiting test,. the whole measuring took " << total_time_ms << "ms";
  std::cout << " (" << total_time_ms/1000 << "seconds or " << total_time_ms/(1000*60) << " minutes)" << std::endl; 
  return 0;
}
