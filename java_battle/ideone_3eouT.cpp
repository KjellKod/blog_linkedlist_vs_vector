//
// Comparison of sort. std::vector vs std::list
//

#include <list>
#include <vector>
#include <iostream>
#include <functional>
#include <random>
#include <iostream>
#include <chrono>
#include <string>
#include <numeric>
#include <algorithm>

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




void listVsVectorSort(size_t nbr_of_randoms)
{
  std::uniform_int_distribution<int> distribution(0, nbr_of_randoms);
  std::mt19937 engine((unsigned int)time(0)); // Mersenne twister MT19937
  auto generator = std::bind(distribution, engine);
  NumbersInVector  vector(nbr_of_randoms);
  NumbersInList list;

  std::for_each(vector.begin(), vector.end(), [&](Number& n) { n = generator(); list.push_back(n); }    );
  TimeValue list_time;
  {  // list measure sort
    g2::StopWatch watch;
    list.sort();
    list_time = watch.elapsedUs().count();
  }
    TimeValue vector_time;    
  {  // vector measure sort
    g2::StopWatch watch;
    std::sort(vector.begin(), vector.end());
    vector_time = watch.elapsedUs().count();
  }

  std::cout <<  nbr_of_randoms << "\t\t, " << list_time << ", (" << (100*list_time/vector_time);
  std::cout <<  "%),\t\t" << vector_time << " , (" << (100*vector_time/vector_time) << "%)" << std::endl;
}







int main(int argc, char** argv)
{ 
std::cout << "\n\n********** Times in microseconds (us) **********" << std::endl;
std::cout << "Elements SORT(List, Vector)" << std::endl;
std::cout <<  "elements\t, list_time\t, vector_time" << std::endl;
g2::StopWatch watch;

       listVsVectorSort(10);
       listVsVectorSort(1000);
       listVsVectorSort(10000);
       listVsVectorSort(100000);
       listVsVectorSort(1000000);
       listVsVectorSort(10000000);

  auto total_time_ms = watch.elapsedMs().count();
  std::cout << "Exiting test,. the whole measuring took " << total_time_ms << "ms";
  std::cout << " (" << total_time_ms/1000 << "seconds or " << total_time_ms/(1000*60) << " minutes)" << std::endl; 
   return 0;
}