// ================================== BIT =================================== //
// Project:         The C++ Bit Library
// Name:            bit.hpp
// Description:     Illustrates the use of the bit library
// Creator:         Vincent Reverdy
// Contributor(s):  Vincent Reverdy [2015-2017]
// License:         BSD 3-Clause License
// ========================================================================== //
// Compilation: 
// g++ -std=c++14 -Wall -Wextra -pedantic -g -O3 bit.cpp -o bit -mpopcnt
// Execution
// ./bit 64000000
// ========================================================================== //



// ================================ PREAMBLE ================================ //
// C++ standard library
#include <chrono>
#include <cstdio>
#include <limits>
#include <random>
#include <iostream>
// Project sources
#include "bit.hpp"
// Third-party libraries
// Miscellaneous
using namespace bit;
// ========================================================================== //



// ================================== BODY ================================== //
// Display algorithm
template <class O, class Iterator> 
void display(O&& os, bit_iterator<Iterator> first, bit_iterator<Iterator> last)
{
    auto mem = first;
    for (auto it = first; it != last; ++it) {
        if (it.base() != mem.base()) {
            os << " ";
        }
        std::forward<O>(os) << *it;
        mem = it;
    }
    std::forward<O>(os) << std::endl;
}

// Make random vector
template <class T>
std::vector<T> make_random_vector(
    std::size_t size, 
    T min = std::numeric_limits<T>::min(), 
    T max = std::numeric_limits<T>::max(), 
    const T& seed = T()
)
{
    std::vector<T> v(size);
    std::random_device device;
    std::mt19937 engine(seed == T() ? device() : seed);
    std::uniform_int_distribution<std::uintmax_t> distribution(min, max);
    for (std::size_t i = 0; i < size; ++i) {
        v[i] = distribution(engine);
    }
    return v;
}

// Benchmark the function on the vector
template <class T, class F, class R>
double benchmark(std::vector<T> v, F&& f, R&& result)
{
    typename std::chrono::high_resolution_clock::time_point tbegin;
    typename std::chrono::high_resolution_clock::time_point tend;
    std::chrono::duration<double> duration;
    volatile std::uintmax_t tmp = 0;
    tbegin = std::chrono::high_resolution_clock::now();
    std::forward<F>(f)(std::begin(v), std::end(v));
    tend = std::chrono::high_resolution_clock::now();
    result = std::accumulate(std::begin(v), std::end(v), T{}); 
    tmp = std::forward<R>(result) + tmp;
    duration = tend - tbegin;
    return duration.count();
}

// Main function
int main(int argc, char* argv[])
{
    // Initialization
    using uint_t = unsigned long long int;
    constexpr std::size_t byte = std::numeric_limits<unsigned char>::digits;
    constexpr std::size_t digits = sizeof(uint_t) * byte;
    const std::size_t n = argc > 1 ? std::stoull(argv[1]) : (1 << 22);
    auto v = make_random_vector<uint_t>(n);
    auto vb = make_random_vector<bool>(n * digits, 0, 1);
    std::uintmax_t k = 0;
    
    // Functions
    auto f0 = [=](auto first, auto last){
        auto bfirst = bit_iterator<decltype(first)>(first);
        auto blast = bit_iterator<decltype(last)>(last);
        reverse(bfirst, blast);
    };
    auto f1 = [=](auto first, auto last){
        for (auto it = first; it != last; ++it) {
            auto bfirst = bit_iterator<decltype(it)>(it);
            auto blast = bit_iterator<decltype(it)>(it + 1);
            reverse(bfirst, blast);
        }
    };
    auto f2 = [=](auto first, auto last){
        for (auto it = first; it != last; ++it) {
            *it = _bitswap(*it);
        }
    };
    auto f3 = [=](auto first, auto last){
        for (auto it = first; it != last; ++it) {
            auto bfirst = bit_iterator<decltype(it)>(it) + 7;
            auto blast = bit_iterator<decltype(it)>(it + 1);
            reverse(bfirst, blast);
        }
    };
    auto f4 = [=](auto first, auto last){
        for (auto it = first; it != last; ++it) {
            auto bfirst = bit_iterator<decltype(it)>(it) + 7;
            auto blast = bit_iterator<decltype(it)>(it + 1) - 13;
            reverse(bfirst, blast);
        }
    };
    auto f5 = [=](auto first, auto last){
        for (auto it = first; it != last; ++it) {
            auto bfirst = bit_iterator<decltype(it)>(it);
            auto blast = bit_iterator<decltype(it)>(it + 1) - 13;
            reverse(bfirst, blast);
        }
    };
    auto f6 = [=](auto first, auto last){
        std::reverse(first, last);
    };
    
    // Benchmark
    std::cout<<"f0 = "<<benchmark(v, f0, k = 0)<<" ";
    std::cout<<k<<std::endl;
    std::cout<<"f1 = "<<benchmark(v, f1, k = 0)<<" ";
    std::cout<<k<<std::endl;
    std::cout<<"f2 = "<<benchmark(v, f2, k = 0)<<" ";
    std::cout<<k<<std::endl;
    std::cout<<"f3 = "<<benchmark(v, f3, k = 0)<<" ";
    std::cout<<k<<std::endl;
    std::cout<<"f4 = "<<benchmark(v, f4, k = 0)<<" ";
    std::cout<<k<<std::endl;
    std::cout<<"f5 = "<<benchmark(v, f5, k = 0)<<" ";
    std::cout<<k<<std::endl;
    std::cout<<"f6 = "<<benchmark(vb, f6, k = 0)<<" ";
    std::cout<<k<<std::endl;
    
    // Finalization
    return static_cast<int>(k);
}
// ========================================================================== //
