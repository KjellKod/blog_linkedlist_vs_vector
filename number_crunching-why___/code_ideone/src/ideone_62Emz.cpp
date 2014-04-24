// Vector vs LinkedList. Random element and linear traversal to 
// get to the right position. Insertion gives sorted order.

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


/*   // Lambda expression for generating a random number using the 'mersenne twister distribution'
     // http://en.wikipedia.org/wiki/Mersenne_twister
auto random_int = [&](const Number& low, const Number& high) -> Number {
    std::uniform_int_distribution<int> distribution(low, high);
    std::mt19937 engine((unsigned int)time(0)); // Mersenne twister MT19937
    auto generator = std::bind(distribution, engine);
    return generator();
};   */

// Random integer function from http://www2.research.att.com/~bs/C++0xFAQ.html#std-random
int random_int(int low, int high)
{
  using namespace std;

  static default_random_engine engine {};
  typedef uniform_int_distribution<int> Distribution;
  static Distribution distribution {};

  return distribution(engine, Distribution::param_type{low, high});
}


// test printout just to see the distribution
void print_distribution(std::vector<Number>& values)
{  
  for (size_t i = 0; i<values.size(); ++i) 
  {
    std::cout << i << '\t';
    for (size_t j=0; j<values[i]; ++j) std::cout << '*';
    std::cout << '\n';
  }
}




// Use a template approach to use functor, function pointer or lambda to insert an
// element in the input container and return the "time result".
// Search is LINEAR. Elements are insert in SORTED order
template<typename Container>
void linearInsertion(const NumbersInVector& numbers, Container& container)
{
    std::for_each(numbers.begin(), numbers.end(),
                  [&](const Number& n)
    {
        auto itr = container.begin();
        for (; itr!= container.end(); ++itr)
        {
            if ((*itr) >= n) {
                break;
            }
        }
        container.insert(itr, n);
    });
}

// Measure time in milliseconds for linear insert in a std container
template<typename Container>
TimeValue linearInsertPerformance(const NumbersInVector& randoms, Container& container)
{
    g2::StopWatch watch;
    linearInsertion(std::cref(randoms), container);
    auto time = watch.elapsedUs().count();
    return time;
}



// Delete of an element from a std container. The Delete of an item is from a random position.
template<typename Container>
void linearErase(Container& container)
{
    // Possibly O(n) to get initially but we use it for the random number generation
    //    and it is only done ONCE
    auto size = container.size();
    while (false == container.empty())
    {
        // force silly linear search to the right position to do a delete
        Number random_position = random_int(0, size -1);
        auto itr = container.begin();

        // using hand-wrought 'find' to force linear search to the position
        for (unsigned int idx = 0; idx != random_position; ++idx)
        {
            ++itr; // silly linear
        }
        container.erase(itr);
        --size;
    }
}

// Measure time in milliseconds for linear remove (i.e. "erase") in a std container
template<typename Container>
TimeValue linearRemovePerformance(Container& container)
{
    g2::StopWatch watch;
    linearErase(container);
    auto time = watch.elapsedUs().count();
    return time;
}


void listVsVectorLinearPerformance(size_t nbr_of_randoms)
{
  // Generate n random values and push to storage
  NumbersInVector     values(nbr_of_randoms);
  std::for_each(values.begin(), values.end(), [&](Number& n) { n = random_int(0, nbr_of_randoms -1);}); 
  //print_distribution(values); 

  TimeValue list_time;
  TimeValue list_delete_time;
  TimeValue vector_time;    
  TimeValue vector_delete_time;
  std::cout << nbr_of_randoms << ",\t" << std::flush;


  { // force local scope - to clear up the containers at exit	
    NumbersInList      list;
    list_time = linearInsertPerformance(values, list);
    list_delete_time = linearRemovePerformance(list);
  }
  {
    NumbersInVector    vector;
    vector_time = linearInsertPerformance(values, vector);
    std::cout << " "  << ", ";
    vector_delete_time = linearRemovePerformance(vector);
  }
  std::cout <<  list_time << ", " << vector_time << ",";
  std::cout << "\t\t" << list_delete_time << ", " << vector_delete_time << std::endl << std::flush;
}




// Used for debugging and verification, 
// normally disabled
template<typename Container>
void printValues(const std::string& msg, const Container& values)
{
  std::cout << msg << std::endl;
  std::for_each(values.begin(), values.end(),
    [&](const Number& n) { std::cout << " " << n << " "; }); 

  std::cout << "\n" << std::endl;
}



// disabled normally,. only used to verify during dev,time not test,time
template<typename ProspectLinear>
void verifyLinearSort(const NumbersInVector& valid_container, const ProspectLinear& container)
{
  auto toCheckItr = container.begin();
  std::for_each(valid_container.begin(), valid_container.end(),
    [&](const Number& n)
  {
    assert(toCheckItr != container.end());
    Number n2 = (*toCheckItr);
    if(n != n2)
    {
      printValues("\nTrue Linear should be: ", valid_container);
      printValues("\nError in comparison printing numbers: ", container);
      std::cout << "check error: " << n << "vs value: " << n2 << std::endl;
      assert(n == n2 && "not matching numbers");
    }
    ++toCheckItr;
  });
}



int main(int argc, char** argv)
{ 

  // Generate N random integers and insert them in its proper position in the numerical order using
  // LINEAR search
  std::cout << "Comparison of insert and erase (delete) of item from a linked list and vector (array)" << std::endl;
  std::cout << "Insert is done on random item, with linear search to get to the insert position" << std::endl;
  std::cout << "Erase is done on a random position within the available elements. Linear search to get to the position" << std::endl;
  std::cout << "\n\n Linked-list shows close to exponential time usage in contrast to the vectors almost linear time usage"<< std::endl;
  std::cout << "The HUGE difference is due to that a vector is sequential in memory and thereby is maximazing cache-line usage" << std::endl;
 std::cout << "The linked-list uses RAM much more and cache-line misses are maximized. " << std::endl;
 std::cout << "\n\n CONCLUSION: When comparing linked list and vector/array then the linear search COMPLETELY" << std::endl; 
 std::cout << "DOMINATES the time consumption. Cache-line friendly data structures show much promise in speed" << std::endl;
 std::cout << "The fact that a vector uses many memory shuffling when increasing, inserting deleting elements is of " << std::endl; 
std::cout << "LIMITED importance compared to the RAM access slow-down for a linked-list" << std::endl;

std::cout << "\nFor test results on Windows and Linux please go to: " << std::endl;
std::cout << "https://docs.google.com/spreadsheet/pub?key=0AkliMT3ZybjAdGJMU1g5Q0QxWEluWGRzRnZKZjNMMGc&output=html" << std::endl;

std::cout << "\n\n********** Times in microseconds**********" << std::endl;
std::cout << "Elements ADD (List, Vector)\tERASE(List, Vector)" << std::endl;
//[elements, linear add time [ms] [list, vector],    linear erase time[ms] [list, vector]" << std::endl;

  g2::StopWatch watch;
  listVsVectorLinearPerformance(100);
  listVsVectorLinearPerformance(200);
  listVsVectorLinearPerformance(500);
  listVsVectorLinearPerformance(1000);
  listVsVectorLinearPerformance(4000);
  listVsVectorLinearPerformance(10000);
  listVsVectorLinearPerformance(20000);
  listVsVectorLinearPerformance(40000);
  auto total_time_ms = watch.elapsedMs().count();

  std::cout << "Exiting test,. the whole measuring took " << total_time_ms << " milliseconds";
  std::cout << " (" << total_time_ms/1000 << "seconds or " << total_time_ms/(1000*60) << " minutes)" << std::endl; 
   return 0;
}
