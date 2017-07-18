// ============================== BIT DETAILS =============================== //
// Project:         The C++ Bit Library
// Name:            bit_details.hpp
// Description:     Provides common implementation details and helper classes
// Creator:         Vincent Reverdy
// Contributor(s):  Vincent Reverdy [2015-2017]
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
#include <stdexcept>
#include <type_traits>
// Project sources
// Third-party libraries
// Miscellaneous
namespace bit {
class bit_value;
template <class WordType> class bit_reference;
template <class WordType> class bit_pointer;
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
template <class T>
constexpr std::size_t binary_digits_v = binary_digits<T>::value;
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



/* *********** IMPLEMENTATION DETAILS: NARROWEST AND WIDEST TYPES *********** */
// Narrowest type structure declaration
template <class... T>
struct _narrowest_type;

// Narrowest type structure specialization: selects the only passed type
template <class T>
struct _narrowest_type<T>
: std::common_type<T>
{
    static_assert(binary_digits<T>::value, "");
};

// Narrowest type structure specialization: selects the type with less bits
template <class T, class U>
struct _narrowest_type<T, U>
: _narrowest_type<
    typename std::conditional<
        (binary_digits<T>::value < binary_digits<U>::value),
        T,
        typename std::conditional<
            (binary_digits<T>::value > binary_digits<U>::value),
            U,
            typename std::common_type<T, U>::type
        >::type
    >::type
>
{
};

// Narrowest type structure specialization: recursively selects the right type
template <class T, class... U>
struct _narrowest_type<T, U...>
: _narrowest_type<T, typename _narrowest_type<U...>::type>
{
};

// Narrowest type alias
template <class... T>
using _narrowest_type_t = typename _narrowest_type<T...>::type;

// Widest type structure declaration
template <class... X>
struct _widest_type;

// Widest type structure specialization: selects the only passed type
template <class T>
struct _widest_type<T>
: std::common_type<T>
{
    static_assert(binary_digits<T>::value, "");
};

// Widest type structure specialization: selects the type with more bits
template <class T, class U>
struct _widest_type<T, U>
: _widest_type<
    typename std::conditional<
        (binary_digits<T>::value > binary_digits<U>::value),
        T,
        typename std::conditional<
            (binary_digits<T>::value < binary_digits<U>::value),
            U,
            typename std::common_type<T, U>::type
        >::type
    >::type
>
{
};

// Widest type structure specialization: recursively selects the right type
template <class T, class... X>
struct _widest_type<T, X...>
: _widest_type<T, typename _widest_type<X...>::type>
{
};

// Widest type alias
template <class... T>
using _widest_type_t = typename _widest_type<T...>::type;
/* ************************************************************************** */



/* ******************* IMPLEMENTATION DETAILS: UTILITIES ******************** */
// Assertions
template <class Iterator>
constexpr bool _assert_range_viability(Iterator first, Iterator last);
/* ************************************************************************** */



/* ****************** IMPLEMENTATION DETAILS: INSTRUCTIONS ****************** */
// Population count
template <class T, class = decltype(__builtin_popcountll(T()))>
constexpr T _popcnt(T src) noexcept;
template <class T, class... X>
constexpr T _popcnt(T src, X...) noexcept;

// Leading zeros count
template <class T, class = decltype(__builtin_clzll(T()))>
constexpr T _lzcnt(T src) noexcept;
template <class T, class... X>
constexpr T _lzcnt(T src, X...) noexcept;

// Trailing zeros count
template <class T, class = decltype(__builtin_ctzll(T()))>
constexpr T _tzcnt(T src) noexcept;
template <class T, class... X>
constexpr T _tzcnt(T src, X...) noexcept;

// Bit field extraction
template <class T, class = decltype(__builtin_ia32_bextr_u64(T(), T(), T()))>
constexpr T _bextr(T src, T start, T len) noexcept;
template <class T, class... X>
constexpr T _bextr(T src, T start, T len, X...) noexcept;

// Parallel bits deposit
template <class T, class = decltype(_pdep_u64(T()))>
constexpr T _pdep(T src, T msk) noexcept;
template <class T, class... X>
constexpr T _pdep(T src, T msk, X...) noexcept;

// Parallel bits extract
template <class T, class = decltype(_pext_u64(T()))>
constexpr T _pext(T src, T msk) noexcept;
template <class T, class... X>
constexpr T _pext(T src, T msk, X...) noexcept;

// Byte swap
template <class T, class T128 = decltype(__uint128_t(__builtin_bswap64(T())))>
constexpr T _byteswap(T src) noexcept;
template <class T, class... X>
constexpr T _byteswap(T src, X...) noexcept;

// Bit swap
template <class T>
constexpr T _bitswap(T src) noexcept;
template <class T, std::size_t N>
constexpr T _bitswap(T src) noexcept;
template <class T, std::size_t N>
constexpr T _bitswap() noexcept;

// Bit blend
template <class T>
constexpr T _bitblend(T src0, T src1, T msk) noexcept;
template <class T>
constexpr T _bitblend(T src0, T src1, T start, T len) noexcept;

// Bit compare
template <class T>
constexpr T _bitcmp(T src0, T src1, T start0, T start1, T len) noexcept;

// Double precision shift left
template <class T>
constexpr T _shld(T dst, T src, T cnt) noexcept;

// Double precision shift right
template <class T>
constexpr T _shrd(T dst, T src, T cnt) noexcept;

// Add carry
template <class... T>
using _supports_addcarry = decltype(__builtin_ia32_addcarryx_u64(T()...));
template <class C, class T, class = _supports_addcarry<C, T, T, std::nullptr_t>>
constexpr C _addcarry(C carry, T src0, T src1, T* dst) noexcept;
template <class C, class T, class... X>
constexpr C _addcarry(C carry, T src0, T src1, T* dst, X...) noexcept;
/* ************************************************************************** */



// ------------- IMPLEMENTATION DETAILS: UTILITIES: ASSERTIONS -------------- //
// If the range allows multipass iteration, checks if last - first >= 0
template <class Iterator>
constexpr bool _assert_range_viability(Iterator first, Iterator last)
{
    using traits_t = std::iterator_traits<Iterator>;
    using category_t =  typename traits_t::iterator_category;
    using multi_t = std::forward_iterator_tag;
    constexpr bool is_multipass = std::is_base_of<multi_t, category_t>::value;
    const bool is_viable = !is_multipass || std::distance(first, last) >= 0;
    assert(is_viable);
    return is_viable;
}
// -------------------------------------------------------------------------- //



// --------- IMPLEMENTATION DETAILS: INSTRUCTIONS: POPULATION COUNT --------- //
// Counts the number of bits set to 1 with compiler intrinsics
template <class T, class>
constexpr T _popcnt(T src) noexcept
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
constexpr T _popcnt(T src, X...) noexcept
{
    static_assert(binary_digits<T>::value, "");
    T dst = T();
    for (dst = T(); src; src >>= 1) {
        dst += src & 1;
    }
    return dst;
}
// -------------------------------------------------------------------------- //



// ------- IMPLEMENTATION DETAILS: INSTRUCTIONS: LEADING ZEROS COUNT -------- //
// Counts the number of leading zeros with compiler intrinsics
template <class T, class>
constexpr T _lzcnt(T src) noexcept
{
    static_assert(binary_digits<T>::value, "");
    constexpr T digits = binary_digits<T>::value;
    T dst = T();
    if (digits < std::numeric_limits<unsigned int>::digits) {
        dst = src ? __builtin_clz(src)
                     - (std::numeric_limits<unsigned int>::digits 
                     - digits)
                   : digits;
    } else if (digits == std::numeric_limits<unsigned int>::digits) {
        dst = src ? __builtin_clz(src) : digits;
    } else if (digits < std::numeric_limits<unsigned long int>::digits) {
        dst = src ? __builtin_clzl(src)
                     - (std::numeric_limits<unsigned long int>::digits 
                     - digits)
                   : digits;
    } else if (digits == std::numeric_limits<unsigned long int>::digits) {
        dst = src ? __builtin_clzl(src) : digits;
    } else if (digits < std::numeric_limits<unsigned long long int>::digits) {
        dst = src ? __builtin_clzll(src)
                     - (std::numeric_limits<unsigned long long int>::digits 
                     - digits)
                   : digits;
    } else if (digits == std::numeric_limits<unsigned long long int>::digits) {
        dst = src ? __builtin_clzll(src) : digits;
    } else {
        dst = _lzcnt(src, std::ignore);
    }
    return dst;
}

// Counts the number of leading zeros without compiler intrinsics
template <class T, class... X>
constexpr T _lzcnt(T src, X...) noexcept
{
    static_assert(binary_digits<T>::value, "");
    constexpr T digits = binary_digits<T>::value;
    T dst = src != T();
    while (src >>= 1) {
        ++dst;
    }
    return digits - dst;
}
// -------------------------------------------------------------------------- //



// ------- IMPLEMENTATION DETAILS: INSTRUCTIONS: TRAILING ZEROS COUNT ------- //
// Counts the number of trailing zeros with compiler intrinsics
template <class T, class>
constexpr T _tzcnt(T src) noexcept
{
    static_assert(binary_digits<T>::value, "");
    constexpr T digits = binary_digits<T>::value;
    T dst = T();
    if (digits <= std::numeric_limits<unsigned int>::digits) {
        dst = src ? __builtin_ctz(src) : digits; 
    } else if (digits <= std::numeric_limits<unsigned long int>::digits) {
        dst = src ? __builtin_ctzl(src) : digits; 
    } else if (digits <= std::numeric_limits<unsigned long long int>::digits) {
        dst = src ? __builtin_ctzll(src) : digits; 
    } else {
        dst = _tzcnt(src, std::ignore);
    }
    return dst;
}

// Counts the number of trailing zeros without compiler intrinsics
template <class T, class... X>
constexpr T _tzcnt(T src, X...) noexcept
{
    static_assert(binary_digits<T>::value, "");
    constexpr T digits = binary_digits<T>::value;
    T dst = digits;
    if (src) {
        src = (src ^ (src - 1)) >> 1;
        for (dst = T(); src; dst++) {
            src >>= 1;
        }
    }
    return dst;
}
// -------------------------------------------------------------------------- //



// ------- IMPLEMENTATION DETAILS: INSTRUCTIONS: BIT FIELD EXTRACTION ------- //
// Extacts to lsbs a field of contiguous bits with compiler intrinsics
template <class T, class>
constexpr T _bextr(T src, T start, T len) noexcept
{
    static_assert(binary_digits<T>::value, "");
    constexpr T digits = binary_digits<T>::value;
    T dst = T();
    if (digits <= std::numeric_limits<unsigned int>::digits) {
        dst = __builtin_ia32_bextr_u32(src, start, len); 
    } else if (digits <= std::numeric_limits<unsigned long long int>::digits) {
        dst = __builtin_ia32_bextr_u64(src, start, len);
    } else {
        dst = _bextr(src, start, len, std::ignore);
    }
    return dst;
}

// Extacts to lsbs a field of contiguous bits without compiler intrinsics
template <class T, class... X>
constexpr T _bextr(T src, T start, T len, X...) noexcept
{
    static_assert(binary_digits<T>::value, "");
    constexpr T digits = binary_digits<T>::value;
    constexpr T one = 1;
    const T msk = (one << len) * (len < digits) - one;
    return (src >> start) & msk * (start < digits);
}
// -------------------------------------------------------------------------- //



// ------- IMPLEMENTATION DETAILS: INSTRUCTIONS: PARALLEL BIT DEPOSIT ------- //
// Deposits bits according to a mask with compiler instrinsics
template <class T, class>
constexpr T _pdep(T src, T msk) noexcept
{
    static_assert(binary_digits<T>::value, "");
    constexpr T digits = binary_digits<T>::value;
    T dst = T();
    if (digits <= std::numeric_limits<unsigned int>::digits) {
        dst = _pdep_u32(src, msk); 
    } else if (digits <= std::numeric_limits<unsigned long long int>::digits) {
        dst = _pdep_u64(src, msk);
    } else {
        dst = _pdep(src, msk, std::ignore);
    }
    return dst;
}

// Deposits bits according to a mask without compiler instrinsics
template <class T, class... X>
constexpr T _pdep(T src, T msk, X...) noexcept
{
    static_assert(binary_digits<T>::value, "");
    constexpr T digits = binary_digits<T>::value;
    T dst = T();
    T cnt = T();
    while (msk) {
        dst >>= 1;
        if (msk & 1) {
            dst |= src << (digits - 1);
            src >>= 1;
        }
        msk >>= 1;
        ++cnt;
    }
    dst >>= (digits - cnt) * (cnt > 0);
    return dst;
}
// -------------------------------------------------------------------------- //



// ------- IMPLEMENTATION DETAILS: INSTRUCTIONS: PARALLEL BIT EXTRACT ------- //
// Extracts bits according to a mask with compiler instrinsics
template <class T, class>
constexpr T _pext(T src, T msk) noexcept
{
    static_assert(binary_digits<T>::value, "");
    constexpr T digits = binary_digits<T>::value;
    T dst = T();
    if (digits <= std::numeric_limits<unsigned int>::digits) {
        dst = _pext_u32(src, msk); 
    } else if (digits <= std::numeric_limits<unsigned long long int>::digits) {
        dst = _pext_u64(src, msk);
    } else {
        dst = _pext(src, msk, std::ignore);
    }
    return dst;
}

// Extracts bits according to a mask without compiler instrinsics
template <class T, class... X>
constexpr T _pext(T src, T msk, X...) noexcept
{
    static_assert(binary_digits<T>::value, "");
    constexpr T digits = binary_digits<T>::value;
    T dst = T();
    T cnt = T();
    while (msk) {
        if (msk & 1) {
            dst >>= 1;
            dst |= src << (digits - 1);
            ++cnt;
        }
        src >>= 1;
        msk >>= 1;
    }
    dst >>= (digits - cnt) * (cnt > 0);
    return dst;
}
// -------------------------------------------------------------------------- //



// ------------ IMPLEMENTATION DETAILS: INSTRUCTIONS: BYTE SWAP ------------- //
// Reverses the order of the underlying bytes with compiler intrinsics
template <class T, class T128>
constexpr T _byteswap(T src) noexcept
{
    static_assert(binary_digits<T>::value, "");
    using byte_t = unsigned char;
    constexpr T digits = sizeof(T) * std::numeric_limits<byte_t>::digits;
    std::uint64_t tmp64 = 0;
    std::uint64_t* ptr64 = nullptr;
    if (std::is_same<T, T128>::value) {
        ptr64 = reinterpret_cast<std::uint64_t*>(&src);
        tmp64 = __builtin_bswap64(*ptr64);
        *ptr64 = __builtin_bswap64(*(ptr64 + 1));
        *(ptr64 + 1) = tmp64;
    } else if (digits == std::numeric_limits<std::uint16_t>::digits) {
        src = __builtin_bswap16(src);
    } else if (digits == std::numeric_limits<std::uint32_t>::digits) {
        src = __builtin_bswap32(src);
    } else if (digits == std::numeric_limits<std::uint64_t>::digits)  {
        src = __builtin_bswap64(src);
    } else if (digits > std::numeric_limits<byte_t>::digits) {
        src = _byteswap(src, std::ignore);
    }
    return src;
}

// Reverses the order of the underlying bytes without compiler intrinsics
template <class T, class... X>
constexpr T _byteswap(T src, X...) noexcept
{
    static_assert(binary_digits<T>::value, "");
    using byte_t = unsigned char;
    constexpr T half = sizeof(T) / 2;
    constexpr T end = sizeof(T) - 1;
    unsigned char* bytes = reinterpret_cast<byte_t*>(&src);
    unsigned char byte = 0;
    for (T i = T(); i < half; ++i) {
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
constexpr T _bitswap(T src) noexcept
{
    static_assert(binary_digits<T>::value, "");
    using byte_t = unsigned char;
    constexpr auto ignore = nullptr;
    constexpr T digits = binary_digits<T>::value;
    constexpr unsigned long long int first = 0x80200802ULL;
    constexpr unsigned long long int second = 0x0884422110ULL;
    constexpr unsigned long long int third = 0x0101010101ULL;
    constexpr unsigned long long int fourth = 32;
    constexpr bool is_size1 = sizeof(T) == 1;
    constexpr bool is_byte = digits == std::numeric_limits<byte_t>::digits;
    constexpr bool is_octet = std::numeric_limits<byte_t>::digits == 8;
    constexpr bool is_pow2 = _popcnt(digits, ignore) == 1;
    T dst = src;
    T i = digits - 1;
    if (is_size1 && is_byte && is_octet) {
        dst = ((src * first) & second) * third >> fourth;
    } else if (is_pow2) {
        dst = _bitswap<T, digits>(src);
    } else {
        for (src >>= 1; src; src >>= 1) {   
            dst <<= 1;
            dst |= src & 1;
            i--;
        }
        dst <<= i;
    }
    return dst;
}

// Reverses the order of the bits: recursive metafunction
template <class T, std::size_t N>
constexpr T _bitswap(T src) noexcept
{
    static_assert(binary_digits<T>::value, "");
    constexpr T cnt = N >> 1;
    constexpr T msk = _bitswap<T, cnt>();
    src = ((src >> cnt) & msk) | ((src << cnt) & ~msk);
    return cnt > 1 ? _bitswap<T, cnt>(src) : src;
}

// Reverses the order of the bits: mask for the recursive metafunction
template <class T, std::size_t N>
constexpr T _bitswap() noexcept
{
    static_assert(binary_digits<T>::value, "");
    constexpr T digits = binary_digits<T>::value;
    T cnt = digits;
    T msk = ~T();
    while (cnt != N) {
        cnt >>= 1;
        msk ^= (msk << cnt);
    }
    return msk;
}
// -------------------------------------------------------------------------- //



// ------------ IMPLEMENTATION DETAILS: INSTRUCTIONS: BIT BLEND ------------- //
// Replaces len bits of src0 by the ones of src1 where the mask is true
template <class T>
constexpr T _bitblend(T src0, T src1, T msk) noexcept
{
    static_assert(binary_digits<T>::value, "");
    return src0 ^ ((src0 ^ src1) & msk);
}

// Replaces len bits of src0 by the ones of src1 starting at start
template <class T>
constexpr T _bitblend(T src0, T src1, T start, T len) noexcept
{
    static_assert(binary_digits<T>::value, "");
    constexpr T digits = binary_digits<T>::value;
    constexpr T one = 1;
    const T msk = ((one << len) * (len < digits) - one) << start;
    return src0 ^ ((src0 ^ src1) & msk * (start < digits));
}
// -------------------------------------------------------------------------- //



// ----------- IMPLEMENTATION DETAILS: INSTRUCTIONS: BIT COMPARE ------------ //
// Compares a subsequence of bits within src0 and src1 and returns 0 if equal
template <class T>
constexpr T _bitcmp(T src0, T src1, T start0, T start1, T len) noexcept
{
    static_assert(binary_digits<T>::value, "");
    return _bextr(src0, start0, len) == _bextr(src1, start1, len);
}
// -------------------------------------------------------------------------- //



// --- IMPLEMENTATION DETAILS: INSTRUCTIONS: DOUBLE PRECISION SHIFT LEFT ---- //
// Left shifts dst by cnt bits, filling the lsbs of dst by the msbs of src
template <class T>
constexpr T _shld(T dst, T src, T cnt) noexcept
{
    static_assert(binary_digits<T>::value, "");
    constexpr T digits = binary_digits<T>::value;
    if (cnt < digits) {
        dst = (dst << cnt) | (src >> (digits - cnt));
    } else {
        dst = (src << (cnt - digits)) * (cnt < digits + digits);
    }
    return dst;
}
// -------------------------------------------------------------------------- //



// --- IMPLEMENTATION DETAILS: INSTRUCTIONS: DOUBLE PRECISION SHIFT RIGHT --- //
// Right shifts dst by cnt bits, filling the msbs of dst by the lsbs of src
template <class T>
constexpr T _shrd(T dst, T src, T cnt) noexcept
{
    static_assert(binary_digits<T>::value, "");
    constexpr T digits = binary_digits<T>::value;
    if (cnt < digits) {
        dst = (dst >> cnt) | (src << (digits - cnt));
    } else {
        dst = (src >> (cnt - digits)) * (cnt < digits + digits);
    }
    return dst;
}
// -------------------------------------------------------------------------- //



// ------------ IMPLEMENTATION DETAILS: INSTRUCTIONS: ADD CARRY ------------- //
/*
// Adds src0 and src1 and returns the new carry bit with compiler intrinsics
template <class C, class T, class>
constexpr C _addcarry(C carry, T src0, T src1, T* dst) noexcept
{
    static_assert(binary_digits<T>::value, "");
    constexpr T digits = binary_digits<T>::value;
    unsigned char uhhdst = 0;
    unsigned short int uhdst = 0;
    unsigned int udst = 0;
    unsigned long int uldst = 0;
    unsigned long long int ulldst = 0;
    if (digits == std::numeric_limits<unsigned int>::digits) {
        carry = __builtin_ia32_addcarryx_u32(carry, src0, src1, *udst);
        *dst = *udst;
    } else if (digits == std::numeric_limits<unsigned long long int>::digits)
        carry = __builtin_ia32_addcarryx_u64(carry, src0, src1, *ulldst);
        *dst = *ulldst;
    } else if (digits < std::numeric_limits<unsigned char>::digits) {
        tmp = src0 + src1;
        dst = tmp;
        carry = tmp >> digits;
    } else {
        carry = _addcarry(carry, src0, src1, dst, std::ignore);
    }
    return carry;
}

// Adds src0 and src1 and returns the new carry bit without compiler intrinsics
template <class C, class T, class... X>
constexpr C _addcarry(C carry, T src0, T src1, T* dst, X...) noexcept
{
    *dst = src0 + src1 + static_cast<T>(static_cast<bool>(carry));
    return carry ? *dst <= src0 || *dst <= src1 : *dst < src0 || *dst < src1;
}
* */
// -------------------------------------------------------------------------- //



// ========================================================================== //
} // namespace bit
#endif // _BIT_DETAILS_HPP_INCLUDED
// ========================================================================== //
