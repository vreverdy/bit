// ============================== BIT ITERATOR ============================== //
// Project:         C++ bit library
// Name:            bit_iterator.hpp
// Description:     A template class emulating an iterator on bits
// Creator:         Vincent Reverdy
// Contributor(s):  Vincent Reverdy [2015-2016]
// License:         BSD 3-Clause License
// ========================================================================== //
#ifndef _BIT_ITERATOR_HPP_INCLUDED
#define _BIT_ITERATOR_HPP_INCLUDED
// ========================================================================== //



// ================================ PREAMBLE ================================ //
// C++ standard library
#include <limits>
#include <iterator>
#include <type_traits>
// Project sources
#include "bit_value.hpp"
#include "bit_reference.hpp"
#include "bit_pointer.hpp"
// Third-party libraries
// Miscellaneous
namespace bit {
// ========================================================================== //



/* ****************************** BIT ITERATOR ****************************** */
// Bit iterator class definition
template <class Iterator>
class bit_iterator
{
    // Assertions
    private:
    using _traits_t = std::iterator_traits<Iterator>;
    using _value_t = typename _traits_t::value_type;
    using _reference_t = typename _traits_t::reference;
    using _pointer_t = typename _traits_t::pointer;
    using _category_t =  typename _traits_t::iterator_category;
    using _no_reference_t = typename std::remove_reference<_reference_t>::type;
    using _no_pointer_t = typename std::remove_pointer<_pointer_t>::type;
    using _raw_value_t = typename std::remove_cv<_value_t>::type;
    using _raw_reference_t = typename std::remove_cv<_no_reference_t>::type;
    using _raw_pointer_t = typename std::remove_cv<_no_pointer_t>::type;
    static_assert(std::is_same<_raw_reference_t, _raw_value_t>::value, "");
    static_assert(std::is_same<_raw_pointer_t, _raw_value_t>::value, "");
    
    // Friendship
    template <class> friend class bit_iterator;
    
    // Types
    public:
    using iterator_type = Iterator;
    using underlying_type = _no_reference_t;
    using iterator_category = _category_t;
    using value_type = bit_value;
    using difference_type = std::intmax_t;
    using pointer = bit_pointer<underlying_type>;
    using reference = bit_reference<underlying_type>;
    using size_type = std::size_t;
    
    // Lifecycle
    public:
    template <class T> 
    constexpr bit_iterator(const bit_iterator<T>& other);
    constexpr bit_iterator();
    constexpr bit_iterator(const iterator_type& i, size_type pos);

    // Assignment
    public:
    template <class T>
    bit_iterator& operator=(const bit_iterator<T>& other) noexcept;
    
    // Access
    public:
    constexpr reference operator*() const noexcept;
    constexpr pointer operator->() const noexcept;
    constexpr reference operator[](difference_type n) const;

    // Increment and decrement operators
    public:
    bit_iterator& operator++();
    bit_iterator& operator--();
    bit_iterator operator++(int);
    bit_iterator operator--(int);
    constexpr bit_iterator operator+(difference_type n) const;
    constexpr bit_iterator operator-(difference_type n) const;
    bit_iterator& operator+=(difference_type n);
    bit_iterator& operator-=(difference_type n);

    // Underlying details
    public:
    constexpr iterator_type base() const;
    constexpr size_type position() const noexcept;
    
    // Implementation details: data members
    private:
    iterator_type _current;
    size_type _position;

    // Non-member arithmetic operators
    template <class T>
    friend constexpr bit_iterator<T> operator+(
        typename bit_iterator<T>::difference_type n,
        const bit_iterator<T>& x
    );
    template <class T, class U>
    friend typename std::common_type<
        typename bit_iterator<T>::difference_type,
        typename bit_iterator<U>::difference_type
    >::type operator-(
        const bit_iterator<T>& lhs,
        const bit_iterator<U>& rhs
    );
    
    // Comparison operators
    template <class T, class U>
    friend constexpr bool operator==(
        const bit_iterator<T>& lhs, 
        const bit_iterator<U>& rhs
    );
    template <class T, class U>
    friend constexpr bool operator!=(
        const bit_iterator<T>& lhs, 
        const bit_iterator<U>& rhs
    );
    template <class T, class U>
    friend constexpr bool operator<(
        const bit_iterator<T>& lhs, 
        const bit_iterator<U>& rhs
    );
    template <class T, class U>
    friend constexpr bool operator<=(
        const bit_iterator<T>& lhs, 
        const bit_iterator<U>& rhs
    );
    template <class T, class U>
    friend constexpr bool operator>(
        const bit_iterator<T>& lhs, 
        const bit_iterator<U>& rhs
    );
    template <class T, class U>
    friend constexpr bool operator>=(
        const bit_iterator<T>& lhs, 
        const bit_iterator<U>& rhs
    );
};

// Make function
template <class T>
constexpr bit_iterator<T> make_bit_iterator(
    const T& i, 
    typename bit_iterator<T>::size_type pos
);
/* ************************************************************************** */



// ------------------------ BIT ITERATOR: LIFECYCLE ------------------------- //
// Constructs the iterator from another bit iterator differently qualified
template <class UIntType>
template <class T>
constexpr bit_iterator<UIntType>::bit_iterator(
    const bit_iterator<T>& other
)
: _current(other._current)
, _position(other._position)
{
}

// Constructs an empty bit iterator
template <class UIntType>
constexpr bit_iterator<UIntType>::bit_iterator(
)
: _current()
, _position()
{
}

// Constructs the bit iterator from an iterator and a position
template <class UIntType>
constexpr bit_iterator<UIntType>::bit_iterator(
    const iterator_type& i, 
    size_type pos
)
: _current(i)
, _position((assert(pos < std::numeric_limits<underlying_type>::digits), pos))
{
}
// -------------------------------------------------------------------------- //



// ------------------------ BIT ITERATOR: ASSIGNMENT ------------------------ //
// Assigns a bit pointer differently qualified to the bit pointer
template <class UIntType>
template <class T> 
bit_iterator<UIntType>& bit_iterator<UIntType>::operator=(
    const bit_iterator<T>& other
) noexcept
{
    _current = other._current;
    _position = other._position;
    return *this;
}
// -------------------------------------------------------------------------- //



// -------------------------- BIT ITERATOR: ACCESS -------------------------- //
// Dereferences the iterator, returning a bit reference
template <class UIntType>
constexpr typename bit_iterator<UIntType>::reference 
bit_iterator<UIntType>::operator*(
) const noexcept
{
    return reference(*_current, _position);
}

// Gets a pointer to a bit
template <class UIntType>
constexpr typename bit_iterator<UIntType>::pointer 
bit_iterator<UIntType>::operator->(
) const noexcept
{
    return pointer(&*_current, _position);
}

// Gets a bit reference, decrementing or incrementing the iterator
template <class UIntType>
constexpr typename bit_iterator<UIntType>::reference 
bit_iterator<UIntType>::operator[](
    difference_type n
) const
{
    using type = difference_type;
    constexpr type digits = std::numeric_limits<underlying_type>::digits;
    const type sum = _position + n;
    type diff = sum / digits;
    if (n < 0 && diff * digits != sum) {
        --diff;
    }
    return reference(*(_current + diff), sum - diff * digits);
}
// -------------------------------------------------------------------------- //



// ------------ BIT ITERATOR: INCREMENT AND DECREMENT OPERATORS ------------- //
// Increments the bit iterator and returns it
template <class UIntType>
bit_iterator<UIntType>& bit_iterator<UIntType>::operator++(
)
{
    constexpr size_type digits = std::numeric_limits<underlying_type>::digits;
    if (_position + 1 < digits) {
        ++_position;
    } else {
        ++_current;
        _position = 0;
    }
    return *this;
}

// Decrements the bit iterator and returns it
template <class UIntType>
bit_iterator<UIntType>& bit_iterator<UIntType>::operator--(
)
{
    constexpr size_type digits = std::numeric_limits<underlying_type>::digits;
    if (_position) {
        --_position;
    } else {
        --_current;
        _position = digits - 1;
    }
    return *this;
}

// Increments the bit iterator and returns the old one
template <class UIntType>
bit_iterator<UIntType> bit_iterator<UIntType>::operator++(
    int
)
{
    bit_iterator old = *this;
    ++(*this);
    return old;
}

// Decrements the bit iterator and returns the old one
template <class UIntType>
bit_iterator<UIntType> bit_iterator<UIntType>::operator--(
    int
)
{
    bit_iterator old = *this;
    --(*this);
    return old;
}

// Looks forward several bits and gets a iterator a this position
template <class UIntType>
constexpr bit_iterator<UIntType> bit_iterator<UIntType>::operator+(
    difference_type n
) const
{
    using type = difference_type;
    constexpr type digits = std::numeric_limits<underlying_type>::digits;
    const type sum = _position + n;
    type diff = sum / digits;
    if (n < 0 && diff * digits != sum) {
        --diff;
    }
    return bit_iterator(_current + diff, sum - diff * digits);
}

// Looks backward several bits and gets a iterator a this position
template <class UIntType>
constexpr bit_iterator<UIntType> bit_iterator<UIntType>::operator-(
    difference_type n
) const
{
    using type = difference_type;
    constexpr type digits = std::numeric_limits<underlying_type>::digits;
    const type sum = _position - n;
    type diff = sum / digits;
    if (n > 0 && diff * digits != sum) {
        --diff;
    }
    return bit_iterator(_current + diff, sum - diff * digits);
}

// Increments the iterator by several bits and returns it
template <class UIntType>
bit_iterator<UIntType>& bit_iterator<UIntType>::operator+=(
    difference_type n
)
{
    *this = *this + n;
    return *this;
}

// Decrements the iterator by several bits and returns it
template <class UIntType>
bit_iterator<UIntType>& bit_iterator<UIntType>::operator-=(
    difference_type n
)
{
    *this = *this - n;
    return *this;
}
// -------------------------------------------------------------------------- //



// -------------------- BIT ITERATOR: UNDERLYING DETAILS -------------------- //
// Returns the iterator to the underlying object
template <class UIntType>
constexpr typename bit_iterator<UIntType>::iterator_type 
bit_iterator<UIntType>::base(
) const
{
    return _current;
}

// Returns the position of the bit within the underlying object
template <class UIntType>
constexpr typename bit_iterator<UIntType>::size_type 
bit_iterator<UIntType>::position(
) const noexcept
{
    return _position;
}
// -------------------------------------------------------------------------- //



// ------------- BIT ITERATOR: NON-MEMBER ARITHMETIC OPERATORS -------------- //
// Advances the bit iterator by several bits
template <class T>
constexpr bit_iterator<T> operator+(
    typename bit_iterator<T>::difference_type n,
    const bit_iterator<T>& x
)
{
    return x + n;
}

// Computes the distance in bits separating two bit iterators
template <class T, class U>
typename std::common_type<
    typename bit_iterator<T>::difference_type,
    typename bit_iterator<U>::difference_type
>::type operator-(
    const bit_iterator<T>& lhs,
    const bit_iterator<U>& rhs
)
{
    using type1 = typename bit_iterator<T>::underlying_type;
    using type2 = typename bit_iterator<U>::underlying_type;
    using lhs_diff = typename bit_iterator<T>::difference_type;
    using rhs_diff = typename bit_iterator<U>::difference_type;
    using difference_type = typename std::common_type<lhs_diff, rhs_diff>::type;
    constexpr difference_type lhs_digits = std::numeric_limits<type1>::digits;
    constexpr difference_type rhs_digits = std::numeric_limits<type2>::digits;
    constexpr difference_type digits = rhs_digits;
    static_assert(lhs_digits == rhs_digits, "");
    const difference_type main = lhs._current - rhs._current;
    return main * digits + (lhs._position - rhs._position);
}
// -------------------------------------------------------------------------- //



// ------------------- BIT ITERATOR: COMPARISON OPERATORS ------------------- //
// Computes equality comparison
template <class T, class U>
constexpr bool operator==(
    const bit_iterator<T>& lhs, 
    const bit_iterator<U>& rhs
)
{
    return lhs._current == rhs._current && lhs._position == rhs._position;
}

// Computes non-equality comparison
template <class T, class U>
constexpr bool operator!=(
    const bit_iterator<T>& lhs, 
    const bit_iterator<U>& rhs
)
{
    return lhs._current != rhs._current || lhs._position != rhs._position;
}

// Computes less than comparison
template <class T, class U>
constexpr bool operator<(
    const bit_iterator<T>& lhs, 
    const bit_iterator<U>& rhs
)
{
    return lhs._current < rhs._current 
        || (lhs._current == rhs._current && lhs._position < rhs._position);
}

// Computes less than or equal comparison
template <class T, class U>
constexpr bool operator<=(
    const bit_iterator<T>& lhs, 
    const bit_iterator<U>& rhs
)
{
    return lhs._current < rhs._current 
        || (lhs._current == rhs._current && lhs._position <= rhs._position);
}

// Computes greater than comparison
template <class T, class U>
constexpr bool operator>(
    const bit_iterator<T>& lhs, 
    const bit_iterator<U>& rhs
)
{
    return lhs._current > rhs._current 
        || (lhs._current == rhs._current && lhs._position > rhs._position);
}

// Computes greater than or equal comparison 
template <class T, class U>
constexpr bool operator>=(
    const bit_iterator<T>& lhs, 
    const bit_iterator<U>& rhs
)
{
    return lhs._current > rhs._current 
        || (lhs._current == rhs._current && lhs._position >= rhs._position);
}
// -------------------------------------------------------------------------- //



// ----------------------------- MAKE FUNCTION ------------------------------ //
// Constructs a bit iterator from an iterator and a position
template <class T>
constexpr bit_iterator<T> make_bit_iterator(
  const T& i,
  typename bit_iterator<T>::size_type pos
)
{
    return bit_iterator<T>(i, pos);
}
// -------------------------------------------------------------------------- //



// ========================================================================== //
} // namespace bit
#endif // _BIT_ITERATOR_HPP_INCLUDED
// ========================================================================== //

