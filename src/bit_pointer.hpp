// ============================== BIT POINTER =============================== //
// Project:         C++ bit library
// Name:            bit_pointer.hpp
// Description:     A template class pointing to a bit in an unsigned integer
// Creator:         Vincent Reverdy
// Contributor(s):  Vincent Reverdy [2015-2016]
// License:         BSD 3-Clause License
// ========================================================================== //
#ifndef _BIT_POINTER_HPP_INCLUDED
#define _BIT_POINTER_HPP_INCLUDED
// ========================================================================== //



// ================================ PREAMBLE ================================ //
// C++ standard library
#include <limits>
#include <type_traits>
// Project sources
#include "bit_value.hpp"
#include "bit_reference.hpp"
// Third-party libraries
// Miscellaneous
namespace bit {
// ========================================================================== //



/* ****************************** BIT POINTER ******************************* */
// Bit pointer class definition
template <class UIntType>
class bit_pointer
{
    // Assertions
    static_assert(std::is_integral<UIntType>::value, "");
    static_assert(std::is_unsigned<UIntType>::value, "");
    static_assert(!std::is_same<UIntType, bool>::value, "");

    // Friendship
    template <class> friend class bit_pointer;
    
    // Types
    public:
    using underlying_type = UIntType;
    using size_type = std::size_t;
    using difference_type = std::intmax_t;
     
    // Lifecycle
    public:
    template <class T> 
    constexpr bit_pointer(const bit_pointer<T>& other) noexcept;
    bit_pointer() noexcept = default;
    constexpr bit_pointer(std::nullptr_t, size_type = 0) noexcept;
    constexpr bit_pointer(underlying_type* ptr, size_type pos);
    
    // Assignment
    public:
    template <class T> 
    bit_pointer& operator=(const bit_pointer<T>& other) noexcept;
    bit_pointer& operator=(const bit_pointer& other) noexcept;

    // Conversion
    public:
    explicit constexpr operator bool() const noexcept;

    // Access
    public:
    constexpr bit_reference<UIntType> operator*() const noexcept;
    constexpr bit_reference<UIntType>* operator->() const noexcept;
    constexpr bit_reference<UIntType> operator[](difference_type n) const;
    
    // Increment and decrement operators
    public:
    bit_pointer& operator++();
    bit_pointer& operator--();
    bit_pointer operator++(int);
    bit_pointer operator--(int);
    constexpr bit_pointer operator+(difference_type n) const;
    constexpr bit_pointer operator-(difference_type n) const;
    bit_pointer& operator+=(difference_type n);
    bit_pointer& operator-=(difference_type n);
    
    // Implementation details: data members
    private:
    bit_reference<UIntType> _ref;

    // Non-member arithmetic operators
    template <class T>
    friend constexpr bit_pointer<T> operator+(
        typename bit_pointer<T>::difference_type n,
        const bit_pointer<T>& x
    );
    template <class T, class U>
    friend typename std::common_type<
        typename bit_pointer<T>::difference_type,
        typename bit_pointer<U>::difference_type
    >::type operator-(
        const bit_pointer<T>& lhs,
        const bit_pointer<U>& rhs
    );

    // Comparison operators
    template <class T, class U>
    friend constexpr bool operator==(
        const bit_pointer<T>& lhs, 
        const bit_pointer<U>& rhs
    ) noexcept;
    template <class T, class U>
    friend constexpr bool operator!=(
        const bit_pointer<T>& lhs, 
        const bit_pointer<U>& rhs
    ) noexcept;
    template <class T, class U>
    friend constexpr bool operator<(
        const bit_pointer<T>& lhs, 
        const bit_pointer<U>& rhs
    ) noexcept;
    template <class T, class U>
    friend constexpr bool operator<=(
        const bit_pointer<T>& lhs, 
        const bit_pointer<U>& rhs
    ) noexcept;
    template <class T, class U>
    friend constexpr bool operator>(
        const bit_pointer<T>& lhs, 
        const bit_pointer<U>& rhs
    ) noexcept;
    template <class T, class U>
    friend constexpr bool operator>=(
        const bit_pointer<T>& lhs, 
        const bit_pointer<U>& rhs
    ) noexcept;
};
                 
// Make function
template <class T>
constexpr bit_pointer<T> make_bit_pointer(
    T* ptr, 
    typename bit_pointer<T>::size_type pos
);
/* ************************************************************************** */



// ------------------------- BIT POINTER: LIFECYCLE ------------------------- //
// Constructs the pointer from another bit pointer differently qualified
template <class UIntType>
template <class T>
constexpr bit_pointer<UIntType>::bit_pointer(
    const bit_pointer<T>& other
) noexcept
: _ref(other._ref)
{
}

// Constructs a null bit pointer
template <class UIntType>
constexpr bit_pointer<UIntType>::bit_pointer(
    std::nullptr_t, 
    size_type
) noexcept
: _ref(nullptr)
{
}

// Constructs the bit pointer from an unsigned integer and a position
template <class UIntType>
constexpr bit_pointer<UIntType>::bit_pointer(
    underlying_type* ptr, 
    size_type pos
)
: _ref(ptr, pos)
{
}
// -------------------------------------------------------------------------- //



// ------------------------ BIT POINTER: ASSIGNMENT ------------------------- //
// Assigns a bit pointer differently qualified to the bit pointer
template <class UIntType>
template <class T> 
bit_pointer<UIntType>& bit_pointer<UIntType>::operator=(
    const bit_pointer<T>& other
) noexcept
{
    _ref._ptr = other._ref._ptr;
    _ref._mask = other._ref._mask;
    return *this;
}
    
// Assigns a bit pointer to the bit pointer
template <class UIntType>
bit_pointer<UIntType>& bit_pointer<UIntType>::operator=(
    const bit_pointer& other
) noexcept
{
    _ref._ptr = other._ref._ptr;
    _ref._mask = other._ref._mask;
    return *this;
}
// -------------------------------------------------------------------------- //



// ------------------------ BIT POINTER: CONVERSION ------------------------- //
// Returns true if the bit pointer is null, false otherwise
template <class UIntType>
constexpr bit_pointer<UIntType>::operator bool(
) const noexcept
{
    return _ref._ptr;
}
// -------------------------------------------------------------------------- //



// -------------------------- BIT POINTER: ACCESS --------------------------- //
// Dereferences the pointer, returning a bit reference
template <class UIntType>
constexpr bit_reference<UIntType> bit_pointer<UIntType>::operator*(
) const noexcept
{
    return _ref;
}

// Gets a pointer to a bit reference
template <class UIntType>
constexpr bit_reference<UIntType>* bit_pointer<UIntType>::operator->(
) const noexcept
{
    return _ref._addressof();
}

// Gets a bit reference, decrementing or incrementing the pointer
template <class UIntType>
constexpr bit_reference<UIntType> bit_pointer<UIntType>::operator[](
    difference_type n
) const
{
    using type = difference_type;
    constexpr type digits = std::numeric_limits<underlying_type>::digits;
    const type sum = _ref.position() + n;
    type diff = sum / digits;
    if (n < 0 && diff * digits != sum) {
        --diff;
    }
    return bit_reference<UIntType>(_ref._ptr + diff, sum - diff * digits);
}
// -------------------------------------------------------------------------- //



// ------------- BIT POINTER: INCREMENT AND DECREMENT OPERATORS ------------- //
// Increments the bit pointer and returns it
template <class UIntType>
bit_pointer<UIntType>& bit_pointer<UIntType>::operator++(
)
{
    using type = typename std::remove_cv<underlying_type>::type;
    constexpr size_type digits = std::numeric_limits<underlying_type>::digits;
    constexpr type one = 1;
    constexpr type mask = one; 
    const size_type pos = _ref.position();
    if (pos + 1 < digits) {
        _ref._mask <<= 1;
    } else {
        ++_ref._ptr;
        _ref._mask = mask;
    }
    return *this;
}

// Decrements the bit pointer and returns it
template <class UIntType>
bit_pointer<UIntType>& bit_pointer<UIntType>::operator--(
)
{
    using type = typename std::remove_cv<underlying_type>::type;
    constexpr size_type digits = std::numeric_limits<underlying_type>::digits;
    constexpr type one = 1;
    constexpr type mask = static_cast<type>(one << (digits - 1)); 
    const size_type pos = _ref.position();
    if (pos) {
        _ref._mask >>= 1;
    } else {
        --_ref._ptr;
        _ref._mask = mask;
    }
    return *this;
}

// Increments the bit pointer and returns the old one
template <class UIntType>
bit_pointer<UIntType> bit_pointer<UIntType>::operator++(
    int
)
{
    bit_pointer old = *this;
    ++(*this);
    return old;
}

// Decrements the bit pointer and returns the old one
template <class UIntType>
bit_pointer<UIntType> bit_pointer<UIntType>::operator--(
    int
)
{
    bit_pointer old = *this;
    --(*this);
    return old;
}

// Looks forward several bits and gets a pointer a this position
template <class UIntType>
constexpr bit_pointer<UIntType> bit_pointer<UIntType>::operator+(
    difference_type n
) const
{
    using type = difference_type;
    constexpr type digits = std::numeric_limits<underlying_type>::digits;
    const type sum = _ref.position() + n;
    type diff = sum / digits;
    if (n < 0 && diff * digits != sum) {
        --diff;
    }
    return bit_pointer(_ref._ptr + diff, sum - diff * digits);
}

// Looks backward several bits and gets a pointer a this position
template <class UIntType>
constexpr bit_pointer<UIntType> bit_pointer<UIntType>::operator-(
    difference_type n
) const
{
    using type = difference_type;
    constexpr type digits = std::numeric_limits<underlying_type>::digits;
    const type sum = _ref.position() - n;
    type diff = sum / digits;
    if (n > 0 && diff * digits != sum) {
        --diff;
    }
    return bit_pointer(_ref._ptr + diff, sum - diff * digits);
}

// Increments the pointer by several bits and returns it
template <class UIntType>
bit_pointer<UIntType>& bit_pointer<UIntType>::operator+=(
    difference_type n
)
{
    *this = *this + n;
    return *this;
}

// Decrements the pointer by several bits and returns it
template <class UIntType>
bit_pointer<UIntType>& bit_pointer<UIntType>::operator-=(
    difference_type n
)
{
    *this = *this - n;
    return *this;
}
// -------------------------------------------------------------------------- //



// -------------- BIT POINTER: NON-MEMBER ARITHMETIC OPERATORS -------------- //
// Advances the bit pointer by several bits
template <class T>
constexpr bit_pointer<T> operator+(
    typename bit_pointer<T>::difference_type n,
    const bit_pointer<T>& x
)
{
    return x + n;
}

// Computes the distance in bits separating two bit pointers
template <class T, class U>
typename std::common_type<
    typename bit_pointer<T>::difference_type,
    typename bit_pointer<U>::difference_type
>::type operator-(
    const bit_pointer<T>& lhs,
    const bit_pointer<U>& rhs
)
{
    using lhs_type = typename bit_pointer<T>::difference_type;
    using rhs_type = typename bit_pointer<U>::difference_type;
    using difference_type = typename std::common_type<lhs_type, rhs_type>::type;
    constexpr difference_type lhs_digits = std::numeric_limits<T>::digits;
    constexpr difference_type rhs_digits = std::numeric_limits<U>::digits;
    constexpr difference_type digits = rhs_digits;
    static_assert(lhs_digits == rhs_digits, "");
    const difference_type main = (lhs._ref.address() - rhs._ref.address());
    return main * digits + (lhs._ref.position() - rhs._ref.position());
}
// -------------------------------------------------------------------------- //



// ------------------- BIT POINTER: COMPARISON OPERATORS -------------------- //
// Computes equality comparison
template <class T, class U>
constexpr bool operator==(
    const bit_pointer<T>& lhs, 
    const bit_pointer<U>& rhs
) noexcept
{
    return lhs._ref.address() == rhs._ref.address() 
        && lhs._ref.position() == rhs._ref.position();
}

// Computes non-equality comparison
template <class T, class U>
constexpr bool operator!=(
    const bit_pointer<T>& lhs, 
    const bit_pointer<U>& rhs
) noexcept
{
    return lhs._ref.address() != rhs._ref.address() 
        || lhs._ref.position() != rhs._ref.position();
}

// Computes less than comparison
template <class T, class U>
constexpr bool operator<(
    const bit_pointer<T>& lhs, 
    const bit_pointer<U>& rhs
) noexcept
{
    return lhs._ref.address() < rhs._ref.address() 
        || (lhs._ref.address() == rhs._ref.address()
            && lhs._ref.position() < rhs._ref.position());
}

// Computes less than or equal comparison
template <class T, class U>
constexpr bool operator<=(
    const bit_pointer<T>& lhs, 
    const bit_pointer<U>& rhs
) noexcept
{
    return lhs._ref.address() < rhs._ref.address() 
        || (lhs._ref.address() == rhs._ref.address()
            && lhs._ref.position() <= rhs._ref.position());
}

// Computes greater than comparison
template <class T, class U>
constexpr bool operator>(
    const bit_pointer<T>& lhs, 
    const bit_pointer<U>& rhs
) noexcept
{
    return lhs._ref.address() > rhs._ref.address() 
        || (lhs._ref.address() == rhs._ref.address()
            && lhs._ref.position() > rhs._ref.position());
}

// Computes greater than or equal comparison 
template <class T, class U>
constexpr bool operator>=(
    const bit_pointer<T>& lhs, 
    const bit_pointer<U>& rhs
) noexcept
{
    return lhs._ref.address() > rhs._ref.address() 
        || (lhs._ref.address() == rhs._ref.address()
            && lhs._ref.position() >= rhs._ref.position());
}
// -------------------------------------------------------------------------- //



// ----------------------------- MAKE FUNCTION ------------------------------ //
// Constructs a bit pointer from an unsigned integer pointer and a position
template <class T>
constexpr bit_pointer<T> make_bit_pointer(
  T* ptr,
  typename bit_pointer<T>::size_type pos
)
{
    return bit_pointer<T>(ptr, pos);
}
// -------------------------------------------------------------------------- //



// ========================================================================== //
} // namespace bit
#endif // _BIT_POINTER_HPP_INCLUDED
// ========================================================================== //
