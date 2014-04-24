/* Hack of earlier code to compare vectors worst-case towards linked-list best case
I.e. 1) insertion of x elements at first position
     1b) The same but do for vector at last position
*/

#include <list>
#include <vector>
#include <iostream>
#include <iomanip>
#include <functional>
#include <random>
#include <iostream>
#include <chrono>
#include <string>
#include <numeric>
#include <algorithm>
#include <cassert>

namespace g2
{       
  typedef std::chrono::high_resolution_clock clock;
  typedef std::chrono::microseconds microseconds;
  typedef std::chrono::milliseconds milliseconds;

  clock::time_point now(){return clock::now();}

  microseconds intervalUs(const clock::time_point& t1, const clock::time_point& t0)
  {return std::chrono::duration_cast<microseconds>(t1 - t0);}

  milliseconds intervalMs(const clock::time_point& t1,const clock::time_point& t0)
  {return std::chrono::duration_cast<milliseconds>(t1 - t0);}


  class StopWatch
  {
    clock::time_point start_;
  public:
    StopWatch() : start_(clock::now()){}
    clock::time_point restart()         { start_ = clock::now(); return start_;}
    microseconds elapsedUs()            { return intervalUs(now(), start_);}
    milliseconds elapsedMs()            {return intervalMs(now(), start_);}
  };
} // g2


typedef unsigned int  Number;
typedef std::list<Number>           NumbersInList;
typedef std::vector<Number>         NumbersInVector;
typedef long long int               TimeValue;


int random_int(int low, int high)
{
  using namespace std;
  static default_random_engine engine;
  typedef uniform_int_distribution<int> Distribution;
  static Distribution distribution;
  return distribution(engine, Distribution::param_type(low, high));
}
template<typename Container>
void firstPositionInsertion(const NumbersInVector& numbers, Container& container)
{
    std::for_each(numbers.begin(), numbers.end(),
                  [&](const Number& n)
    {
        auto itr = container.begin();
        container.insert(itr, n);
    });
}

// For vector, push always at back position
TimeValue lastPositionInsertionPerformance(const NumbersInVector& numbers, 
                                            NumbersInVector& vector)
{
    g2::StopWatch watch;
    std::for_each(numbers.begin(), numbers.end(),
                  [&](const Number& n)
    {
	    vector.push_back(n);
    });

	auto time = watch.elapsedUs().count();
    return time;
}


// Measure time in milliseconds for linear insert in a std container
template<typename Container>
TimeValue linearInsertPerformance(const NumbersInVector& randoms, Container& container)
{
    g2::StopWatch watch;
    firstPositionInsertion(std::cref(randoms), container);
    auto time = watch.elapsedUs().count();
    return time;
}



void addPerformance(size_t nbr_of_randoms)
{
  // Generate n random values and push to storage
  NumbersInVector     values(nbr_of_randoms);
  std::for_each(values.begin(), values.end(), [&](Number& n) { n = random_int(0, nbr_of_randoms -1);}); 

  NumbersInVector vector_worst;
  NumbersInVector vector_best;
  NumbersInList   list;
  
      
  std::cout << nbr_of_randoms << ",  " << std::flush;
  TimeValue list_time = linearInsertPerformance(values, list);
  TimeValue vector_best_time = lastPositionInsertionPerformance(values, vector_best);
  TimeValue vector_worst_time = linearInsertPerformance(values, vector_worst);
  std::cout << list_time << ", " << vector_best_time << ", " << vector_worst_time << " " << std::endl << std::flush;
}






int main(int argc, char** argv)
{ 

  std::cout << "Perhaps the most common 'data collecting' operation is putting 'one piece of data" << std::endl;
  std::cout << "  in front of an older piece of data and so forth.\n" << std::endl; 
  std::cout << "For linked-list this is insertion at index 0\n" << std::endl;
  std::cout << "For std::vector  the 'push at front' operation is working against nature of the " << std::endl;
  std::cout  << "  data structure and in this common scenario such use is called 'naive'\n" << std::endl;
  std::cout << "For std::vector it is natural in this scenario to work in the direction of 'growth' " << std::endl; 
  std::cout << "i.e. push_back. It solves the same task but works as intended with the nature of the " << std::endl; 
  std::cout << "  data structure.\n\n" << std::endl; 

  std::cout << "elements, list, vector_best, vector_worst(naive)" << std::endl;
  g2::StopWatch watch;
  addPerformance(10);
  addPerformance(100);
  addPerformance(500);
  addPerformance(1000);
  addPerformance(2000);
  addPerformance(10000);
  addPerformance(20000);
  addPerformance(40000);
  addPerformance(100000);
  // more than this will timeout on ideone
   // addPerformance(500000);
  //addPerformance(1000000);

  auto total_time_ms = watch.elapsedMs().count();
  std::cout << "Exiting test,. the whole measuring took " << total_time_ms << " milliseconds";
  std::cout << " (" << total_time_ms/1000 << "seconds or " << total_time_ms/(1000*60) << " minutes)" << std::endl; 
   return 0;
}