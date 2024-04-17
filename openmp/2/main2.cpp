#include <iostream>
#include <omp.h>
#include <list>
#include <ctime>
#include <cstdlib>
#include <iterator>
#include <chrono>

#define NUM_THREADS 10
#define EPS 10e-9

void divide_by_first_openmp(std::list<double> &l)
{
    omp_set_num_threads(NUM_THREADS);

    double scale = l.front();
    auto end_iter = l.end();
    if (std::abs(scale) > EPS)
    {
        #pragma omp parallel
        {
            std::ios_base::sync_with_stdio(0);
            std::cin.tie(0); std::cout.tie(0);
            int cur_thread = omp_get_thread_num();
            
            auto cur_iter = l.begin();
            std::advance(cur_iter, cur_thread);
            while (true)
            {
                (*cur_iter) /= scale;
            
                if (std::distance(cur_iter, end_iter) > NUM_THREADS)
                    std::advance(cur_iter, NUM_THREADS); 
                else
                    break;
            }  
        }    
    }
    
    return;
}


void divide_by_first_cons(std::list<double> &l)
{
    double scale = l.front();
    if (std::abs(scale) > EPS)
        for (double &i: l)
            i /= scale;
    return;
}

int main(void)
{
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0); std::cout.tie(0);
    srand((unsigned int)time(0));

    std::list<double> l;

    l.push_back(17);
    for (int i = 0; i < 100000; ++i)
        l.push_back(rand() % 100 - 50);

    /*
    for (double &i: l)
        std::cout << i << " ";
    std::cout << "\n";
    */

    auto beg = std::chrono::high_resolution_clock::now();
    divide_by_first_openmp(l);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> ms = end - beg;
    std::cout << "openmp time: " << ms.count() << "ms" << std::endl;

    beg = std::chrono::high_resolution_clock::now();
    divide_by_first_cons(l);
    end = std::chrono::high_resolution_clock::now();
    ms = end - beg;
    std::cout << "  cons time: " << ms.count() << "ms" << std::endl;

    /*
    for (double &i: l)
        std::cout << i << " ";
    std::cout << "\n";
    */
    
    return 0;
}