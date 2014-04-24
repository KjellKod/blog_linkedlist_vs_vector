//
// compare linear insert with different POD sizes. std::list vs std::vector
//

#include <list>
#include <vector>
#include <deque>
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
typedef long long int            TimeValue;
const std::string rows_explained = "elements         list_time   vector_time   deque_time ";


// Silly POD to test with variadic POD size
template<Number Size>
struct POD
{
   Number a[Size];

  bool operator>=(const POD& b) const
  {
    return (this->a[0] >= b.a[0]);
  }
};


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
template<typename ValueType, typename Container>
void linearInsertion(const std::vector<ValueType>& numbers, Container& container)
{
    std::for_each(numbers.begin(), numbers.end(),
                  [&](const ValueType& n)
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

// Measure time in microseconds (us) for linear insert in a std container
template<typename Container, typename ValueType>
TimeValue linearInsertPerformance(const std::vector<ValueType>& randoms, Container& container)
{
    g2::StopWatch watch;
    linearInsertion<ValueType, Container>(std::cref(randoms), container);
    auto time = watch.elapsedUs().count();
    return time;
}

template<Number SizeOfPod>
void listVsVectorLinearPerformance(const size_t nbr_of_randoms)
{
  // Generate n random values and push to storage
  typedef POD<SizeOfPod> POD_value;
  std::vector<POD_value> values(nbr_of_randoms);
  const auto lower_limit = 0;
  const auto upper_limit = nbr_of_randoms -1;
  std::for_each(values.begin(), values.end(), [&](POD_value& n) { n.a[0] = random_int(lower_limit, upper_limit);});

  TimeValue list_time;
  TimeValue vector_time;
  TimeValue deque_time;
  std::cout << nbr_of_randoms << ",\t" << std::flush;
  { // force local scope - to clear up the containers at exit
    std::list<POD_value>      list;
    list_time = linearInsertPerformance<std::list<POD_value>, POD_value>(values, list);
  }
  {
    std::vector<POD_value>    vector;
    vector_time = linearInsertPerformance<std::vector<POD_value>, POD_value>(values, vector);
  }

  {
    std::deque<POD_value>    deque;
    deque_time = linearInsertPerformance<std::deque<POD_value>, POD_value>(values, deque);
  }


  std::cout << "\t" << list_time << ",\t" << vector_time << ",\t" << deque_time;
  std::cout << ",\tsizeof(POD): " << sizeof(POD_value) << " bytes" << std::endl << std::flush;

}

   template<Number PodSizeIn4ByteIncrements>
   void measure()
   {
     g2::StopWatch watch;
     std::cout << "Measuring In Microseconds (us)" << std::endl;
     std::cout << rows_explained << std::endl;

     // small increments for measuring up to 4000
     listVsVectorLinearPerformance<PodSizeIn4ByteIncrements>(100);
     listVsVectorLinearPerformance<PodSizeIn4ByteIncrements>(200);
     listVsVectorLinearPerformance<PodSizeIn4ByteIncrements>(400);
     listVsVectorLinearPerformance<PodSizeIn4ByteIncrements>(800);
     listVsVectorLinearPerformance<PodSizeIn4ByteIncrements>(1000);
     listVsVectorLinearPerformance<PodSizeIn4ByteIncrements>(2000);
     listVsVectorLinearPerformance<PodSizeIn4ByteIncrements>(3000);
     listVsVectorLinearPerformance<PodSizeIn4ByteIncrements>(4000);
     listVsVectorLinearPerformance<PodSizeIn4ByteIncrements>(5000);

     for(auto cnt = 5000; cnt <= 11000; cnt+=2000)
     {
       listVsVectorLinearPerformance<PodSizeIn4ByteIncrements>(cnt);
     }
     auto total_time_ms = watch.elapsedMs().count();
     std::cout << "[" << rows_explained << "]" << std::endl;

     typedef POD<PodSizeIn4ByteIncrements> POD_value;
     std::cout << "Test finished for " << sizeof(POD_value) << " bytes POD" << std::endl;
     std::cout << "The POD sized test took " << total_time_ms << " milliseconds (or " << total_time_ms/1000 << " seconds)\n\n" << std::endl;
   }



   int main(int argc, char** argv)
   {
     g2::StopWatch watch;
     measure<1>(); // measure 4 bytes
     measure<2>(); // measure 8 bytes
     measure<4>(); // measure 16 bytes
     measure<8>(); // 32 bytes
     measure<16>(); // 64 bytes
     measure<32>(); // 128 bytes*/
     measure<64>(); // 256 bytes
     auto total_time_s = watch.elapsedMs().count()/1000;
     std::cout << "\n\n**********************************************\n" << std::endl;
     std::cout << "Exiting test: the whole measuring took " << total_time_s << " seconds";
     std::cout << " (or " << total_time_s/(60) << " minutes)" << std::endl;

     return 0;
   }


