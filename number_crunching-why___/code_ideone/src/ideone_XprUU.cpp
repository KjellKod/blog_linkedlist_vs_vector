/* SMART Linked-list vs "normal" linked-list vs Vector. 15seconds maximum run-time 
Linear search and sorted insertion. std::list remembers last position to 
minimize overhead in traversing the list.*/

#include <list>
#include <vector>
#include <iostream>
#include <iomanip>
#include <random>
#include <functional>
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

namespace{
// A little bit smarter --- remembers the last insertion point's position and can 
// start from that if wanted-position is >= last-position: this is calculated from the values
// and not from the absolute positions
TimeValue linearSmartInsertPerformance(const NumbersInVector& numbers, NumbersInList& container)
{
    g2::StopWatch watch; 
    auto last_inserted_value = 0;
    auto last_iter_position = container.begin();

    std::for_each(numbers.begin(), numbers.end(),
                  [&](const Number& n)
    {
        auto itr = container.begin();
        if(n >= last_inserted_value)
        {
           itr = last_iter_position;
        }
           
        for (; itr!= container.end(); ++itr)
        {
            if ((*itr) >= n) {
                break;
            }
        }
        last_iter_position = container.insert(itr, n);
        last_inserted_value = n;
    });
    auto time = watch.elapsedUs().count();
    return time;
}
} // anonymous namespace

// Measure time in milliseconds for linear insert in a std container
template<typename Container>
TimeValue linearInsertPerformance(const NumbersInVector& randoms, Container& container)
{
    g2::StopWatch watch;
    linearInsertion(std::cref(randoms), container);
    auto time = watch.elapsedUs().count();
    return time;
}


void listVsVectorLinearPerformance(size_t nbr_of_randoms)
{
  // Generate n random values and push to storage
  NumbersInVector     values(nbr_of_randoms);
  std::for_each(values.begin(), values.end(), [&](Number& n) { n = random_int(0, nbr_of_randoms -1);}); 
  //print_distribution(values); 

  TimeValue smarter_list_time;
  TimeValue list_time;
  TimeValue vector_time;    
  std::cout << nbr_of_randoms << ",\t" << std::flush;


  { // force local scope - to clear up the containers at exit	
    NumbersInList      list;
    smarter_list_time = linearSmartInsertPerformance(values, list);
  }
  {
    NumbersInList      list;
    list_time = linearInsertPerformance(values, list);
  }
  {
    NumbersInVector    vector;
    vector_time = linearInsertPerformance(values, vector);
    std::cout << " "  << ", ";
  }
  std::cout <<  smarter_list_time << ", " << list_time << ", " << vector_time << std::endl << std::flush;
}


int main(int argc, char** argv)
{

std::cout << "\n\n********** Times in microseconds**********" << std::endl;
std::cout << "Elements ADD (Smarter-List,   List,    Vector)" << std::endl;

  g2::StopWatch watch; // only 15seconds available, therefore the short ranges
  listVsVectorLinearPerformance(10);
  listVsVectorLinearPerformance(100);
  listVsVectorLinearPerformance(500);
  listVsVectorLinearPerformance(1000);
  listVsVectorLinearPerformance(2000);
  listVsVectorLinearPerformance(3000);
  listVsVectorLinearPerformance(4000);
  listVsVectorLinearPerformance(6000);
  listVsVectorLinearPerformance(8000);
  listVsVectorLinearPerformance(16000);
  listVsVectorLinearPerformance(32000);
  auto total_time_ms = watch.elapsedMs().count();

  std::cout << "Exiting test,. the whole measuring took " << total_time_ms << " milliseconds";
  std::cout << " (" << total_time_ms/1000 << "seconds or " << total_time_ms/(1000*60) << " minutes)" << std::endl; 
   return 0;
}