// Compilation: 
// g++ -std=c++14 -Wall -Wextra -pedantic -g -O3 bit.cpp -o bit -mpopcnt
// Execution
// ./bit 64000000

// Preprocessor
#include <chrono>
#include <random>
#include <cstdint>
#include <iostream>
#include "bit.hpp"
using namespace bit;

// Makes a random vector of the provided size
template <class T>
std::vector<T> make_random_vector(std::size_t size, const T& seed = T())
{
    std::vector<T> v(size);
    std::random_device device;
    std::mt19937 engine(seed == T() ? device() : seed);
    std::uniform_int_distribution<T> distribution;
    for (std::size_t i = 0; i < size; ++i) {
        v[i] = distribution(engine);
    }
    return v;
}

// Makes a boolean vector from a vector of integers
template <class T>
std::vector<bool> make_bool_vector(const std::vector<T>& v)
{
    constexpr std::size_t digits = binary_digits<T>::value;
    const std::size_t n = v.size();
    const std::size_t bn = n * digits;
    std::vector<bool> bv(bn);
    std::size_t bi = 0;
    for (std::size_t i = 0; i < n; ++i) {
        for (std::size_t j = 0; j < digits; ++j) {
            bv[bi++] = bool(make_bit_reference(v[i], j));
        }
    }
    return bv;
}

// Benchmark vector bool
double benchmark_vector_bool(std::vector<bool>& v)
{
    typename std::chrono::high_resolution_clock::time_point tbegin;
    typename std::chrono::high_resolution_clock::time_point tend;
    std::chrono::duration<double> duration;
    volatile std::uintmax_t tmp = 0;
    auto first = std::begin(v);
    auto last = std::end(v);
    tbegin = std::chrono::high_resolution_clock::now();
    tmp = ::std::count(first, last, 0);
    tend = std::chrono::high_resolution_clock::now();
    duration = tend - tbegin;
    return duration.count() + tmp / std::numeric_limits<double>::max();
}

// Benchmark bit iterator loop
template <class T>
double benchmark_bit_iterator_loop(std::vector<T>& v)
{
    typename std::chrono::high_resolution_clock::time_point tbegin;
    typename std::chrono::high_resolution_clock::time_point tend;
    std::chrono::duration<double> duration;
    std::uintmax_t result = 0;
    volatile std::uintmax_t tmp = 0;
    tbegin = std::chrono::high_resolution_clock::now();
    for (auto&& element: v) {
        result += ::bit::count(
            make_bit_iterator(&element, 0), 
            make_bit_iterator(&element + 1, 0), 
            zero_bit
        );
    }
    tmp = result;
    tend = std::chrono::high_resolution_clock::now();
    duration = tend - tbegin;
    return duration.count() + tmp / std::numeric_limits<double>::max();
}

// Benchmark bit iterator
template <class T>
double benchmark_bit_iterator(std::vector<T>& v)
{
    typename std::chrono::high_resolution_clock::time_point tbegin;
    typename std::chrono::high_resolution_clock::time_point tend;
    std::chrono::duration<double> duration;
    volatile std::uintmax_t tmp = 0;
    auto first = make_bit_iterator(std::begin(v), 0);
    auto last = make_bit_iterator(std::end(v), 0);
    tbegin = std::chrono::high_resolution_clock::now();
    tmp = ::bit::count(first, last, zero_bit);
    tend = std::chrono::high_resolution_clock::now();
    duration = tend - tbegin;
    return duration.count() + tmp / std::numeric_limits<double>::max();
}

// Main function
int main(int argc, char* argv[])
{
    using uint_t = unsigned long long int;
    constexpr std::size_t digits = std::numeric_limits<uint_t>::digits;
    const std::size_t n = argc > 1 ? std::stoull(argv[1]) / digits : (1 << 22);
    const std::string digitsstr = std::to_string(digits);
    const std::string exceptstr = "the number of bits should be a multiple of";
    std::vector<uint_t> v = make_random_vector<uint_t>(n);
    std::vector<bool> bv = make_bool_vector(v);
    double vector_bool_duration = 0;
    double bit_iterator_loop_duration = 0;
    double bit_iterator_duration = 0;
    if (v.size() * digits != std::stoull(argv[1])) {
        throw std::invalid_argument(exceptstr + " " + digitsstr);
    }
    vector_bool_duration = benchmark_vector_bool(bv);
    bit_iterator_loop_duration = benchmark_bit_iterator_loop(v);
    bit_iterator_duration = benchmark_bit_iterator(v);
    std::cout<<"Counting bits set to 0 within "<<n * digits<<" bits"<<"\n";
    std::cout<<"Time: boolean vector = "<<vector_bool_duration<<"\n";
    std::cout<<"Time: bit iterator loop = "<<bit_iterator_loop_duration<<"\n";
    std::cout<<"Time: bit iterator = "<<bit_iterator_duration<<"\n";
    std::cout<<"Speedup: boolean vector / bit iterator = ";
    std::cout<<(vector_bool_duration/bit_iterator_duration)<<std::endl;
    std::cout<<"Overhead: bit iterator loop / bit iterator = ";
    std::cout<<(bit_iterator_loop_duration/bit_iterator_duration)<<std::endl;
    return 0;
}
