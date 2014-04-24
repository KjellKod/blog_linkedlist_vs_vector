#ifndef LINEAR_PERFORMANCE_H_
#define LINEAR_PERFORMANCE_H_

#define SERIAL_RUN 1
// Comment away SERIAL_RUN in case you want a less exact but faster run
// in case of testing for huge numbers this will speed up execution somewhat if running 
// on a multicore --- IMPORTANT: If you do not have std::thread support then you should
// instead use the SERIAL_RUN


#include <list>
#include <vector>
#include <iostream>
#include <iomanip>
#include <random>
#include <string>
#include <numeric>
#include <cassert>
#include <algorithm>


typedef unsigned int  Number;
typedef std::list<Number>           NumbersInList;
typedef std::vector<Number>         NumbersInVector;
typedef long long int               TimeValue;




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
    auto time = watch.elapsedMs().count();
    return time;
}



// Generate a random number using the 'mersenne twister distribution'
// http://en.wikipedia.org/wiki/Mersenne_twister
// Random numbers are chosen within the range limits of 'low' and 'high'
auto randomNumber = [&](const Number& low, const Number& high) -> Number {
    std::uniform_int_distribution<int> distribution(low, high);
    std::mt19937 engine((unsigned int)time(0)); // Mersenne twister MT19937
    auto generator = std::bind(distribution, engine);
    return generator();
};


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
        Number random_position = randomNumber(0, size -1);
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
    auto time = watch.elapsedMs().count();
    return time;
}






void listVsVectorLinearPerformance(size_t nbr_of_randoms)
{
    std::uniform_int_distribution<int> distribution(0, nbr_of_randoms);
    std::mt19937 engine((unsigned int)time(0)); // Mersenne twister MT19937
    // alternatively:
    // std::random_device rd;
    // std::mt19937 engine(rd()); // Mersenne twister MT19937

    auto generator = std::bind(distribution, engine);
    // Generate n random values and push to storage
    NumbersInVector     values(nbr_of_randoms);
    std::for_each(values.begin(), values.end(), [&](Number& n) {
        n = generator();
    });
    TimeValue list_time;
    TimeValue list_delete_time;
    TimeValue vector_time;
    TimeValue vector_delete_time;
    std::cout << nbr_of_randoms << ",\t" << std::flush;
#ifdef SERIAL_RUN
    // ---- START SERIAL
    // UNCOMMENT THIS IF YOU want to run concurrent    
     NumbersInList    list;
     list_time = linearInsertPerformance(values, list);
     NumbersInVector    vector;
     vector_time = linearInsertPerformance(values, vector);
     // Random delete
     list_delete_time = linearRemovePerformance(list);
     vector_delete_time = linearRemovePerformance(vector);
// --- STOP SERIAL
#else     
// ---- START UNCOMMENT in case you do not have std::thread
// ---- or want more "exact" measurements. Running some jobs in parallell
// ---- can create some unwanted side effects which is espacially seen 
// ---- for low numbers
    // Insert to list and erase from list are the time consuming operations
    // Splitting them into concurrent tasks will speed up the total execution time
    // 1. Random Insert into list,. possibley done in another thread, copying 'values'
    // so that if possible avoid false sharing
    NumbersInList      list;
    auto future_list_time = std::async([=, &list]()->TimeValue {return linearInsertPerformance(values, list);});

    // Random delete from list,. possibly done in another thread
    NumbersInList list_to_delete; // 1. create the list to delete
    std::for_each(values.begin(), values.end(),
    [&](const Number& n) {
        list_to_delete.push_front(n);
    });
    // then empty the list
    auto future_list_delete_time = std::async(
                                       [&]()->TimeValue {return  linearRemovePerformance(list_to_delete);});


    // Faster operations: Random Insert/Erase of items to/from Vector, done in foreground
    NumbersInVector    vector;
    vector_time = linearInsertPerformance(std::cref(values), vector);
    vector_delete_time = linearRemovePerformance(vector);


    list_time = future_list_time.get(); // sync with the list insert
    list_delete_time = future_list_delete_time.get(); // and the list delete
// ----
// ---- end parallell running
// ----
#endif
    
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
    [&](const Number& n) {
        std::cout << " " << n << " ";
    });

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
        if (n != n2)
        {
            printValues("\nTrue Linear should be: ", valid_container);
            printValues("\nError in comparison printing numbers: ", container);
            std::cout << "check error: " << n << "vs value: " << n2 << std::endl;
            assert(n == n2 && "not matching numbers");
        }
        ++toCheckItr;
    });
}


#endif
