// ============================== BIT DETAILS =============================== //
// Project:         The C++ Bit Library
// Name:            bit_details.hpp
// Description:     Provides common implementation details and helper classes
// Creator:         Vincent Reverdy
// Contributor(s):  Vincent Reverdy [2015-2016]
// License:         BSD 3-Clause License
// ========================================================================== //
#ifndef _BIT_DETAILS_HPP_INCLUDED
#define _BIT_DETAILS_HPP_INCLUDED
// ========================================================================== //



// ================================ PREAMBLE ================================ //
// C++ standard library
#include <tuple>
#include <limits>
#include <cassert>
#include <cstdint>
#include <utility>
#include <iterator>
#include <algorithm>
#include <type_traits>
// Project sources
// Third-party libraries
// Miscellaneous
namespace bit {
class bit_value;
template <class UIntType> class bit_reference;
template <class UIntType> class bit_pointer;
template <class Iterator> class bit_iterator;
// ========================================================================== //



/* ***************************** BINARY DIGITS ****************************** */
// Binary digits structure definition
template <class UIntType>
struct binary_digits 
: std::integral_constant<std::size_t, std::numeric_limits<UIntType>::digits> 
{
    // Assertions
    static_assert(std::is_integral<UIntType>::value, "");
    static_assert(std::is_unsigned<UIntType>::value, "");
    static_assert(!std::is_same<UIntType, bool>::value, "");
};

// Binary digits value
template <class UIntType>
constexpr std::size_t binary_digits_v = binary_digits<UIntType>::value;
/* ************************************************************************** */



/* *************** IMPLEMENTATION DETAILS: CV ITERATOR TRAITS *************** */
// Cv iterator traits structure definition
template <class Iterator>
struct _cv_iterator_traits
{
    // Assertions
    private:
    using _traits_t = std::iterator_traits<Iterator>;
    using _difference_t = typename _traits_t::difference_type;
    using _value_t = typename _traits_t::value_type;
    using _pointer_t = typename _traits_t::pointer;
    using _reference_t = typename _traits_t::reference;
    using _category_t =  typename _traits_t::iterator_category;
    using _no_pointer_t = typename std::remove_pointer<_pointer_t>::type;
    using _no_reference_t = typename std::remove_reference<_reference_t>::type;
    using _raw_value_t = typename std::remove_cv<_value_t>::type;
    using _raw_pointer_t = typename std::remove_cv<_no_pointer_t>::type;
    using _raw_reference_t = typename std::remove_cv<_no_reference_t>::type;
    using _cv_value_t = _no_reference_t;
    static_assert(std::is_same<_raw_pointer_t, _raw_value_t>::value, "");
    static_assert(std::is_same<_raw_reference_t, _raw_value_t>::value, "");
    
    // Types
    public:
    using difference_type = _difference_t;
    using value_type = _cv_value_t;
    using pointer = _pointer_t;
    using reference = _reference_t;
    using iterator_category = _category_t;
};
/* ************************************************************************** */



/* ****************** IMPLEMENTATION DETAILS: INSTRUCTIONS ****************** */
// Population count
template <class T, class = decltype(__builtin_popcountll(T()))>
constexpr T _popcnt(T src);
template <class T, class... X>
constexpr T _popcnt(T src, X...);

// Leading zeros count
template <class T, class = decltype(__builtin_clzll(T()))>
constexpr T _lzcnt(T src);
template <class T, class... X>
constexpr T _lzcnt(T src, X...);

// Trailing zeros count
template <class T, class = decltype(__builtin_ctzll(T()))>
constexpr T _tzcnt(T src);
template <class T, class... X>
constexpr T _tzcnt(T src, X...);

// Bit field extraction
template <class T, class = decltype(__builtin_ia32_bextr_u64(T(), T(), T()))>
constexpr T _bextr(T src, T start, T len);
template <class T, class... X>
constexpr T _bextr(T src, T start, T len, X...);

// Parallel bits deposit
template <class T, class = decltype(_pdep_u64(T()))>
constexpr T _pdep(T src, T mask);
template <class T, class... X>
constexpr T _pdep(T src, T mask, X...);

// Parallel bits extract
template <class T, class = decltype(_pext_u64(T()))>
constexpr T _pext(T src, T mask);
template <class T, class... X>
constexpr T _pext(T src, T mask, X...);

// Byte swap
template <class T, class = decltype(__builtin_bswap64(T()))>
constexpr T _byteswap(T src);
template <class T, class... X>
constexpr T _byteswap(T src, X...);

// Bit swap
template <class T>
constexpr T _bitswap(T src);
/* ************************************************************************** */



// --------- IMPLEMENTATION DETAILS: INSTRUCTIONS: POPULATION COUNT --------- //
// Counts the number of bits set to 1 with compiler intrinsics
template <class T, class>
constexpr T _popcnt(T src)
{
    static_assert(binary_digits<T>::value, "");
    constexpr T digits = binary_digits<T>::value;
    if (digits <= std::numeric_limits<unsigned int>::digits) {
        src = __builtin_popcount(src); 
    } else if (digits <= std::numeric_limits<unsigned long int>::digits) {
        src = __builtin_popcountl(src); 
    } else if (digits <= std::numeric_limits<unsigned long long int>::digits) {
        src = __builtin_popcountll(src); 
    } else {
        src = _popcnt(src, std::ignore);
    }
    return src;
}

// Counts the number of bits set to 1 without compiler intrinsics
template <class T, class... X>
constexpr T _popcnt(T src, X...)
{
    static_assert(binary_digits<T>::value, "");
    T dest = 0;
    for (dest = 0; src; src >>= 1) {
        dest += src & 1;
    }
    return dest;
}
// -------------------------------------------------------------------------- //



// ------- IMPLEMENTATION DETAILS: INSTRUCTIONS: LEADING ZEROS COUNT -------- //
// Counts the number of leading zeros with compiler intrinsics
template <class T, class>
constexpr T _lzcnt(T src)
{
    static_assert(binary_digits<T>::value, "");
    constexpr T digits = binary_digits<T>::value;
    T dest = 0;
    if (digits < std::numeric_limits<unsigned int>::digits) {
        dest = src ? __builtin_clz(src)
                     - (std::numeric_limits<unsigned int>::digits 
                     - digits)
                   : digits;
    } else if (digits == std::numeric_limits<unsigned int>::digits) {
        dest = src ? __builtin_clz(src) : digits;
    } else if (digits < std::numeric_limits<unsigned long int>::digits) {
        dest = src ? __builtin_clzl(src)
                     - (std::numeric_limits<unsigned long int>::digits 
                     - digits)
                   : digits;
    } else if (digits == std::numeric_limits<unsigned long int>::digits) {
        dest = src ? __builtin_clzl(src) : digits;
    } else if (digits < std::numeric_limits<unsigned long long int>::digits) {
        dest = src ? __builtin_clzll(src)
                     - (std::numeric_limits<unsigned long long int>::digits 
                     - digits)
                   : digits;
    } else if (digits == std::numeric_limits<unsigned long long int>::digits) {
        dest = src ? __builtin_clzll(src) : digits;
    } else {
        dest = _lzcnt(src, std::ignore);
    }
    return dest;
}

// Counts the number of leading zeros without compiler intrinsics
template <class T, class... X>
constexpr T _lzcnt(T src, X...)
{
    static_assert(binary_digits<T>::value, "");
    constexpr T digits = binary_digits<T>::value;
    T dest = src != 0;
    while (src >>= 1) {
        ++dest;
    }
    return digits - dest;
}
// -------------------------------------------------------------------------- //



// ------- IMPLEMENTATION DETAILS: INSTRUCTIONS: TRAILING ZEROS COUNT ------- //
// Counts the number of trailing zeros with compiler intrinsics
template <class T, class>
constexpr T _tzcnt(T src)
{
    static_assert(binary_digits<T>::value, "");
    constexpr T digits = binary_digits<T>::value;
    T dest = 0;
    if (digits <= std::numeric_limits<unsigned int>::digits) {
        dest = src ? __builtin_ctz(src) : digits; 
    } else if (digits <= std::numeric_limits<unsigned long int>::digits) {
        dest = src ? __builtin_ctzl(src) : digits; 
    } else if (digits <= std::numeric_limits<unsigned long long int>::digits) {
        dest = src ? __builtin_ctzll(src) : digits; 
    } else {
        dest = _tzcnt(src, std::ignore);
    }
    return dest;
}

// Counts the number of trailing zeros without compiler intrinsics
template <class T, class... X>
constexpr T _tzcnt(T src, X...)
{
    static_assert(binary_digits<T>::value, "");
    constexpr T digits = binary_digits<T>::value;
    T dest = digits;
    if (src) {
        src = (src ^ (src - 1)) >> 1;
        for (dest = 0; src; dest++) {
            src >>= 1;
        }
    }
    return dest;
}
// -------------------------------------------------------------------------- //



// ------- IMPLEMENTATION DETAILS: INSTRUCTIONS: BIT FIELD EXTRACTION ------- //
// Extacts a field of contiguous bits with compiler intrinsics
template <class T, class>
constexpr T _bextr(T src, T start, T len)
{
    static_assert(binary_digits<T>::value, "");
    constexpr T digits = binary_digits<T>::value;
    T dest = 0;
    if (digits <= std::numeric_limits<unsigned int>::digits) {
        dest = start < digits ? __builtin_ia32_bextr_u32(src, start, len) : 0; 
    } else if (digits <= std::numeric_limits<unsigned long long int>::digits) {
        dest = start < digits ? __builtin_ia32_bextr_u64(src, start, len) : 0;
    } else {
        dest = _bextr(src, std::ignore);
    }
    return dest;
}

// Extacts a field of contiguous bits without compiler intrinsics
template <class T, class... X>
constexpr T _bextr(T src, T start, T len, X...)
{
    static_assert(binary_digits<T>::value, "");
    constexpr T digits = binary_digits<T>::value;
    constexpr T zero = 0;
    constexpr T one = 1;
    return start < digits ? (src >> start) & ((one << len) - one) : zero;
}
// -------------------------------------------------------------------------- //



// ------- IMPLEMENTATION DETAILS: INSTRUCTIONS: PARALLEL BIT DEPOSIT ------- //
// Deposits bits according to a mask with compiler instrinsics
template <class T, class>
constexpr T _pdep(T src, T mask)
{
    static_assert(binary_digits<T>::value, "");
    constexpr T digits = binary_digits<T>::value;
    T dest = 0;
    if (digits <= std::numeric_limits<unsigned int>::digits) {
        dest = _pdep_u32(src, mask); 
    } else if (digits <= std::numeric_limits<unsigned long long int>::digits) {
        dest = _pdep_u64(src, mask);
    } else {
        dest = _pdep(src, mask, std::ignore);
    }
    return dest;
}

// Deposits bits according to a mask without compiler instrinsics
template <class T, class... X>
constexpr T _pdep(T src, T mask, X...)
{
    static_assert(binary_digits<T>::value, "");
    constexpr T digits = binary_digits<T>::value;
    T dest = 0;
    T count = 0;
    while (mask) {
        dest >>= 1;
        if (mask & 1) {
            dest |= src << (digits - 1);
            src >>= 1;
        }
        mask >>= 1;
        ++count;
    }
    dest >>= (digits - count) * (count > 0);
    return dest;
}
// -------------------------------------------------------------------------- //



// ------- IMPLEMENTATION DETAILS: INSTRUCTIONS: PARALLEL BIT EXTRACT ------- //
// Extracts bits according to a mask with compiler instrinsics
template <class T, class>
constexpr T _pext(T src, T mask)
{
    static_assert(binary_digits<T>::value, "");
    constexpr T digits = binary_digits<T>::value;
    T dest = 0;
    if (digits <= std::numeric_limits<unsigned int>::digits) {
        dest = _pext_u32(src, mask); 
    } else if (digits <= std::numeric_limits<unsigned long long int>::digits) {
        dest = _pext_u64(src, mask);
    } else {
        dest = _pext(src, mask, std::ignore);
    }
    return dest;
}

// Extracts bits according to a mask without compiler instrinsics
template <class T, class... X>
constexpr T _pext(T src, T mask, X...)
{
    static_assert(binary_digits<T>::value, "");
    constexpr T digits = binary_digits<T>::value;
    T dest = 0;
    T count = 0;
    while (mask) {
        if (mask & 1) {
            dest >>= 1;
            dest |= src << (digits - 1);
            ++count;
        }
        src >>= 1;
        mask >>= 1;
    }
    dest >>= (digits - count) * (count > 0);
    return dest;
}
// -------------------------------------------------------------------------- //



// ------------ IMPLEMENTATION DETAILS: INSTRUCTIONS: BYTE SWAP ------------- //
// Reverses the order of the underlying bytes with compiler intrinsics
template <class T, class>
constexpr T _byteswap(T src)
{
    static_assert(binary_digits<T>::value, "");
    constexpr T digits = std::numeric_limits<T>::digits;
    if (digits == std::numeric_limits<std::uint16_t>::digits) {
        src = __builtin_bswap16(src);
    } else if (digits == std::numeric_limits<std::uint32_t>::digits) {
        src = __builtin_bswap32(src);
    } else if (digits == std::numeric_limits<std::uint64_t>::digits)  {
        src = __builtin_bswap64(src);
    } else if (digits > std::numeric_limits<unsigned char>::digits) {
        src = _byteswap(src, std::ignore);
    }
    return src;
}

// Reverses the order of the underlying bytes without compiler intrinsics
template <class T, class... X>
constexpr T _byteswap(T src, X...)
{
    static_assert(binary_digits<T>::value, "");
    constexpr T half = sizeof(T) / 2;
    constexpr T end = sizeof(T) - 1;
    unsigned char* bytes = reinterpret_cast<unsigned char*>(&src);
    unsigned char byte = 0;
    for (T i = 0; i < half; ++i) {
        byte = bytes[i];
        bytes[i] = bytes[end - i];
        bytes[end - i] = byte;
    }
    return src;
}
// -------------------------------------------------------------------------- //



// ------------- IMPLEMENTATION DETAILS: INSTRUCTIONS: BIT SWAP ------------- //
// Reverses the order of the bits with or without of compiler intrinsics
template <class T>
constexpr T _bitswap(T src)
{
    static_assert(binary_digits<T>::value, "");
    constexpr T digits = binary_digits<T>::value;
    constexpr unsigned long long int first = 0x80200802ULL;
    constexpr unsigned long long int second = 0x0884422110ULL;
    constexpr unsigned long long int third = 0x0101010101ULL;
    constexpr unsigned long long int fourth = 32;
    constexpr bool is_octet = std::numeric_limits<unsigned char>::digits == 8;
    unsigned char* bytes = nullptr;
    T dest = src;
    T i = digits - 1;
    if (false && is_octet) {
        bytes = reinterpret_cast<unsigned char*>(&dest);
        dest = _byteswap(dest);
        for (i = 0; i < sizeof(T); ++i) {
            bytes[i] = ((bytes[i] * first) & second) * third >> fourth;
        }
    } else {
        for (src >>= 1; src; src >>= 1) {   
            dest <<= 1;
            dest |= src & 1;
            i--;
        }
        dest <<= i;
    }
    return dest;
}
// -------------------------------------------------------------------------- //



// ========================================================================== //
} // namespace bit
#endif // _BIT_DETAILS_HPP_INCLUDED
// ========================================================================== //
