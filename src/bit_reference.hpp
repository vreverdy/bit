// ============================= BIT REFERENCE ============================== //
// Project:         C++ bit library
// Name:            bit_reference.hpp
// Description:     A template class referencing a bit in an unsigned integer
// Creator:         Vincent Reverdy
// Contributor(s):  Vincent Reverdy [2015-2016]
// License:         BSD 3-Clause License
// ========================================================================== //
#ifndef _BIT_REFERENCE_HPP_INCLUDED
#define _BIT_REFERENCE_HPP_INCLUDED
// ========================================================================== //



// ================================ PREAMBLE ================================ //
// C++ standard library
#include <limits>
#include <cassert>
#include <iostream>
#include <type_traits>
// Project sources
#include "bit_value.hpp"
// Third-party libraries
// Miscellaneous
namespace bit {
// ========================================================================== //



/* ***************************** BIT REFERENCE ****************************** */
// Bit reference class definition
template <class UIntType>
class bit_reference
{
    // Assertions
    static_assert(std::is_integral<UIntType>::value, "");
    static_assert(std::is_unsigned<UIntType>::value, "");
    static_assert(!std::is_same<UIntType, bool>::value, "");

    // Friendship
    template <class> friend class bit_reference;
    friend class bit_pointer<UIntType>;
    
    // Types
    public:
    using underlying_type = UIntType;
    using size_type = std::size_t;
     
    // Lifecycle
    public:
    template <class T> 
    constexpr bit_reference(const bit_reference<T>& other) noexcept;
    constexpr bit_reference(underlying_type& ref, size_type pos);
    
    // Assignment
    public:
    bit_reference& operator=(const bit_reference& other) noexcept;
    bit_reference& operator=(bit_value val) noexcept;
    
    // Conversion
    public:
    constexpr operator bit_value() const noexcept;

    // Access
    public:
    constexpr bit_pointer<UIntType> operator&() const noexcept;

    // Operations
    public:
    template <class T> 
    void swap(bit_reference<T> other);
    void swap(bit_value& other);
    void set(bool b) noexcept;
    void set() noexcept;
    void reset() noexcept;
    void flip() noexcept;
    
    // Underlying details
    public:
    constexpr underlying_type* address() const noexcept;
    constexpr size_type position() const noexcept;
    
    // Implementation details: function members
    private:
    bit_reference() noexcept = default;
    explicit constexpr bit_reference(std::nullptr_t, size_type = 0) noexcept;
    explicit constexpr bit_reference(underlying_type* ptr, size_type pos);
    constexpr bit_reference* _addressof() const noexcept;
    template <class T> static constexpr size_type _tzcnt(T val) noexcept;
    
    // Implementation details: data members
    private:
    underlying_type* _ptr;
    typename std::remove_cv<underlying_type>::type _mask;
};

// Swap and exchange
template <class T, class U>
void swap(
    bit_reference<T> lhs,
    bit_reference<U> rhs
) noexcept;
template <class T>
void swap(
    bit_reference<T> lhs,
    bit_value& rhs
) noexcept;
template <class U>
void swap(
    bit_value& lhs,
    bit_reference<U> rhs
) noexcept;
template <class T, class U = bit_value>
bit_value exchange(
    bit_reference<T> x,
    U&& val
);

// Stream functions
template <class CharT, class Traits, class T>
std::basic_ostream<CharT, Traits>& operator<<(
    std::basic_ostream<CharT, Traits>& os,
    const bit_reference<T>& x
);
template <class CharT, class Traits, class T>
std::basic_istream<CharT, Traits>& operator>>(
    std::basic_istream<CharT, Traits>& is,
    bit_reference<T>& x
);

// Make function
template <class T>
constexpr bit_reference<T> make_bit_reference(
  T& ref,
  typename bit_reference<T>::size_type pos
);
/* ************************************************************************** */



// ------------------------ BIT REFERENCE: LIFECYCLE ------------------------ //
// Constructs the reference from another bit reference differently qualified
template <class UIntType>
template <class T>
constexpr bit_reference<UIntType>::bit_reference(
    const bit_reference<T>& other
) noexcept
: _ptr(other._ptr)
, _mask(other._mask)
{
}

// Constructs the bit reference from an unsigned integer and a position
template <class UIntType>
constexpr bit_reference<UIntType>::bit_reference(
    underlying_type& ref, 
    size_type pos
)
: _ptr((assert(pos < std::numeric_limits<underlying_type>::digits), &ref))
, _mask(static_cast<underlying_type>(1) << pos)
{
}
// -------------------------------------------------------------------------- //



// ----------------------- BIT REFERENCE: ASSIGNMENT ------------------------ //
// Assigns the value of a referenced bit to the referenced bit
template <class UIntType>
bit_reference<UIntType>& bit_reference<UIntType>::operator=(
    const bit_reference& other
) noexcept
{
    other ? set() : reset();
    return *this;
}

// Assigns the value of a bit value to the referenced bit
template <class UIntType>
bit_reference<UIntType>& bit_reference<UIntType>::operator=(
    bit_value val
) noexcept
{
    val ? set() : reset();
    return *this;
}
// -------------------------------------------------------------------------- //



// ----------------------- BIT REFERENCE: CONVERSION ------------------------ //
// Implicitly converts the referenced bit to a boolean
template <class UIntType>
constexpr bit_reference<UIntType>::operator bool(
) const noexcept
{
    return *_ptr & _mask;
}

// Implicitly converts the referenced bit to a bit value
template <class UIntType>
constexpr bit_reference<UIntType>::operator bit_value(
) const noexcept
{
    return *_ptr & _mask;
}
// -------------------------------------------------------------------------- //



// ------------------------- BIT REFERENCE: ACCESS -------------------------- //
// Gets a bit pointer from the bit reference
template <class UIntType>
constexpr bit_pointer<UIntType> bit_reference<UIntType>::operator&(
) const noexcept
{
    return bit_pointer<UIntType>(_ptr, position());
}
// -------------------------------------------------------------------------- //



// ----------------------- BIT REFERENCE: OPERATIONS ------------------------ //
// Swaps the value of the referenced bit with another bit reference
template <class UIntType>
template <class T>
void bit_reference<UIntType>::swap(
    bit_reference<T> other
)
{
    if (other != *this) {
        flip();
        other.flip();
    }
}

// Swaps the value of the referenced bit with a bit value
template <class UIntType>
void bit_reference<UIntType>::swap(
    bit_value& other
)
{
    if (other != *this) {
        flip();
        other.flip();
    }
}

// Sets the value of the reference bit to the provided boolean value
template <class UIntType>
void bit_reference<UIntType>::set(
    bool b
) noexcept
{
    b ? set() : reset();
}

// Sets the value of the referenced bit
template <class UIntType>
void bit_reference<UIntType>::set(
) noexcept
{
    *_ptr |= _mask;
}

// Resets the value of the referenced bit
template <class UIntType>
void bit_reference<UIntType>::reset(
) noexcept
{
    *_ptr &= ~_mask;
}

// Flips the value of the referenced bit
template <class UIntType>
void bit_reference<UIntType>::flip(
) noexcept
{
    *_ptr ^= _mask;
}
// -------------------------------------------------------------------------- //



// ------------------- BIT REFERENCE: UNDERLYING DETAILS -------------------- //
// Returns a pointer to the underlying object
template <class UIntType>
constexpr typename bit_reference<UIntType>::underlying_type*
bit_reference<UIntType>::address(
) const noexcept
{
    return _ptr;
}

// Returns the position of the bit within the underlying object
template <class UIntType>
constexpr typename bit_reference<UIntType>::size_type
bit_reference<UIntType>::position(
) const noexcept
{
    constexpr size_type digits = std::numeric_limits<underlying_type>::digits;
    size_type pos = 0;
    if (digits <= std::numeric_limits<unsigned int>::digits) {
        pos = __builtin_ctz(_mask); 
    } else if (digits <= std::numeric_limits<unsigned long int>::digits) {
        pos = __builtin_ctzl(_mask); 
    } else if (digits <= std::numeric_limits<unsigned long long int>::digits) {
        pos = __builtin_ctzll(_mask); 
    } else {
        pos = _tzcnt(_mask);
    }
    return pos;
}
// -------------------------------------------------------------------------- //



// --------------------------- SWAP AND EXCHANGE ---------------------------- //
// Swaps two bit references
template <class T, class U>
void swap(
    bit_reference<T> lhs,
    bit_reference<U> rhs
) noexcept
{
    if (lhs != rhs) {
        lhs.flip();
        rhs.flip();
    }
}

// Swaps a bit reference and a bit value
template <class T>
void swap(
    bit_reference<T> lhs,
    bit_value& rhs
) noexcept
{
    if (lhs != rhs) {
        lhs.flip();
        rhs.flip();
    }
}

// Swaps a bit value and a bit reference
template <class U>
void swap(
    bit_value& lhs,
    bit_reference<U> rhs
) noexcept
{
    if (lhs != rhs) {
        lhs.flip();
        rhs.flip();
    }
}

// Exchanges a bit reference and a value
template <class T, class U>
bit_value exchange(
    bit_reference<T> x,
    U&& val
)
{
    bit_value old = x;
    x = val;
    return old;
}
// -------------------------------------------------------------------------- //



// ---------------------------- STREAM FUNCTIONS ---------------------------- //
// Inserts a bit value in an output stream
template <class CharT, class Traits, class T>
std::basic_ostream<CharT, Traits>& operator<<(
    std::basic_ostream<CharT, Traits>& os,
    const bit_reference<T>& x
)
{
    return os << (x ? '1' : '0');
}

// Extracts a bit value from an input stream
template <class CharT, class Traits, class T>
std::basic_istream<CharT, Traits>& operator>>(
    std::basic_istream<CharT, Traits>& is,
    bit_reference<T>& x
)
{
    unsigned int tmp;
    is >> tmp;
    x = tmp;
    return is;
}
// -------------------------------------------------------------------------- //



// ----------------------------- MAKE FUNCTION ------------------------------ //
// Constructs a bit reference from an unsigned integer reference and a position
template <class T>
constexpr bit_reference<T> make_bit_reference(
  T& ref,
  typename bit_reference<T>::size_type pos
)
{
    return bit_reference<T>(ref, pos);
}
// -------------------------------------------------------------------------- //



// -------- BIT REFERENCE: IMPLEMENTATION DETAILS: FUNCTION MEMBERS --------- //
// Privately constructs the bit reference from a null pointer
template <class UIntType>
constexpr bit_reference<UIntType>::bit_reference(
    std::nullptr_t, 
    size_type
) noexcept
: _ptr(nullptr)
, _mask()
{
}

// Privately constructs the bit reference from a pointer and a position
template <class UIntType>
constexpr bit_reference<UIntType>::bit_reference(
    underlying_type* ptr, 
    size_type pos
)
: _ptr((assert(pos < std::numeric_limits<underlying_type>::digits), ptr))
, _mask(static_cast<underlying_type>(1) << pos)
{
}

// Returns a pointer to the bit reference
template <class UIntType>
constexpr bit_reference<UIntType>* bit_reference<UIntType>::_addressof(
) const noexcept
{
    return const_cast<bit_reference<UIntType>*>(this);
}

// Returns the position of the first set bit of the mask
template <class UIntType>
template <class T> 
constexpr typename bit_reference<UIntType>::size_type 
bit_reference<UIntType>::_tzcnt(
    T val
) noexcept
{
    size_type pos = 0;
    while (val >>= 1) {
        ++pos;
    }
    return pos;
}
// -------------------------------------------------------------------------- //



// ========================================================================== //
} // namespace bit
#endif // _BIT_REFERENCE_HPP_INCLUDED
// ========================================================================== //
